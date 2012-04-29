#include <assert.h>

#include "DataChannelReader.h"


DataChannelReader::DataChannelReader(void)
{
}


DataChannelReader::~DataChannelReader(void)
{
}

bool DataChannelReader::GetReadPosition( unsigned & nSizePos, unsigned & nDataPos ){
	// �����ȡ����д������ͬ��֤��û���µ����ݵ��
	if( m_pHead->m_nReadCount == m_pHead->m_nWriteCount ){
		return false;
	}

	// ���ݶε��ܵĳ���
	const unsigned nDataTotalLength = m_pHead->m_nTotalLength - sizeof(*m_pHead);

	// �����ǰ�Ķ�ȡλ�õ���β�����ܷ���һ��unsigned����˵����ǰλ�þ��Ǵ�Ŵ�С��
	// ��������ڿ�ͷ��
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
		// �ص���ͷȥ��
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

	// ��ȡλ��ָ����һ��λ��
	unsigned nSize =  *(unsigned*) ( m_pData + nSizePos );
	if( nSize > MAX_DATA_SIZE ){
		assert(false);
	}

	boost::shared_ptr<std::string> ret ( new std::string );
	ret->assign(m_pData + nDataPos, nSize);

	//m_strReturnedData.assign(m_pData + nDataPos, nSize );

	// ��ȡλ����Զ��4���������ĵ�ַ��
	m_pHead->m_nReadPos = IncreToDivisibleBy4(nDataPos + nSize);

	// ��ȡ������
	++m_pHead->m_nReadCount;
	return ret;
}

