#include "userInfoModule.h"
#include "../entity/PlayerEntity.h"
#include <boost/date_time.hpp>

CUserInfoModule::CUserInfoModule(std::shared_ptr<CPlayerEntity> entity)
	:CPlayerModule(ETB_USER_INFO)
{
	assert(entity);
	_entity = entity;
}

void CUserInfoModule::onLine()
{

}

void CUserInfoModule::onOffline()
{
	boost::posix_time::ptime now(boost::posix_time::second_clock::local_time());
	_tb_user->set_lastloginouttime(boost::posix_time::to_iso_string(now));
	_tb_user->set_onlone(false);
}

void CUserInfoModule::onLoadData(DbDataBasePtr data)
{
	_tb_user = std::static_pointer_cast<tb_user_info>(data);
	assert(_tb_user);
}

std::string CUserInfoModule::getName()
{
	assert(_tb_user);
	return _tb_user->accoutname();
}
