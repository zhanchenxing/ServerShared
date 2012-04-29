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

		// ���λ�õ�����������ŵ�4������λ���ϡ�
		if( nSizePos % 4 ){
			assert(false);
		}
	}
	else{
		// �ص�ԭ�㡣
		nSizePos = 0;
	}

	// ���������ݵ�λ��
	nDataPos = nSizePos + sizeof(unsigned);
	
	if( nDataPos + nDataLength > nDataTotalLength )	{
		// Խ���ˡ���ͷ��ʼ
		nDataPos = 0;
	}

	unsigned nBegin = nSizePos;
	unsigned nEnd = nDataPos + nDataLength;
	//if( nEnd % 4 ){
	//	nEnd += ( 4 - nEnd %4 );
	//}


	// �����ʼλ�ú͵�ǰ��ȡ��λ��һ������ôֻ�е�û���κ����ݿɶ���ʱ�򣬲��ǿ�д�ġ�
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
	// ����Ƿ��ܹ�д
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


