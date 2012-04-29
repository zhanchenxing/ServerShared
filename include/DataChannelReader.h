#pragma once

#include <string>
#include <boost/shared_ptr.hpp>

#include "DataChannel.h"

class DataChannelReader : public DataChannel
{
public:
	DataChannelReader(void);
	~DataChannelReader(void);

	// 读取一段数据
	// 如果暂时没有数据，返回NULL，nSize的值不会改变
	//const char* ReadData( unsigned & nSize );

	boost::shared_ptr<std::string> ReadData(  );
	unsigned GetCurrentMessageSize();
private:
	bool GetReadPosition( unsigned & nSizePos, unsigned & nDataPos );

	std::string m_strReturnedData;
};

