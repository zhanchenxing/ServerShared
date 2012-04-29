#pragma once

#include <memory>

#include <boost/interprocess/mapped_region.hpp>

const static size_t MIN_DATACHANNEL_SIZE = 1024*1024;	// 共享内存最小这么多字节
const static size_t MAX_DATA_SIZE = 1024;				// 一次最多存这么多

struct DataChannelHead{
	unsigned m_nTotalLength;	// 共享内存的总长度

	unsigned m_nReadCount;		// 读者一直把这个值往上加
	unsigned m_nWriteCount;		// 写者一直反这个值往上加

	unsigned m_nReadPos;			// 下次从这个位置开始读
	unsigned m_nWritePos;		// 下次从这个位置开始写
};

unsigned IncreToDivisibleBy4( unsigned nNumber );

class DataChannel
{
private:
	// 禁用拷贝构造函数
	DataChannel(const DataChannel & rhs);
public:
	DataChannel(void);
	~DataChannel(void);

	// 打开一个通道用于读取消息
	bool Open( const char * szUniqueKey, size_t nSize );

protected:

	DataChannelHead * m_pHead;
	char * m_pData;
	std::auto_ptr<boost::interprocess::mapped_region> m_pMappedRegion;	// 需要这个变量的存在来保证m_pData的可用
};

