#include "permissionComponent.h"

CPermissionComponent::CPermissionComponent()
{
	_permission = EPermissionVisitor;
}

bool CPermissionComponent::checkPermission(EActionType action, ErrorCode& ec, std::string& errDesc)
{
	return true;
}
