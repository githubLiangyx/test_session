#pragma once
#include "playerModule.h"
#include "../common/proto_head_common.h"

class CPlayerEntity;
class CUserInfoModule
	: public CPlayerModule
{
public:
	CUserInfoModule(std::shared_ptr<CPlayerEntity> entity);

public:
	void onLine();

	void onOffline();

	void onLoadData(DbDataBasePtr data);

	std::string getName();

private:

	tb_user_info_ptr _tb_user;
	std::shared_ptr<CPlayerEntity> _entity;
};
typedef std::shared_ptr<CUserInfoModule> CUserInfoModulePtr;