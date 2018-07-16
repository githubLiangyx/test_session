#include "playerHelper.h"
#include "../manager/CInviteKeyManager.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "../session/session.h"
#include "../manager/playerEntityManager.h"

void CPlayerHelper::do_register()
{
	CDispatchEventMgr* dispatch = CDispatchEventMgr::instance();
	dispatch->registerPlayerHandle("Chat", CPlayerHelper::OnChat);
}

EUserType CPlayerHelper::getUserTypeByInviteKey(std::string inviteKey)
{
	return CInviteKeyManager::instance()->getUserTypeByInviteKey(inviteKey);
}

void CPlayerHelper::makeCookieForClt(CPlayerEntityPtr entity)
{
	CHECK_NULLPTR_RETURN(entity);
	CSessionPtr session = entity->getSession();
	CHECK_NULLPTR_RETURN(session);
	std::string cookie = getRandomString();
	session->setCookie(cookie);
	Json::Value json;
	json["cookie"] = cookie;
	entity->messageToClt(ECmdSetCookie, json.toCutString());
}

std::string CPlayerHelper::getRandomString()
{
	boost::uuids::random_generator rgen;
	boost::uuids::uuid uid = rgen();
	return boost::lexical_cast<std::string>(uid);

}

void CPlayerHelper::OnChat(CPlayerEntityPtr entity, Json::Value json)
{
	CHECK_NULLPTR_RETURN(entity);
	EMsgType chatType = (EMsgType)json["chatType"].asInt();
	int targetId = json["targetId"].asInt();
	std::string content = json["content"].asString();
	if (chatType == EMsgChat)
	{
		CPlayerEntityPtr toEntity = CPlayerEntityManager::instance()->find(targetId);
		if (!toEntity)
		{
			entity->errorMessage(ErrorEntityNotOnLine, "player not on line");
			return;
		}
		Json::Value json;
		json["chatType"] = (int)chatType;
		json["fromId"] = entity->getEntityId().playerId;
		json["fromName"] = entity->getName();
		json["content"] = content;
		toEntity->messageToClt(ECmdMsg, json.toCutString());
	}
}
