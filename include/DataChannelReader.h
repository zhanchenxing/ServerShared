#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

#include "DataChannel.h"

class DataChannelReader : public DataChannel
{
public:
	DataChannelReader(void);
	~DataChannelReader(void);

	// ��ȡһ������
	// �����ʱû�����ݣ�����NULL��nSize��ֵ����ı�
	//const char* ReadData( unsigned & nSize );

	boost::shared_ptr<std::string> ReadData(  );
	unsigned GetCurrentMessageSize();
private:
	bool GetReadPosition( unsigned & nSizePos, unsigned & nDataPos );

	std::string m_strReturnedData;
};

