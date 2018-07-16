#pragma once
#include "../common/common.h"
#include <string>

class CPermissionComponent
	: public CObject
{
public:
	CPermissionComponent();

	bool checkPermission(EActionType action, ErrorCode& ec, std::string& errDesc);

	void loadData() {}

	EPermissionType getPermission() { return _permission; }

private:
	EPermissionType _permission;
};
typedef std::shared_ptr<CPermissionComponent> CPermissionComponentPtr;