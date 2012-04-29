#include "DataChannel.h"

#include <iostream>
#include <memory>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/scoped_ptr.hpp>


using namespace boost::interprocess;

DataChannel::DataChannel(void)
{
}

// 增加这个数字，以能够被4整除
unsigned IncreToDivisibleBy4( unsigned nNumber ){
	unsigned nRemainder = nNumber % 4;		// 余数
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

// 打开一个通道用于读取消息
bool DataChannel::Open( const char * szUniqueKey, size_t nSize )
{
	if( nSize < MIN_DATACHANNEL_SIZE ){
		return false;
	}

	// 大小必需是4的整数倍
	nSize = IncreToDivisibleBy4(nSize);

	bool bNeedInitial = false;

	boost::scoped_ptr<shared_memory_object> sp;
	try{
		// 如果已经存在，就不需要进行初始化。
		sp.reset( new shared_memory_object(	open_only//open_or_create                  //open or create
			,szUniqueKey              //name
			,read_write                   //read-write mode
			) );
	}	
	catch ( boost::interprocess::interprocess_exception &  ){

		// 如果还不存在，就创建出来，并且初始化
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

		// 如果是别人创建的，长度必需一到
		if( pHead->m_nTotalLength != nSize ){
			std::cerr<<"长度不一致"<< std::endl;
			return false;
		}

	}

	m_pHead = pHead;
	m_pData = (char*)( pHead+1 );

	// Check it on C:\ProgramData
	return true;
}

