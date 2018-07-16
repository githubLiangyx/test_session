#pragma once
#include "common.h"
#include <deque>
#include <string>
#include <mutex>
#include <boost/asio.hpp>

class base_session
	: public std::enable_shared_from_this<base_session>
{
public:
	base_session(boost::asio::ip::tcp::socket sock, ESessionType ct);

public:
	virtual void onConnected() {}

	virtual void onDisConneted() { _handler.close(); }

	virtual void onRecv(const std::string& recvdata) {}

public:
	void start();

	void sendToClt(const std::string& msg);

	ESessionType getSessionType() { return _stype; }
	//下面是 boost使用的回调 不要乱加
private:
	//============================================================================
	size_t read_complete_handler(STransProtocolPtr buf, boost::system::error_code ec, size_t size);

	void read_handler(STransProtocolPtr buf, boost::system::error_code ec, size_t size);

	void write_handler(std::shared_ptr<std::string> msg_ptr, boost::system::error_code ec, size_t size);
	//=============================================================================

private:
	void __sendQueMsgToClt(std::shared_ptr<std::string> msg);

private:
	ESessionType _stype;
	std::string _cache;			//分包缓存
	std::deque<std::string> _sendMsgQue;		//发送队列
	bool _isSending;
	std::mutex _sendMT;

	//CDispatchEventMgr* _dispatchMgr;			//分发管理器
	std::string _cookie;

	//for test 
	boost::asio::ip::tcp::endpoint _remote;
	boost::asio::ip::tcp::socket _handler;		//核心descriptor
};
typedef std::shared_ptr<base_session> CBaseSession;