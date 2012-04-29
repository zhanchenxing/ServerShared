#pragma once

#include "DataChannel.h"

class DataChannelWriter : public DataChannel
{
public:
	DataChannelWriter(void);
	~DataChannelWriter(void);

	// 读取一段数据
	// 写入一段数据
	// 如果已经满了，无法再写入，就返回false
	bool WriteData( const char * pData, unsigned nSize );

private:
	bool GetWritePosition( unsigned & nSizePos, unsigned & nDataPos, unsigned nDataLength );
};

