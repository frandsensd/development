#pragma once
#include "api.h"

namespace ADCDriver
{
	typedef enum {
		OK,
		ERR_BUFFER_TOO_SMALL,
	} Result;

	typedef enum {
		ADCSingleEndedCh0 = 8,
		ADCSingleEndedCh1 = 9,
		ADCSingleEndedCh2 = 10,
		ADCSingleEndedCh3 = 11,
		ADCSingleEndedCh4 = 12,
		ADCSingleEndedCh5 = 13,
		ADCSingleEndedCh6 = 14,
		ADCSingleEndedCh7 = 15,
		ADCDifferentialCh0PosCh1Neg = 0,
		ADCDifferentialCh1PosCh0Neg = 1,
		ADCDifferentialCh2PosCh3Neg = 2,
		ADCDifferentialCh3PosCh2Neg = 3,
		ADCDifferentialCh4PosCh5Neg = 4,
		ADCDifferentialCh5PosCh4Neg = 5,
		ADCDifferentialCh6PosCh7Neg = 6,
		ADCDifferentialCh7PosCh6Neg = 7,
	} AudioDriverConfig;
	
	class Mcp3008Driver
	{
		std::string _driverName;
		AudioDriverConfig _config;
	public:
		Mcp3008Driver();
		Mcp3008Driver(std::string name);
		~Mcp3008Driver();
		Result GetDriverName(std::string& name);
		Result ConfigureAudioDriver(AudioDriverConfig config);
		AudioDriverConfig GetAudioDriverConfiguration();
	};
}
