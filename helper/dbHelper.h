#pragma once
#include "../common/proto_head_common.h"
#include "../entity/PlayerEntity.h"
#include "../db/dbbaseManager.h"

class CDBHelper
{
public:
	static void registerUser(tb_user_info_ptr userInfo);

	static bool accountNameExist(const std::string& name);

	static bool accountExist(const std::string& account);

	static bool checkLoginAccount(const std::string& account, const std::string& password, const std::string& macAddr, int& playerId, ErrorCode& ec, std::string& errDesc);

	static void loadPlayerData(CPlayerEntityPtr entity);

	static int getUserId();

	template <typename T>
	static std::shared_ptr<T> getDBMgr(ETB_MODULE tb);
};

template<typename T>
inline std::shared_ptr<T> CDBHelper::getDBMgr(ETB_MODULE tb)
{
	std::shared_ptr<T> mgr = std::static_pointer_cast<T>(CDBbaseManager::instance()->getDBManager(tb));
	return mgr;
}
