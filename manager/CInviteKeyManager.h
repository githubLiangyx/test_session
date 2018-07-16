#pragma once
#include <boost/noncopyable.hpp>
#include "../entity/PlayerEntity.h"

class CInviteKeyManager
	: public boost::noncopyable
{
public:
	static CInviteKeyManager* instance();

	static void initData();

	void addInviteKey(CPlayerEntityPtr entity);

	EUserType getUserTypeByInviteKey(const std::string& inviteKey);

private:
	void addInviteKey(tb_user_info_ptr tb);

private:
	std::map<std::string, tb_user_info_ptr> _inviteKey2PlayerInfo;
};