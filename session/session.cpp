#include "session.h"
#include "../entity/PlayerEntity.h"

session::session(boost::asio::ip::tcp::socket sock)
	:base_session(std::move(sock), ESessionClt)
{
	_owner = NULL;
	_dispatchMgr = CDispatchEventMgr::instance();
	_isDoingTask = false;
}
session::~session()
{
	std::cout << "destroy session remote: <<"<< std::endl;
}
void session::start()
{
	base_session::start();
	_owner = CPlayerEntityPtr(new CPlayerEntity(shared_self()));
}

std::shared_ptr<session> session::shared_self()
{
	auto self = shared_from_this();
	return std::static_pointer_cast<session>(self);
}

void session::onConnected()
{
	/////////
}

void session::onDisConneted()
{
	base_session::onDisConneted();

	CPlayerEntityPtr entity = _owner;
	//hasCookie is say the session has login
	if (hasCookie() && entity)
	{
		entity->onOffline();
	}
	_owner = NULL;
	_cookie.clear();
}

void session::__doTask()
{
	std::string recvdata;
	{
		std::unique_lock<std::mutex> mt(_doTaskMT);
		if (_taskMsgQue.empty())
		{
			_isDoingTask = false;
			return;
		}
		_isDoingTask = true;
		recvdata = _taskMsgQue.front();
		_taskMsgQue.pop_front();
	}

	if (!recvdata.empty())
	{
		_dispatchMgr->onSessionMessage(shared_self(), recvdata);
	}
	__doTask();
}

void session::onRecv(const std::string & recvdata)
{
	bool doTask = false;
	{
		std::unique_lock<std::mutex> mt(_doTaskMT);
		doTask = _isDoingTask;
		_taskMsgQue.push_back(recvdata);
	}
	if (!doTask)
	{
		__doTask();
	}
}
