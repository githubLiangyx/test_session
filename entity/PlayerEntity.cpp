#include "PlayerEntity.h"
#include "../component/permissionComponent.h"
#include "../session/session.h"
#include <boost/date_time.hpp>
#include "../module/userInfoModule.h"
#include "../manager/playerEntityManager.h"

CPlayerEntity::CPlayerEntity(std::shared_ptr<session> sess)
{
	_session = sess;
}

CPlayerEntity::~CPlayerEntity()
{
	std::cout << "destroy playerentity" << std::endl;
}

void CPlayerEntity::addComponent(EEntityComponentType type, CObjectPtr component)
{
	_component[type] = component;
}

CObjectPtr CPlayerEntity::getComponent(EEntityComponentType type)
{
	if (_component.count(type))
	{
		return _component[type];
	}
	return NULL;
}

void CPlayerEntity::removeComponent(EEntityComponentType type)
{
	if (_component.count(type))
	{
		_component.erase(type);
	}
}

void CPlayerEntity::errorMessage(ErrorCode ec, const std::string & errDesc)
{
	Json::Value js;
	js["errorCode"] = (int)ec;
	js["errDesc"] = errDesc;
	js["command"] = (int)ECmdError;
	serializeDataToClt(js.toCutString());
}

void CPlayerEntity::messageToClt(ECmdType cmd, const std::string & content, ErrorCode ec, const std::string errDesc)
{
	Json::Value js;
	js["errorCode"] = (int)ec;
	js["errDesc"] = errDesc;
	js["content"] = content;
	js["command"] = (int)cmd;
	serializeDataToClt(js.toCutString());
}

void CPlayerEntity::onOffline()
{
	for (std::map<ETB_MODULE, CPlayerModulePtr>::const_iterator it = _module.begin(); it != _module.end(); ++it)
	{
		it->second->onOffline();
	}
	//用户下线 移出playerEntityMgr队列
	CPlayerEntityManager::instance()->removePlayer(shared_from_this());

	//======== last do this
	_module.clear();
	_component.clear();
	_session = NULL;
}

void CPlayerEntity::onLine()
{
	//注册用户到playerEntityMgr
	CPlayerEntityManager::instance()->addPlayer(shared_from_this());

	for (std::map<ETB_MODULE, CPlayerModulePtr>::const_iterator it = _module.begin(); it != _module.end(); ++it)
	{
		it->second->onLine();
	}
}

void CPlayerEntity::onLoadData(const std::map<ETB_MODULE, DbDataBasePtr>& data)
{
	for (std::map<ETB_MODULE, DbDataBasePtr>::const_iterator it = data.begin(); it != data.end(); it++)
	{
		if (_module.count(it->first) > 0)
		{
			_module[it->first]->onLoadData(it->second);
		}
	}
	// load data finish 
	onLine();
}

std::string CPlayerEntity::getName()
{
	CUserInfoModulePtr userModule = getPlayerModule<CUserInfoModule>(ETB_USER_INFO);
	assert(userModule);
	return userModule->getName();
}

void CPlayerEntity::registerModule()
{
	auto self = shared_from_this();
	registerModuleType(ETB_USER_INFO, CUserInfoModulePtr(new CUserInfoModule(self)));
}

void CPlayerEntity::serializeDataToClt(const std::string & msg)
{
	if (false == msg.empty())
	{
		assert(_session);
		_session->sendToClt(msg);
	}
}

void CPlayerEntity::registerModuleType(ETB_MODULE type, CPlayerModulePtr pModule)
{
	assert(pModule);
	_module[type] = pModule;
}
