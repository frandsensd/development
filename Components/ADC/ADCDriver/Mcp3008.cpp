#include "Mcp3008.h"
#include <string>
ADC::Mcp3008::Mcp3008()
{
	_driverId = L"MCP3008";
}

ADC::Mcp3008::~Mcp3008()
{
	_driverId = L"";
}

ADC::Mcp3008Results ADC::Mcp3008::GetDriverID(wchar_t *name)
{
	wcscpy_s(name, sizeof(_driverId)/sizeof(wchar_t), _driverId);
	return ADC::Mcp3008Results::OK;
}
