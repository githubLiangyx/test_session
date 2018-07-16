#include "CInviteKeyManager.h"
#include "../db/db_UserInfoManager.h"
#include "../helper/dbHelper.h"

CInviteKeyManager* CInviteKeyManager::instance()
{
	static CInviteKeyManager singleton;
	return &singleton;
}

void CInviteKeyManager::initData()
{
	CInviteKeyManager* singleton = CInviteKeyManager::instance();

	CDbUserInfoManagerPtr user = CDBHelper::getDBMgr<CDbUserInfoManager>(ETB_USER_INFO);
	CHECK_NULLPTR_RETURN(user);
	const std::map<int, tb_user_info_ptr>& tb_users = user->getUserInfoMap();
	for (std::map<int, tb_user_info_ptr>::const_iterator it = tb_users.begin(); it != tb_users.end(); ++it)
	{
		singleton->addInviteKey(it->second);
	}
}

void CInviteKeyManager::addInviteKey(CPlayerEntityPtr entity)
{
	CHECK_NULLPTR_RETURN(entity);
	//tb_user_info_ptr tb = entity->getTbUserInfo();
	//addInviteKey(tb);
}

EUserType CInviteKeyManager::getUserTypeByInviteKey(const std::string & inviteKey)
{
	if (_inviteKey2PlayerInfo.count(inviteKey) > 0)
	{
		int userType = _inviteKey2PlayerInfo[inviteKey]->usertype();
		switch (userType)
		{
		case EUserManager:
		case EUserRoot:
			return EUserManager;
		}
	}
	return EUserNormal;
}

void CInviteKeyManager::addInviteKey(tb_user_info_ptr tb)
{
	CHECK_NULLPTR_RETURN(tb);
	_inviteKey2PlayerInfo[tb->selfinvitekey()] = tb;
}
