#include "dbHelper.h"
#include "../common/common.h"
#include "../db/db_UserInfoManager.h"

void CDBHelper::registerUser(tb_user_info_ptr userInfo)
{
	CDbUserInfoManagerPtr tb_user_info = getDBMgr<CDbUserInfoManager>(ETB_USER_INFO);
	CHECK_NULLPTR_RETURN(tb_user_info);
	tb_user_info->addRegisterAccount(userInfo);
}

bool CDBHelper::accountNameExist(const std::string & name)
{
	CDbUserInfoManagerPtr tb_user_info = getDBMgr<CDbUserInfoManager>(ETB_USER_INFO);
	assert(tb_user_info);
	CHECK_NULLPTR_RETURN_FALSE(tb_user_info);

	return tb_user_info->accoutNameExist(name);
}

bool CDBHelper::accountExist(const std::string & account)
{
	CDbUserInfoManagerPtr tb_user_info = getDBMgr<CDbUserInfoManager>(ETB_USER_INFO);
	assert(tb_user_info);
	CHECK_NULLPTR_RETURN_FALSE(tb_user_info);

	return tb_user_info->accountExist(account);
}

bool CDBHelper::checkLoginAccount(const std::string & account, const std::string & password, const std::string & macAddr, int & playerId, ErrorCode & ec, std::string & errDesc)
{
	playerId = 0;
	ec = ErrorAccountNotExist;
	errDesc = "account not exist";

	CDbUserInfoManagerPtr tb_user_info = getDBMgr<CDbUserInfoManager>(ETB_USER_INFO);
	CHECK_NULLPTR_RETURN_FALSE(tb_user_info);
	tb_user_info_ptr user_info = tb_user_info->getUserInfoByAccount(account);
	CHECK_NULLPTR_RETURN_FALSE(user_info);

	if (user_info->password() != password)
	{
		ec = ErrorPasswordUnCorrect;
		errDesc = "password uncorrect";
		return false;
	}
	else if (user_info->opendevicelock() && user_info->registeraddr() != macAddr)
	{
		ec = ErrorDeviceLock;
		errDesc = "you have open device lock, please use last time login device";
		return false;
	}
	else
	{
		playerId = user_info->userid();
		return true;
	}
	return false;
}

void CDBHelper::loadPlayerData(CPlayerEntityPtr entity)
{
	CHECK_NULLPTR_RETURN(entity);
	std::map<ETB_MODULE, DbDataBasePtr> data = CDBbaseManager::instance()->loadPlayerData(entity->getEntityId());
	entity->onLoadData(data);
}

int CDBHelper::getUserId()
{
	CDbUserInfoManagerPtr tb_user_info = getDBMgr<CDbUserInfoManager>(ETB_USER_INFO);
	assert(tb_user_info);
	return tb_user_info->getUserId();
}

