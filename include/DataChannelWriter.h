#pragma once

#include "DataChannel.h"

class DataChannelWriter : public DataChannel
{
public:
	DataChannelWriter(void);
	~DataChannelWriter(void);

	// ��ȡһ������
	// д��һ������
	// ����Ѿ����ˣ��޷���д�룬�ͷ���false
	bool WriteData( const char * pData, unsigned nSize );

private:
	bool GetWritePosition( unsigned & nSizePos, unsigned & nDataPos, unsigned nDataLength );
};

