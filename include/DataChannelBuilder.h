#pragma once

#include <memory>

class ChannelsConfigLoader;
class DataChannel;

class DataChannelBuilder
{


public:
	explicit DataChannelBuilder(void);
	~DataChannelBuilder(void);
	bool LoadConfig();
	bool BuildChannel( DataChannel & r, unsigned nIndex );

private:
	std::auto_ptr<ChannelsConfigLoader> m_apConfigLoader;
};

