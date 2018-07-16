#pragma once
#include <boost/noncopyable.hpp>


struct SEntityId;

class CCommonHelper
{
public:
	static SEntityId makeEntityId(const int playerId);
};