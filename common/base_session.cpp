#include "base_session.h"

base_session::base_session(boost::asio::ip::tcp::socket sock, ESessionType ct)
	:_handler(std::move(sock))
{
	_stype = ct;
	_isSending = false;
}

void base_session::start()
{
	onConnected();
	STransProtocolPtr buf = STransProtocolPtr(new STransProtocol);
	buf->need_total_data = 0;
	auto self = shared_from_this();

	boost::asio::async_read(_handler, boost::asio::buffer(buf->u_data, SOCK_DATA_MAX_LEN),
		std::bind(&base_session::read_complete_handler, self, buf, std::placeholders::_1, std::placeholders::_2),
		std::bind(&base_session::read_handler, self, buf, std::placeholders::_1, std::placeholders::_2));
}

void base_session::sendToClt(const std::string & msg)
{
	std::string quemsg;
	{
		char lenmsg[PROTO_HEAD_LEN + 1];
		sprintf_s(lenmsg, "%.4d", msg.length() + 4);
		quemsg = std::string(lenmsg, PROTO_HEAD_LEN);
		quemsg.append(msg);
	}

	bool isSend = false;
	{
		std::unique_lock<std::mutex> lk(_sendMT);
		_sendMsgQue.push_back(quemsg);
		isSend = _isSending;
	}
	if (!isSend)
	{
		std::shared_ptr<std::string> msg(new std::string);
		__sendQueMsgToClt(msg);
	}
}

size_t base_session::read_complete_handler(STransProtocolPtr buf, boost::system::error_code ec, size_t size)
{
	if (!ec)
	{
		if (buf->need_total_data > 0)
		{
			return buf->need_total_data - size;
		}
		else if (size < PROTO_HEAD_LEN)
		{
			return PROTO_HEAD_LEN - size;
		}
		else
		{
			try
			{
				std::string lenStr(buf->u_data, PROTO_HEAD_LEN);
				buf->need_total_data = atoi(lenStr.c_str());

				if (buf->need_total_data <= 0 || buf->need_total_data > SOCK_PACK_MAX_LEN)
				{
					buf->need_total_data = 0;
					return 0;
				}
				return buf->need_total_data - size;
			}
			catch (const std::exception&)
			{
				return 0;
			}
		}
	}
	return 0;
}

void base_session::read_handler(STransProtocolPtr buf, boost::system::error_code ec, size_t size)
{
	if (!ec)
	{
		int need_total = buf->need_total_data;
		if (need_total == 0)
		{
			if (size == 0)
			{
				assert(false);
				//unable to here !!
			}

			//protocal error!
			onDisConneted();
			return;
		}
		else if (need_total> size)
		{
			buf->need_total_data -= size;
			if (_cache.empty())
			{
				_cache = std::string(buf->u_data + PROTO_HEAD_LEN, size - PROTO_HEAD_LEN);
			}
			else
			{
				_cache.append(buf->u_data, size);
			}
		}
		else if (need_total == size)
		{
			std::string recvdata;
			if (!_cache.empty())
			{
				recvdata = std::move(_cache);
				recvdata.append(buf->u_data, size);
			}
			else
			{
				recvdata.append(buf->u_data + PROTO_HEAD_LEN, size - PROTO_HEAD_LEN);
			}
			buf->need_total_data = 0;

			try
			{
				onRecv(recvdata);
			}
			catch (const std::exception&)
			{
				//处理过程出错 应该返回错误码 因为在逻辑层已经捕获过了 因此传输层不用处理
			}
		}
		else
		{
			assert(false);
			//unable to here 
		}
		auto self = shared_from_this();
		boost::asio::async_read(_handler, boost::asio::buffer(buf->u_data, SOCK_DATA_MAX_LEN),
			std::bind(&base_session::read_complete_handler, self, buf, std::placeholders::_1, std::placeholders::_2),
			std::bind(&base_session::read_handler, self, buf, std::placeholders::_1, std::placeholders::_2));
	}
	else
	{
		//连接中断
		onDisConneted();
	}
}

void base_session::write_handler(std::shared_ptr<std::string> msg_ptr, boost::system::error_code ec, size_t size)
{
	if (!ec)
	{
		__sendQueMsgToClt(msg_ptr);
	}
	else if (_handler.is_open())
	{
		assert(false);
	}
}

void base_session::__sendQueMsgToClt(std::shared_ptr<std::string> msg)
{
	{
		std::unique_lock<std::mutex> lk(_sendMT);
		if (false == _sendMsgQue.empty())
		{
			*msg = _sendMsgQue.front();
			_sendMsgQue.pop_front();
			_isSending = true;
		}
		else
		{
			_isSending = false;
			return;
		}
	}

	assert(!msg->empty());
	boost::asio::async_write(_handler, boost::asio::buffer(*msg), std::bind(&base_session::write_handler, shared_from_this(), msg, std::placeholders::_1, std::placeholders::_2));

}