#pragma once
#include "../dispatch/dispatchMgr.h"
class CAccoutHelper
{
public:
	static void do_register();

	static void OnRegister(CPlayerEntityPtr entity, Json::Value json);

	static void OnLogin(CPlayerEntityPtr entity, Json::Value json);
};