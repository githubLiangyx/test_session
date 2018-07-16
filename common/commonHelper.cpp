#include "commonHelper.h"
#include "common.h"

SEntityId CCommonHelper::makeEntityId(const int playerId)
{
	SEntityId entity;
	entity.playerId = playerId;
	return entity;
}
