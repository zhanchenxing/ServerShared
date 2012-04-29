
#include <string>

#include "DataChannelBuilder.h"
#include "DataChannelReader.h"
#include "DataChannelWriter.h"
#include "ChannelsConfigLoader.h"

using namespace std;

DataChannelBuilder::DataChannelBuilder(void)
{
	m_apConfigLoader.reset( new ChannelsConfigLoader() );
}

bool DataChannelBuilder::LoadConfig(){
	return m_apConfigLoader->LoadChannelsConfig();
}


DataChannelBuilder::~DataChannelBuilder(void)
{
}

bool DataChannelBuilder::BuildChannel( DataChannel & r, unsigned nIndex ){
	std::string sKey = m_apConfigLoader->GetChannelKey(nIndex);
	if( sKey.empty() ){
		return false;
	}

	sKey += "_DataChannelBuilder";

	size_t nSize = m_apConfigLoader->GetChannelSize(nIndex);
	if( nSize < MIN_DATACHANNEL_SIZE ){
		return false;
	}

	return r.Open( sKey.c_str(), nSize );
}

