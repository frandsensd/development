#pragma once

namespace ADC
{

	class Mcp3008
	{
		wchar_t* _driverId;
	public:
		Mcp3008();
		~Mcp3008();
		Mcp3008Results GetDriverID(wchar_t*);
	};
}
