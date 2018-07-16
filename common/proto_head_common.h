#pragma once
#include "../proto_cpp/db.pb.h"
#include <google/protobuf/message.h>

//自定义结构要在这里加上智能指针定义 
typedef google::protobuf::Message DbDataBase;
typedef std::shared_ptr<DbDataBase> DbDataBasePtr;
typedef std::shared_ptr<tb_user_info> tb_user_info_ptr;
typedef std::shared_ptr<tb_user_info_file> tb_user_info_file_ptr;