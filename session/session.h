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

	//������һЩ�¼�
public:
	virtual void onConnected();

	virtual void onDisConneted();

	virtual void onRecv(const std::string& recvdata);

private:
	void __doTask();

private:
	CPlayerEntityPtr _owner;					//ӵ����
	std::deque<std::string> _taskMsgQue;		//�����������
	bool _isDoingTask;
	std::mutex _doTaskMT;

	CDispatchEventMgr* _dispatchMgr;			//�ַ�������
	std::string _cookie;						//ͨ����֤��ʶ
};
typedef std::shared_ptr<session> CSessionPtr;