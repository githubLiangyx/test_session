#pragma once
#include <functional>
#include "../entity/PlayerEntity.h"
#include <json/json.h>

class CPlayerDispatch
{
public:
	CPlayerDispatch() { _keyfunc.clear(); }
	typedef void(*PlayerDispachFunc)(CPlayerEntityPtr, Json::Value);
	void registerPlayerHandlr(const std::string&, PlayerDispachFunc);
	void onDispatchEvent(const std::string& key, CPlayerEntityPtr playerEntity, Json::Value js);
private:
	std::map<std::string, PlayerDispachFunc> _keyfunc;
};
typedef std::shared_ptr<CPlayerDispatch> CPlayerDispatchPtr;

class CSystemDispatch
{
public:
	typedef void (*SystemDispatchFunc)(Json::Value);

	void registerSystemHandle(const std::string&, SystemDispatchFunc);

	void onDispatchEvent(const std::string& key, Json::Value js);

private:
	std::map<std::string, SystemDispatchFunc> _keyfunc;
};
typedef std::shared_ptr<CSystemDispatch> CSystemDispatchPtr;