#include "AccoutHelper.h"
#include "../common/common.h"
#include "../common/commonHelper.h"
#include "dbHelper.h"
#include <boost/date_time.hpp>
#include "playerHelper.h"

void CAccoutHelper::do_register()
{
	CDispatchEventMgr::instance()->registerPlayerAccountHandle("Register", CAccoutHelper::OnRegister);
	CDispatchEventMgr::instance()->registerPlayerAccountHandle("Login", CAccoutHelper::OnLogin);
}

void CAccoutHelper::OnRegister(CPlayerEntityPtr entity, Json::Value json)
{
	CHECK_NULLPTR_RETURN(entity);
	std::string account, accoutName, passWord, phoneNum, macAddr;
	boost::posix_time::ptime now(boost::posix_time::second_clock::local_time());

	CHECK_JSON_PARAM_AND_GET(entity, json, asString(), "account", account, "account can not be null");
	CHECK_JSON_PARAM_AND_GET(entity, json, asString(), "password", passWord, "password can not be null");
	CHECK_JSON_PARAM_AND_GET(entity, json, asString(), "macAddr", macAddr, "macAddr can not be null");
	CHECK_JSON_PARAM_AND_GET(entity, json, asString(), "phoneNum", phoneNum, "phoneNum can not be null");
	CHECK_JSON_PARAM_AND_GET(entity, json, asString(), "userName", accoutName, "userName can not be null");

	//检测帐号是否已存在
	if (CDBHelper::accountExist(account))
	{
		ERROR_MESSAGE_TO_CLIENT(entity, ErrorAccountExist, "accout exist");
	}
	//检测用户名是否已存在
	if (CDBHelper::accountNameExist(accoutName))
	{
		ERROR_MESSAGE_TO_CLIENT(entity, ErrorAccountNameExist, "accout name exist");
	}

	bool isOpenDeviceLock = false;
	if (json.isMember("openDeviceLock"))
	{
		isOpenDeviceLock = json["openDeviceLock"].asBool();
	}
	EUserType userType = EUserNormal;

	if (json.isMember("inviteKey"))
	{
		std::string inviteKey = json["inviteKey"].asString();
		if (false == inviteKey.empty())
		{
			userType = CPlayerHelper::getUserTypeByInviteKey(inviteKey);
		}
	}

	tb_user_info_ptr userInfo = tb_user_info_ptr(new tb_user_info);
	userInfo->set_userid(CDBHelper::getUserId());
	userInfo->set_accout(account);
	userInfo->set_accoutname(accoutName);
	userInfo->set_password(passWord);
	userInfo->set_phonenum(phoneNum);
	userInfo->set_usertype(userType);
	userInfo->set_opendevicelock(isOpenDeviceLock);
	userInfo->set_onlone(false);

	userInfo->set_email(json["email"].asString());
	userInfo->set_registertime(boost::posix_time::to_iso_string(now));
	CDBHelper::registerUser(userInfo);
	entity->messageToClt(ECmdRegister, "");

	Json::Value temp;
	temp["account"] = account;
	temp["password"] = passWord;
	temp["macAddr"] = macAddr;
	CDispatchEventMgr::instance()->onPlayerAccountMessage("Login", entity, temp);
}

void CAccoutHelper::OnLogin(CPlayerEntityPtr entity, Json::Value json)
{
	CHECK_NULLPTR_RETURN(entity); CHECK_NULLPTR_RETURN(entity);
	std::string account, passWord, macAddr;
	CHECK_JSON_PARAM_AND_GET(entity, json, asString(), "account", account, "account can not be null");
	CHECK_JSON_PARAM_AND_GET(entity, json, asString(), "password", passWord, "password can not be null");
	CHECK_JSON_PARAM_AND_GET(entity, json, asString(), "macAddr", macAddr, "macAddr can not be null");

	ErrorCode ec;
	std::string errDesc;
	int userId = 0;
	if (!CDBHelper::checkLoginAccount(account, passWord, macAddr, userId, ec, errDesc))
	{
		entity->errorMessage(ec, errDesc);
		return;
	}
	SEntityId entityId = CCommonHelper::makeEntityId(userId);
	entity->setEntityId(entityId);
	entity->registerModule();

	//认证成功发送通信cookie
	CPlayerHelper::makeCookieForClt(entity);
	CDBHelper::loadPlayerData(entity);
	entity->onLine();
}
