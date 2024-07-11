#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(uint32 bufferSize)
{
	_buffer.resize(bufferSize);
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::CopyData(void* data, int32 len)
{
	ASSERT_CRASH(Capacity() >= len);
	::memcpy(Buffer(), data, len);
	_writeSize = len;
}

void SendBuffer::Close(uint32 writeSize)
{
	ASSERT_CRASH(Capacity() >= writeSize);
	_writeSize = writeSize;
}
