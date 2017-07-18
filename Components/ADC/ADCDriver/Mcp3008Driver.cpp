#include <string>

#include "Mcp3008Driver.h"

const std::string DefaultDriverName = "Mcp3008Driver";

ADCDriver::Mcp3008Driver::Mcp3008Driver()
{
	_driverName = DefaultDriverName;
	_config = AudioDriverConfig::ADCSingleEndedCh0;
}

ADCDriver::Mcp3008Driver::Mcp3008Driver(std::string name)
{
	_driverName = name;
	_config = AudioDriverConfig::ADCSingleEndedCh0;
}

ADCDriver::Mcp3008Driver::~Mcp3008Driver()
{
}

ADCDriver::Result ADCDriver::Mcp3008Driver::GetDriverName(std::string& name)
{
	name = _driverName;
	return ADCDriver::OK;
}

ADCDriver::Result ADCDriver::Mcp3008Driver::ConfigureAudioDriver(AudioDriverConfig config)
{
	_config = config;
	return ADCDriver::OK;
}

ADCDriver::AudioDriverConfig ADCDriver::Mcp3008Driver::GetAudioDriverConfiguration()
{
	return _config;
}
