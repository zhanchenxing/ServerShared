#include "DataChannel.h"

#include <iostream>
#include <memory>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/scoped_ptr.hpp>


using namespace boost::interprocess;

DataChannel::DataChannel(void)
{
}

// ����������֣����ܹ���4����
unsigned IncreToDivisibleBy4( unsigned nNumber ){
	unsigned nRemainder = nNumber % 4;		// ����
	if( nRemainder ){
		return nNumber + ( 4 - nRemainder );
	}
	else {
		return nNumber;
	}
}


DataChannel::~DataChannel(void)
{
}

// ��һ��ͨ�����ڶ�ȡ��Ϣ
bool DataChannel::Open( const char * szUniqueKey, size_t nSize )
{
	if( nSize < MIN_DATACHANNEL_SIZE ){
		return false;
	}

	// ��С������4��������
	nSize = IncreToDivisibleBy4(nSize);

	bool bNeedInitial = false;

	boost::scoped_ptr<shared_memory_object> sp;
	try{
		// ����Ѿ����ڣ��Ͳ���Ҫ���г�ʼ����
		sp.reset( new shared_memory_object(	open_only//open_or_create                  //open or create
			,szUniqueKey              //name
			,read_write                   //read-write mode
			) );
	}	
	catch ( boost::interprocess::interprocess_exception &  ){

		// ����������ڣ��ʹ������������ҳ�ʼ��
		sp.reset( new shared_memory_object(	open_or_create                  //open or create
			,szUniqueKey              //name
			,read_write                   //read-write mode
			) );
		
		bNeedInitial = true;

		sp->truncate(nSize);
	}

	shared_memory_object & shm_obj = *sp;

	//Map the second half of the memory
	m_pMappedRegion.reset ( new mapped_region
	( shm_obj                      //Memory-mappable object
	, read_write               //Access mode
	, 0                //Offset from the beginning of shm
	));

	mapped_region & region = * m_pMappedRegion;
	//Get the address of the region
	void * pData = region.get_address();

	//Get the size of the region
	std::size_t nRealSize = region.get_size();


	DataChannelHead * pHead = (DataChannelHead *)pData;
	if( bNeedInitial ){
		pHead->m_nTotalLength = nSize;
		pHead->m_nReadCount = 0;
		pHead->m_nWriteCount = 0;
		pHead->m_nReadPos = 0;
		pHead->m_nWritePos = 0;
	}
	else {

		// ����Ǳ��˴����ģ����ȱ���һ��
		if( pHead->m_nTotalLength != nSize ){
			std::cerr<<"���Ȳ�һ��"<< std::endl;
			return false;
		}

	}

	m_pHead = pHead;
	m_pData = (char*)( pHead+1 );

	// Check it on C:\ProgramData
	return true;
}

