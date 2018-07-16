#include "dbbaseManager.h"
#include <fstream>
#include <iostream>
#include "db_UserInfoManager.h"
#include "../common/common.h"
using namespace leveldb;


CDBbaseManager * CDBbaseManager::instance()
{
	static CDBbaseManager singleton;
	return &singleton;
}

void CDBbaseManager::start()
{
	CDBbaseManager* singleton = instance();
	singleton->registerDB();
	singleton->loadDBData();
}

CDBbasePtr CDBbaseManager::getDBManager(ETB_MODULE db)
{
	if (_dbId2DbInstance.count(db) > 0)
	{
		return _dbId2DbInstance[db];
	}
	return NULL;
}

void CDBbaseManager::registerDB()
{
	_dbId2DbInstance[ETB_USER_INFO] = CDBbasePtr(new CDbUserInfoManager);

}

DbDataBasePtr CDBbaseManager::getFileData(const std::string & protokey)
{
	DbDataBasePtr tb_data(NULL);
	std::string value;
	leveldb::Status status = _db->Get(leveldb::ReadOptions(), protokey, &value);
	if (status.ok())
	{
		const char* data = value.data();
		std::string ptemp(data, 2);
		data += 2;
		int protolen = atoi(ptemp.data());
		ptemp = std::string(data, protolen);
		data += protolen;
		google::protobuf::MessageFactory* factory = google::protobuf::MessageFactory::generated_factory();
		const google::protobuf::DescriptorPool* pool = google::protobuf::DescriptorPool::generated_pool();
		const google::protobuf::Descriptor* desc = pool->FindMessageTypeByName(ptemp);
		if (desc)
		{
			const google::protobuf::Message* message = factory->GetPrototype(desc);
			if (message)
			{
				tb_data = DbDataBasePtr(message->New());
				bool ret = tb_data->ParseFromArray(data, value.length() - 2 - protolen);
				if (!ret)
				{
					tb_data = DbDataBasePtr(NULL);
				}
			}
		}

	}
	return tb_data;
}

void CDBbaseManager::loadDBData()
{
	for (std::map<ETB_MODULE, CDBbasePtr>::const_iterator it = _dbId2DbInstance.begin(); it != _dbId2DbInstance.end(); ++it)
	{
		std::string protokey = it->second->getSavePath();
		tb_user_info_file_ptr tb_data = std::static_pointer_cast<tb_user_info_file>(getFileData(protokey));
		if (!tb_data || tb_data->uinfo_size() == 0)
		{
			return;
		}
		it->second->loadData(tb_data);
	}
}

std::map<ETB_MODULE, DbDataBasePtr> CDBbaseManager::loadPlayerData(SEntityId entityId)
{
	std::map<ETB_MODULE, DbDataBasePtr> ret;
	for (std::map<ETB_MODULE, CDBbasePtr>::const_iterator it = _dbId2DbInstance.begin(); it != _dbId2DbInstance.end(); ++it)
	{
		if (it->second)
		{
			ret[it->first] = it->second->loadPlayerData(entityId);
		}
	}
	return ret;
}

void CDBbaseManager::saveDBData()
{

	for (std::map<ETB_MODULE, CDBbasePtr>::const_iterator it = _dbId2DbInstance.begin(); it != _dbId2DbInstance.end(); ++it)
	{
		if (it->second)
		{
			saveDBData(it->first);
		}
	}
}

void CDBbaseManager::saveDBData(ETB_MODULE tb)
{
	std::string real_value, protokey;
	{
		CDBbasePtr ptr = _dbId2DbInstance[tb];
		DbDataBasePtr tb_data = ptr->getDBData();
		protokey = ptr->getSavePath();
		std::string value = tb_data->SerializeAsString();
		
		std::string prototype = tb_data->GetTypeName();
		char data[3] = "0";
		sprintf(data, "%.2d", prototype.length());
		real_value = std::string(data, 2);
		real_value.append(prototype);
		real_value.append(std::move(value));
		
	}

	_db->Put(leveldb::WriteOptions(), protokey, real_value);
}

CDBbaseManager::~CDBbaseManager()
{
	delete _db;
}

CDBbaseManager::CDBbaseManager()
{
	leveldb::Options options;
	options.create_if_missing = true;
	_db = NULL;
	leveldb::Status status = leveldb::DB::Open(options, db_name, &_db);
	if (!status.ok())
	{
		assert(false);
		// _error happend
	}
}
