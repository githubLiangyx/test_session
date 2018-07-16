#pragma once
#include "dbbaseManager.h"
#include "../common/proto_head_common.h"

const static std::string TA_USER_INFO = "tb_user_info";

class CDbUserInfoManager
	: public CDBbase
{
public:
	CDbUserInfoManager() :_userIdMax(0) {}

	DbDataBasePtr getDBData();

	void loadData(DbDataBasePtr data);

	void addRegisterAccount(tb_user_info_ptr);

	std::string getSavePath();

	bool accoutNameExist(const std::string& name);

	bool accountExist(const std::string& account);

	const std::map<int, tb_user_info_ptr>& getUserInfoMap() { return _playerId2UserInfo; }

	tb_user_info_ptr getUserInfoByAccount(const std::string& account);

	DbDataBasePtr loadPlayerData(SEntityId entityId);

	int getUserId();

private:
	void saveDBData();

private:
	std::map<int, tb_user_info_ptr> _playerId2UserInfo;
	std::map<std::string, tb_user_info_ptr> _account2UserInfo;
	std::map<std::string, tb_user_info_ptr> _userName2UserInfo;

	int _userIdMax;
};
typedef std::shared_ptr<CDbUserInfoManager> CDbUserInfoManagerPtr;