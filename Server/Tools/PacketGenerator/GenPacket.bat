START /WAIT ../../Plugins/protobuf/protoc.exe -I=./ --cpp_out=./ ./Enum.proto
START /WAIT ../../Plugins/protobuf/protoc.exe -I=./ --cpp_out=./ ./Struct.proto
START /WAIT ../../Plugins/protobuf/protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

IF NOT ERRORLEVEL 0 PAUSE

:: for server
XCOPY Enum.pb.h "../../Codes/Protocol" /Y
XCOPY Struct.pb.h "../../Codes/Protocol" /Y
XCOPY Protocol.pb.h "../../Codes/Protocol" /Y
XCOPY Enum.pb.cc "../../Codes/Protocol" /Y
XCOPY Struct.pb.cc "../../Codes/Protocol" /Y
XCOPY Protocol.pb.cc "../../Codes/Protocol" /Y

:: for client
XCOPY Enum.pb.h "../../../Source/MiniDS/Network/Protocol" /Y
XCOPY Struct.pb.h "../../../Source/MiniDS/Network/Protocol" /Y
XCOPY Protocol.pb.h "../../../Source/MiniDS/Network/Protocol" /Y
XCOPY Enum.pb.cc "../../../Source/MiniDS/Network/Protocol" /Y
XCOPY Struct.pb.cc "../../../Source/MiniDS/Network/Protocol" /Y
XCOPY Protocol.pb.cc "../../../Source/MiniDS/Network/Protocol" /Y

DEL *.pb.h
DEL *.pb.cc

PAUSE
