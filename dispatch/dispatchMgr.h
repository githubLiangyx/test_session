#pragma once
#include "dispatch.h"
#include "../common/base_session.h"

class session;
class CDispatchEventMgr
{
public:
	static CDispatchEventMgr* instance();

	static void start();

	void registerPlayerHandle(const std::string& key, CPlayerDispatch::PlayerDispachFunc func);

	void registerPlayerAccountHandle(const std::string& key, CPlayerDispatch::PlayerDispachFunc func);

	void registerSystemHandle(const std::string& key, CSystemDispatch::SystemDispatchFunc func);

	void onPlayerAccountMessage(const std::string& key, CPlayerEntityPtr entity, Json::Value js);

	void onPlayerMessage(const std::string& key, CPlayerEntityPtr entity, Json::Value js);

	void onSystemMessage(const std::string& key, Json::Value js);

	void onSessionMessage(CBaseSession session, const std::string& msg);

private:
	void onCltSessionMessage(std::shared_ptr<session> cltSession, const std::string& msg);

	void onSvrSessionMessage(CBaseSession session, const std::string& msg);

private:
	CDispatchEventMgr();

private:
	CPlayerDispatchPtr _playerAccountDispatch;	//ע����ҵ�¼��ע���ʺŴ�����
	CPlayerDispatchPtr _playerDispatch;			//ע��������ߺ�Ĳ���������
	CSystemDispatchPtr _systemDispatch;			//ע��ϵͳ����������
};