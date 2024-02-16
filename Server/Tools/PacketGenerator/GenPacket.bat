START /WAIT ../../Plugins/protobuf/protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

IF NOT ERRORLEVEL 0 PAUSE

XCOPY Protocol.pb.h "../.." /Y
XCOPY Protocol.pb.cc "../.." /Y

XCOPY Protocol.pb.h "../../../Source/MiniDS/Protocol" /Y
XCOPY Protocol.pb.cc "../../../Source/MiniDS/Protocol" /Y

DEL *.pb.h
DEL *.pb.cc

PAUSE
