#pragma once

#include <memory>

#include <boost/interprocess/mapped_region.hpp>

const static size_t MIN_DATACHANNEL_SIZE = 1024*1024;	// �����ڴ���С��ô���ֽ�
const static size_t MAX_DATA_SIZE = 1024;				// һ��������ô��

struct DataChannelHead{
	unsigned m_nTotalLength;	// �����ڴ���ܳ���

	unsigned m_nReadCount;		// ����һֱ�����ֵ���ϼ�
	unsigned m_nWriteCount;		// д��һֱ�����ֵ���ϼ�

	unsigned m_nReadPos;			// �´δ����λ�ÿ�ʼ��
	unsigned m_nWritePos;		// �´δ����λ�ÿ�ʼд
};

unsigned IncreToDivisibleBy4( unsigned nNumber );

class DataChannel
{
private:
	// ���ÿ������캯��
	DataChannel(const DataChannel & rhs);
public:
	DataChannel(void);
	~DataChannel(void);

	// ��һ��ͨ�����ڶ�ȡ��Ϣ
	bool Open( const char * szUniqueKey, size_t nSize );

protected:

	DataChannelHead * m_pHead;
	char * m_pData;
	std::auto_ptr<boost::interprocess::mapped_region> m_pMappedRegion;	// ��Ҫ��������Ĵ�������֤m_pData�Ŀ���
};

