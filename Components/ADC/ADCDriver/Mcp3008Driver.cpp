#include <string>

#include "Mcp3008Driver.h"

const std::string DefaultDriverName = "Mcp3008Driver";

ADCDriver::Mcp3008Driver::Mcp3008Driver()
{
	_driverName = DefaultDriverName;
	_config = ADCConfig::ADCSingleEndedCh0;
}

ADCDriver::Mcp3008Driver::Mcp3008Driver(std::string name)
{
	_driverName = name;
	_config = ADCConfig::ADCSingleEndedCh0;
}

ADCDriver::Mcp3008Driver::~Mcp3008Driver()
{
}

ADCDriver::Result ADCDriver::Mcp3008Driver::GetDriverName(std::string& name)
{
	name = _driverName;
	return ADCDriver::OK;
}

ADCDriver::Result ADCDriver::Mcp3008Driver::ConfigureADC(ADCConfig config)
{
	_config = config;
	return ADCDriver::OK;
}

ADCDriver::ADCConfig ADCDriver::Mcp3008Driver::GetADCConfiguration()
{
	return _config;
}

ADCDriver::Result ADCDriver::Mcp3008Driver::WriteStartSequenceToHardware()
{
#ifndef USE_MCP3008_DOUBLE_TO_ABSTRACT_HW
	this.hal.
#endif
	return ADCDriver::Result::ERR_BUFFER_TOO_SMALL;
}
