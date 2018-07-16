#pragma once
#include <memory>

#define PROTO_HEAD_LEN 4
#define PACK_FLAG_LEN 2
#define SOCK_DATA_MAX_LEN 40*1024 //each transmission data Unit max length ÿ�����䵥Ԫ�Ĵ�С���� 40kb
#define SOCK_PACK_MAX_LEN 1000*1024 //each data pack max length ÿ�����Ĵ�С���� 10M
#define SOCK_DATA_NOT_INCLUDE_HEAD 2000

#define CHECK_NULLPTR_RETURN(ptr) do{if(!ptr) return;} while(false);
#define CHECK_NULLPTR_RETURN_FALSE(ptr) do{if(!ptr) return false;} while(false);

#define ERROR_MESSAGE_TO_CLIENT(entity, errCode, errDesc) entity->errorMessage(errCode, errDesc);\
														  return;

#define CHECK_JSON_PARAM_AND_GET(entity, json, jsonfunc, fieldName, var, errDesc) \
					if(json.isMember(fieldName))	var = json[fieldName].##jsonfunc;\
					else {ERROR_MESSAGE_TO_CLIENT(entity, ErrorInvalidParam, errDesc)};

#define GET_JSON_PARAM_OR_SET_DEFAULT(json, jsonfunc, fieldName, var, defaultValue) \
					if(json.isMember(fieldName))	var = json[fieldName.##jsonfunc;\
					else var = defaultValue;


enum EPermissionType
{
	EPermissionVisitor = 1,
	EPermissionNormal  = 2,
};

enum EEntityComponentType
{
	EEntityPermissionComponent = 1,
};

enum EActionType
{
};

enum ErrorCode
{
	ErrorNot = 0,
	ErrorInvalidParam = 1,
	ErrorNotLogin = 2,
	ErrorUnkowExp = 3,
	ErrorMethodNotFound = 4,
	ErrorInValidCookie = 5,
	ErrorPermissionDenied = 6, //Ȩ�޲���
	
	ErrorAccountNameExist = 10,
	ErrorAccountExist = 11,
	ErrorAccountNotExist = 12,
	ErrorPasswordUnCorrect = 13,
	ErrorDeviceLock = 14,
	ErrorEntityNotOnLine = 15,	//��Ҳ�����
	ErrorTargetNotExist = 16,    //Ŀ�겻����
};

enum EUserType
{
	EUserNormal = 1,
	EUserManager = 5,
	EUserRoot = 99,
};

enum ECmdType
{
	ECmdError = 1,
	ECmdRegister = 2,
	ECmdSetCookie = 3,
	ECmdMsg = 4,
};

enum ETB_MODULE
{
	ETB_USER_INFO = 1,
};

enum ESessionType
{
	ESessionClt = 0,
};

enum EMsgType
{
	EMsgChat = 0, //˽�� 
	EMsgGroupChat = 1, //Ⱥ��
	EMsgSystem  = 2,   //ϵͳ֪ͨ
	EMsChatAllOnLine = 10,  //��ȫ�����ߵ��˷�����Ϣ ��Ҫ����Ա������Ȩ��
};

struct STransProtocol
{
	int need_total_data;
	char u_data[SOCK_DATA_MAX_LEN];
};
typedef std::shared_ptr<STransProtocol> STransProtocolPtr;

struct SEntityId
{
	int playerId;

	bool operator < (const SEntityId& other) const
	{
		return playerId < other.playerId;
	}
};

class CObject
{};
typedef std::shared_ptr<CObject> CObjectPtr;
