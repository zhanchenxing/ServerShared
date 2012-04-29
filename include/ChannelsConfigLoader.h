#pragma once

#include <memory>
struct ChannelsConfigLoaderPimplData;

class ChannelsConfigLoader
{
private:
	ChannelsConfigLoader( const ChannelsConfigLoader & rhs );

public:
	ChannelsConfigLoader(void);
	~ChannelsConfigLoader(void);

	bool LoadChannelsConfig();

	const char * GetChannelKey( size_t nIndex );
	size_t GetChannelSize( size_t nIndex );

	size_t GetChannelsCount();

private:

	std::auto_ptr< ChannelsConfigLoaderPimplData > m_pPimplData;
};

