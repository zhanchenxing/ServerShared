#include <assert.h>

#include "DataChannelReader.h"


DataChannelReader::DataChannelReader(void)
{
}


DataChannelReader::~DataChannelReader(void)
{
}

bool DataChannelReader::GetReadPosition( unsigned & nSizePos, unsigned & nDataPos ){
	// 如果读取数和写入数相同，证明没有新的数据到达。
	if( m_pHead->m_nReadCount == m_pHead->m_nWriteCount ){
		return false;
	}

	// 数据段的总的长度
	const unsigned nDataTotalLength = m_pHead->m_nTotalLength - sizeof(*m_pHead);

	// 如果当前的读取位置到结尾，还能放下一个unsigned，就说明当前位置就是存放大小的
	// 否则就是在开头处
	if( m_pHead->m_nReadPos + sizeof(unsigned) <= nDataTotalLength ){
		nSizePos = m_pHead->m_nReadPos;

		if( nSizePos % 4 ){
			assert(false);
		}
	}
	else{
		nSizePos = 0;
	}

	// 
	unsigned nDataSize =  *(unsigned*) ( m_pData + nSizePos );
	nDataPos = nSizePos + sizeof(unsigned);
	if( nDataPos + nDataSize > nDataTotalLength ){ 
		// 回到开头去读
		nDataPos = 0;
	}

	return true;
}

unsigned DataChannelReader::GetCurrentMessageSize()	{
	unsigned nSizePos = 0;
	unsigned nDataPos = 0;
	if( !GetReadPosition(nSizePos, nDataPos) ){
		return 0;
	}

	return  *(unsigned*) ( m_pData + nSizePos );
}


boost::shared_ptr<std::string> DataChannelReader::ReadData( ){

	unsigned nSizePos = 0;
	unsigned nDataPos = 0;
	if( !GetReadPosition(nSizePos, nDataPos) ){
		return boost::shared_ptr<std::string>();
	}

	// 读取位置指向下一个位置
	unsigned nSize =  *(unsigned*) ( m_pData + nSizePos );
	if( nSize > MAX_DATA_SIZE ){
		assert(false);
	}

	boost::shared_ptr<std::string> ret ( new std::string );
	ret->assign(m_pData + nDataPos, nSize);

	//m_strReturnedData.assign(m_pData + nDataPos, nSize );

	// 读取位置永远在4的整数倍的地址上
	m_pHead->m_nReadPos = IncreToDivisibleBy4(nDataPos + nSize);

	// 读取数递增
	++m_pHead->m_nReadCount;
	return ret;
}

