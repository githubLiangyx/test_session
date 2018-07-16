#include "dispatchMgr.h"
#include "../session/session.h"
#include "../helper/AccoutHelper.h"
#include "../helper/playerHelper.h"

CDispatchEventMgr * CDispatchEventMgr::instance()
{
	static CDispatchEventMgr singleton;
	return &singleton;
}

void CDispatchEventMgr::start()
{
	CAccoutHelper::do_register();
	CPlayerHelper::do_register();
}

void CDispatchEventMgr::registerPlayerHandle(const std::string & key, CPlayerDispatch::PlayerDispachFunc func)
{
	if (false == key.empty())
	{
		_playerDispatch->registerPlayerHandlr(key, func);
	}
}

void CDispatchEventMgr::registerPlayerAccountHandle(const std::string & key, CPlayerDispatch::PlayerDispachFunc func)
{
	if (false == key.empty())
	{
		_playerAccountDispatch->registerPlayerHandlr(key, func);
	}
}

void CDispatchEventMgr::registerSystemHandle(const std::string & key, CSystemDispatch::SystemDispatchFunc func)
{
	if (false == key.empty())
	{
		_systemDispatch->registerSystemHandle(key, func);
	}
}

void CDispatchEventMgr::onPlayerAccountMessage(const std::string & key, CPlayerEntityPtr entity, Json::Value js)
{
	_playerAccountDispatch->onDispatchEvent(key, entity, js);
}

void CDispatchEventMgr::onPlayerMessage(const std::string & key, CPlayerEntityPtr entity, Json::Value js)
{
	try
	{
		_playerDispatch->onDispatchEvent(key, entity, js);
	}
	catch (const std::exception& e)
	{
		assert(entity);
	}
}

void CDispatchEventMgr::onSystemMessage(const std::string & key, Json::Value js)
{
	try
	{
		_systemDispatch->onDispatchEvent(key, js);
	}
	catch (const std::exception&)
	{
		// ...
	}
}

void CDispatchEventMgr::onSessionMessage(CBaseSession baseSS, const std::string & msg)
{
	CHECK_NULLPTR_RETURN(baseSS);
	ESessionType type = baseSS->getSessionType();
	switch (type)
	{
	case ESessionClt:
		onCltSessionMessage(std::static_pointer_cast<session>(baseSS), msg);
		break;
	}
}

void CDispatchEventMgr::onCltSessionMessage(std::shared_ptr<session> cltSession, const std::string & msg)
{
	CHECK_NULLPTR_RETURN(cltSession);
	CPlayerEntityPtr playerEntity = cltSession->getOwner();
	assert(playerEntity);
	try
	{
		Json::Value json;
		bool ret = json.parse(msg);
		if (ret && json["method"].isString())
		{
			std::string key = json["method"].asString();
			std::string cltCookie = json["cookie"].asString();
			std::string cookie = cltSession->getCookie();
			Json::Value contentJs;
			{
				std::string content = json["content"].asString();
				bool ret2 = contentJs.parse(content);
			}

			if (cookie.empty())
			{
				onPlayerAccountMessage(key, playerEntity, contentJs);
			}
			else if (cltCookie == cookie)
			{
				onPlayerMessage(key, playerEntity, contentJs);
			}
			else
			{
				playerEntity->errorMessage(ErrorInValidCookie, "cookie has been timeout");
			}
		}
		else
		{
			playerEntity->errorMessage(ErrorInvalidParam, "params is incorrect");
		}
	}
	catch (const std::exception&)
	{
		playerEntity->errorMessage(ErrorUnkowExp, "unkow exception");
	}
}

void CDispatchEventMgr::onSvrSessionMessage(CBaseSession session, const std::string & msg)
{

}

CDispatchEventMgr::CDispatchEventMgr()
{
	_playerAccountDispatch = CPlayerDispatchPtr(new CPlayerDispatch);
	_playerDispatch = CPlayerDispatchPtr(new CPlayerDispatch);
	_systemDispatch = CSystemDispatchPtr(new CSystemDispatch);
}
