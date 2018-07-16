#pragma once
#include <boost/noncopyable.hpp>
#include "../entity/PlayerEntity.h"

class CPlayerEntityManager
	: public boost::noncopyable
{
public:
	static CPlayerEntityManager* instance();

	void addPlayer(CPlayerEntityPtr entity);

	void removePlayer(CPlayerEntityPtr entity);

	CPlayerEntityPtr find(const SEntityId& entity);

	CPlayerEntityPtr find(const int& playerId);

	CPlayerEntityPtr find(const std::string& name);

private:
	CPlayerEntityManager() {}

private:
	//��֤�ڴ˶����е����һ������
	std::map<SEntityId, CPlayerEntityPtr> _entityIdToEntity;
	std::map<std::string, CPlayerEntityPtr> _nameToEntity;
};