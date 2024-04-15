START /WAIT ../../Plugins/protobuf/protoc.exe -I=./ --cpp_out=./ ./Enum.proto
START /WAIT ../../Plugins/protobuf/protoc.exe -I=./ --cpp_out=./ ./Struct.proto
START /WAIT ../../Plugins/protobuf/protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

IF NOT ERRORLEVEL 0 PAUSE

:: for server
XCOPY Enum.pb.h "../.." /Y
XCOPY Struct.pb.h "../.." /Y
XCOPY Protocol.pb.h "../.." /Y
XCOPY Enum.pb.cc "../.." /Y
XCOPY Struct.pb.cc "../.." /Y
XCOPY Protocol.pb.cc "../.." /Y

:: for client
XCOPY Enum.pb.h "../../../Source/MiniDS/Protocol" /Y
XCOPY Struct.pb.h "../../../Source/MiniDS/Protocol" /Y
XCOPY Protocol.pb.h "../../../Source/MiniDS/Protocol" /Y
XCOPY Enum.pb.cc "../../../Source/MiniDS/Protocol" /Y
XCOPY Struct.pb.cc "../../../Source/MiniDS/Protocol" /Y
XCOPY Protocol.pb.cc "../../../Source/MiniDS/Protocol" /Y

DEL *.pb.h
DEL *.pb.cc

PAUSE
