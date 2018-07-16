#pragma once
#include <memory>
#include <boost/asio.hpp>
#include "../common/common.h"
#include "../common/proto_head_common.h"
#include "../module/playerModule.h"

class session;

class CPlayerEntity
	: public boost::noncopyable, public CObject, public std::enable_shared_from_this<CPlayerEntity>
{
public:
	CPlayerEntity(std::shared_ptr<session> session);

	~CPlayerEntity();

	void addComponent(EEntityComponentType type, CObjectPtr component);

	CObjectPtr getComponent(EEntityComponentType type);

	void removeComponent(EEntityComponentType type);

	void errorMessage(ErrorCode ec, const std::string& errDesc);

	void messageToClt(ECmdType cmd, const std::string& content, ErrorCode er = ErrorNot, const std::string errDesc = "");

	std::shared_ptr<session> getSession() { return _session; }

	void setEntityId(const SEntityId& entityId) { _playerId = entityId; }

	SEntityId getEntityId() { return _playerId; }

	void onLoadData(const std::map<ETB_MODULE, DbDataBasePtr>& data);

	void onOffline();

	void onLine();

	template <class T>
	std::shared_ptr<T> getPlayerModule(ETB_MODULE moduleType);

	std::string getName();

	void registerModule();

private:
	void serializeDataToClt(const std::string& msg);

	void registerModuleType(ETB_MODULE type, CPlayerModulePtr pModule);

private:
	std::map<EEntityComponentType, CObjectPtr> _component;
	std::shared_ptr<session> _session;

	SEntityId _playerId;

	//======module data
	std::map<ETB_MODULE, CPlayerModulePtr> _module;
};

typedef std::shared_ptr<CPlayerEntity> CPlayerEntityPtr;

template<class T>
inline std::shared_ptr<T> CPlayerEntity::getPlayerModule(ETB_MODULE moduleType)
{
	if (_module.count(moduleType) > 0)
	{
		return std::static_pointer_cast<T>(_module[moduleType]);
	}
	return NULL;
}
