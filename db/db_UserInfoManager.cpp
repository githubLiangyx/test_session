#include "db_UserInfoManager.h"
#include "../common/common.h"

DbDataBasePtr CDbUserInfoManager::getDBData()
{
	tb_user_info_file_ptr tb_data = tb_user_info_file_ptr(new tb_user_info_file);
	for (std::map<int, tb_user_info_ptr>::const_iterator it = _playerId2UserInfo.begin(); it != _playerId2UserInfo.end(); ++it)
	{
		*tb_data->add_uinfo() = *(it->second);
	}
	return tb_data;
}

void CDbUserInfoManager::loadData(DbDataBasePtr data)
{
	tb_user_info_file_ptr tb_data = std::static_pointer_cast<tb_user_info_file>(data);
	CHECK_NULLPTR_RETURN(tb_data);
	for (size_t i = 0; i < tb_data->uinfo_size(); ++i)
	{
		tb_user_info_ptr info = tb_user_info_ptr(new tb_user_info(tb_data->uinfo(i)));
		if (_userIdMax < info->userid())
		{
			_userIdMax = info->userid();
		}
		_playerId2UserInfo[info->userid()] = info;
		_userName2UserInfo[info->accoutname()] = info;
		_account2UserInfo[info->accout()] = info;
	}
}

void CDbUserInfoManager::addRegisterAccount(tb_user_info_ptr info)
{
	assert(info);
	_playerId2UserInfo[info->userid()] = info;
	_userName2UserInfo[info->accoutname()] = info;
	_account2UserInfo[info->accout()] = info;
	saveDBData();
}

std::string CDbUserInfoManager::getSavePath()
{
	return TA_USER_INFO;
}

bool CDbUserInfoManager::accoutNameExist(const std::string & name)
{
	return _userName2UserInfo.count(name) > 0;
}

bool CDbUserInfoManager::accountExist(const std::string & account)
{
	return _account2UserInfo.count(account) > 0;
}

tb_user_info_ptr CDbUserInfoManager::getUserInfoByAccount(const std::string & account)
{
	if (_account2UserInfo.count(account) > 0)
	{
		return _account2UserInfo[account];
	}
	return NULL;
}

DbDataBasePtr CDbUserInfoManager::loadPlayerData(SEntityId entityId)
{
	if (_playerId2UserInfo.count(entityId.playerId) > 0)
	{
		return _playerId2UserInfo[entityId.playerId];
	}
	return NULL;
}

int CDbUserInfoManager::getUserId()
{
	if (_userIdMax == INT_MAX)
	{
		_userIdMax = 0;
	}
	return ++_userIdMax;
}

void CDbUserInfoManager::saveDBData()
{
	CDBbaseManager::instance()->saveDBData(ETB_USER_INFO);
}
