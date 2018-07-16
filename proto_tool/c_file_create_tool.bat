echo off
set DEST_DIR=..\proto_cpp

echo remove old proto c++ proto_file ...
del  ..\proto_cpp\* /Q
echo remove old proto c++ proto_file success ...

echo target dir is %DEST_DIR%

echo begin create c++ proto_file ...
protoc.exe db.proto --cpp_out=%DEST_DIR%
echo create c++ proto_file success ...

echo on