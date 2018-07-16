#include "dispatch.h"

void CPlayerDispatch::registerPlayerHandlr(const std::string& key, PlayerDispachFunc func)
{
	if (_keyfunc.count(key) <= 0)
	{
		_keyfunc[key] = func;
	}
	else
	{
		assert(false);
	}
}

void CPlayerDispatch::onDispatchEvent(const std::string& key, CPlayerEntityPtr playerEntity, Json::Value js)
{
	if (_keyfunc.count(key) > 0)
	{
		(_keyfunc[key])(playerEntity, js);
	}
	else
	{
		playerEntity->errorMessage(ErrorMethodNotFound, "method not found");
	}
}

void CSystemDispatch::registerSystemHandle(const std::string& key, SystemDispatchFunc func)
{
	if (_keyfunc.count(key) <= 0)
	{
		_keyfunc[key] = func;
	}
	else
	{
		assert(false);
	}
}

void CSystemDispatch::onDispatchEvent(const std::string& key, Json::Value js)
{
	if (_keyfunc.count(key) > 0)
	{
		(_keyfunc[key])(js);
	}
}
