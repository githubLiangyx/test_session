#include "playerEntityManager.h"

CPlayerEntityManager * CPlayerEntityManager::instance()
{
	static CPlayerEntityManager singleton;
	return &singleton;
}

void CPlayerEntityManager::addPlayer(CPlayerEntityPtr entity)
{
	CHECK_NULLPTR_RETURN(entity);
	_entityIdToEntity[entity->getEntityId()] = entity;
	_nameToEntity[entity->getName()] = entity;
}

void CPlayerEntityManager::removePlayer(CPlayerEntityPtr entity)
{
	CHECK_NULLPTR_RETURN(entity);
	SEntityId entityId = entity->getEntityId();
	std::string name = entity->getName();
	if (_entityIdToEntity.count(entityId) > 0)
	{
		_entityIdToEntity.erase(entityId);
	}
	if (_nameToEntity.count(name) > 0)
	{
		_nameToEntity.erase(name);
	}
}

CPlayerEntityPtr CPlayerEntityManager::find(const SEntityId & entityId)
{
	if (_entityIdToEntity.count(entityId) > 0)
	{
		return _entityIdToEntity[entityId];
	}
	return NULL;
}

CPlayerEntityPtr CPlayerEntityManager::find(const int & playerId)
{
	SEntityId entityId;
	entityId.playerId = playerId;
	return find(entityId);
}

CPlayerEntityPtr CPlayerEntityManager::find(const std::string & name)
{
	if (_nameToEntity.count(name) > 0)
	{
		return _nameToEntity[name];
	}
	return NULL;
}
