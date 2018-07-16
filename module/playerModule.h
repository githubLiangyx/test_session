#pragma once
#include "../common/common.h"
#include <google/protobuf/message.h>

class CPlayerModule
{
public:
	CPlayerModule(ETB_MODULE tb);

	ETB_MODULE getModuleType() { return _moduleType; }
public:

	virtual void onLoadData(std::shared_ptr<google::protobuf::Message>) = 0;

	virtual void onLine() = 0;

	virtual void onOffline() = 0;

	virtual ~CPlayerModule() {}

private:
	ETB_MODULE _moduleType;
};
typedef std::shared_ptr<CPlayerModule> CPlayerModulePtr;