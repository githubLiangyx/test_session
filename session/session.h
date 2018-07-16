#pragma once
#include <memory>
#include "../common/base_session.h"
#include "../dispatch/dispatchMgr.h"
#include "../entity/PlayerEntity.h"

class session
	: public base_session
{
public:

	session(boost::asio::ip::tcp::socket sock);

	~session();

	void start();

	CPlayerEntityPtr getOwner() { return _owner; }

	bool hasCookie() { return false == _cookie.empty(); }

	void setCookie(const std::string& cookie) { _cookie = cookie; }

	std::string getCookie() { return _cookie; }

	std::shared_ptr<session> shared_self();

	//下面是一些事件
public:
	virtual void onConnected();

	virtual void onDisConneted();

	virtual void onRecv(const std::string& recvdata);

private:
	void __doTask();

private:
	CPlayerEntityPtr _owner;					//拥有者
	std::deque<std::string> _taskMsgQue;		//接收任务队列
	bool _isDoingTask;
	std::mutex _doTaskMT;

	CDispatchEventMgr* _dispatchMgr;			//分发管理器
	std::string _cookie;						//通信认证标识
};
typedef std::shared_ptr<session> CSessionPtr;