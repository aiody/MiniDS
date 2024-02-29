#pragma once

class SendBuffer : public enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(uint32 bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.data(); }
	uint32 WriteSize() { return _writeSize; }
	uint32 Capacity() { return static_cast<uint32>(_buffer.size()); }

	void CopyData(void* data, int32 len);
	void Close(uint32 writeSize);

private:
	vector<BYTE> _buffer;
	uint32 _writeSize = 0;
};
