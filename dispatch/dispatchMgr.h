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
	CPlayerDispatchPtr _playerAccountDispatch;	//注册玩家登录、注册帐号处理器
	CPlayerDispatchPtr _playerDispatch;			//注册玩家上线后的操作处理器
	CSystemDispatchPtr _systemDispatch;			//注册系统操作处理器
};