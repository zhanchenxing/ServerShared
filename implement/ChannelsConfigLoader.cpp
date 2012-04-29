#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>


#include "ChannelsConfigLoader.h"

#include "SimpleIni.h"

using namespace std;

struct ChannelsConfigLoaderPimplData{
	std::vector< std::pair<std::string, size_t> > m_vtChannels;
};

ChannelsConfigLoader::ChannelsConfigLoader(void)
{
}


ChannelsConfigLoader::~ChannelsConfigLoader(void)
{
}

bool ChannelsConfigLoader::LoadChannelsConfig(){
	m_pPimplData.reset( new ChannelsConfigLoaderPimplData  );

	CSimpleIniA ini;
	if( SI_OK != ini.LoadFile("./ChannelsConfig.ini") ){
		std::cerr<< "Load ChannelsConfig.ini failed!"<<std::endl;
		return false;
	}

	unsigned nChannelsCount = atoi(
		ini.GetValue("channels", "channels_count", "")
		);

	if( nChannelsCount == 0 ){
		std::cerr<< "nChannelsCount is 0" <<std::endl;
		return false;
	}

	for( unsigned n = 0; n<nChannelsCount; ++n ){
		stringstream sKey; sKey<<"channel_"<<n<<"_key";
		stringstream sSize; sSize<<"channel_"<<n<<"_size";

		string k = ini.GetValue( "channels", sKey.str().c_str(), "");
		size_t s = atoi( ini.GetValue( "channels", sSize.str().c_str(), "" ) );

		if( k.empty() || s==0 ){
			cerr<<n<<"th channel config error!"<<endl;
			return false;
		}

		m_pPimplData->m_vtChannels.push_back( make_pair( k, s) );
	}

	return true;
}

const char * ChannelsConfigLoader::GetChannelKey( size_t nIndex ){
	if( nIndex>=m_pPimplData->m_vtChannels.size() ){
		return "";
	}

	return m_pPimplData->m_vtChannels[nIndex].first.c_str();
}

size_t ChannelsConfigLoader::GetChannelSize( size_t nIndex ){
	if( nIndex >= m_pPimplData->m_vtChannels.size() ){
		return 0;
	}

	return m_pPimplData->m_vtChannels[nIndex].second;
}

size_t ChannelsConfigLoader::GetChannelsCount(){
	return m_pPimplData->m_vtChannels.size();
}

