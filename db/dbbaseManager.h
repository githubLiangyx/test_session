#pragma once
#include <string>
#include <memory>
#include <vector>
#include <json/json.h>
#include "../common/proto_head_common.h"
#include <leveldb/db.h>
#include "../common/common.h"

class CDBbase
{
public:
	virtual std::string getSavePath() = 0;

	virtual void loadData(DbDataBasePtr) = 0;

	virtual DbDataBasePtr getDBData() = 0;

	virtual DbDataBasePtr loadPlayerData(SEntityId entityId) = 0;
};
typedef std::shared_ptr<CDBbase> CDBbasePtr;

//后续改成配置
static const std::string db_name = "./db/dbfile-v1";

class CDBbaseManager
{
public:
	static CDBbaseManager* instance();

	static void start();

	CDBbasePtr getDBManager(ETB_MODULE db);

public:
	void registerDB();

public:
	void loadDBData();

	std::map<ETB_MODULE, DbDataBasePtr> loadPlayerData(SEntityId entityId);

	void saveDBData();

	void saveDBData(ETB_MODULE tb);

	~CDBbaseManager();

private:
	CDBbaseManager();
	CDBbaseManager(const CDBbaseManager&) = delete;
	CDBbaseManager& operator=(const CDBbaseManager&) = delete;
	DbDataBasePtr getFileData(const std::string& filePath);

private:

	std::map<ETB_MODULE, CDBbasePtr> _dbId2DbInstance;

	leveldb::DB *_db;

};