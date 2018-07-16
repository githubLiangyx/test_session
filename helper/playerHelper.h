#pragma once
#include <boost/noncopyable.hpp>
#include "../entity/PlayerEntity.h"
#include <json/json.h>

class CPlayerHelper :
	public boost::noncopyable
{
public:
	static void do_register();

	static EUserType getUserTypeByInviteKey(std::string inviteKey);

	static void makeCookieForClt(CPlayerEntityPtr entity);

	static std::string getRandomString();

	static void OnChat(CPlayerEntityPtr entity, Json::Value json);
};