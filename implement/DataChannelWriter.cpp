#include "DataChannelWriter.h"

#include <iostream>

using namespace std;


DataChannelWriter::DataChannelWriter(void)
{
}


DataChannelWriter::~DataChannelWriter(void)
{
}

bool DataChannelWriter::GetWritePosition( unsigned & nSizePos, unsigned & nDataPos, unsigned nDataLength ){
	const unsigned nDataTotalLength = m_pHead->m_nTotalLength - sizeof(*m_pHead);

	const unsigned nCurReadPos = m_pHead->m_nReadPos;
	const unsigned nCurReadCount = m_pHead->m_nReadCount;
	const unsigned nCurWriteCount = m_pHead->m_nWriteCount;

	if( m_pHead->m_nWritePos + sizeof(unsigned) <= nDataTotalLength ){
		nSizePos = m_pHead->m_nWritePos;

		// 存放位置的整数，必需放到4的整数位置上。
		if( nSizePos % 4 ){
			assert(false);
		}
	}
	else{
		// 回到原点。
		nSizePos = 0;
	}

	// 后面是数据的位置
	nDataPos = nSizePos + sizeof(unsigned);
	
	if( nDataPos + nDataLength > nDataTotalLength )	{
		// 越界了。从头开始
		nDataPos = 0;
	}

	unsigned nBegin = nSizePos;
	unsigned nEnd = nDataPos + nDataLength;
	//if( nEnd % 4 ){
	//	nEnd += ( 4 - nEnd %4 );
	//}


	// 如果起始位置和当前读取的位置一样，那么只有当没有任何数据可读的时候，才是可写的。
	if( nBegin==nCurReadPos && nCurReadCount!=nCurWriteCount  ){
		return false;
	}

	if( nBegin < nEnd ){
		if( nBegin < nCurReadPos && nEnd > nCurReadPos ){
			return false;
		}
	}

	if( nBegin > nEnd ){
		if( nBegin < nCurReadPos || nEnd > nCurReadPos ){
			return false;
		}
	}

	return true;
}

bool DataChannelWriter::WriteData( const char * pData, unsigned nSize ){
	// 检查是否能够写
	unsigned nSizePos = 0;
	unsigned nDataPos = 0;
	if( ! GetWritePosition(nSizePos, nDataPos, nSize) )
	{
		return false;
	}

	memcpy( m_pData + nSizePos, &nSize, sizeof(nSize) );
	memcpy( m_pData + nDataPos, pData, nSize );
	m_pHead->m_nWritePos =  IncreToDivisibleBy4( nDataPos + nSize );
	++m_pHead->m_nWriteCount;
	return true;
}


