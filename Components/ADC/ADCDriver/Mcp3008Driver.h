#pragma once
#include "api.h"

/* NOTE: To use Doubles for testing purpose,
** define USE_MCP3008_DOUBLE_TO_ABSTRACT_HW
** as build flag! */

namespace ADCDriver
{
	typedef enum {
		OK,
		ERR_BUFFER_TOO_SMALL,
		NUMBER_OF_ERRORS_IN_THIS_ENUM_DO_NOT_REFERENCE // Must be last
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
	} ADCConfig;
	
	const static unsigned MaxErrorMessageLength = 200;
	const static wchar_t ErrorMessages[Result::NUMBER_OF_ERRORS_IN_THIS_ENUM_DO_NOT_REFERENCE][MaxErrorMessageLength] = {
		L"No error",
		L"Buffer too small",
	};

	class Mcp3008Driver
	{
		std::string _driverName;
		ADCConfig _config;
#ifdef USE_MCP3008_DOUBLE_TO_ABSTRACT_HW
	protected:
		ADCHAL hal;
#endif
	public:
		Mcp3008Driver();
#ifdef USE_MCP3008_DOUBLE_TO_ABSTRACT_HW
		Mcp3008Driver(ADCHAL& halDouble) {
			hal = halDouble;
		}
#endif
		Mcp3008Driver(std::string name);
		~Mcp3008Driver();
		Result GetDriverName(std::string& name);
		Result ConfigureADC(ADCConfig config);
		ADCConfig GetADCConfiguration();
		Result WriteStartSequenceToHardware();
	};


	#define ADCOK(X) if (X != Result::OK) throw ADCException(X);


	class ADCException : protected std::exception {
		Result adcResult;
		wchar_t description[MaxErrorMessageLength];
		char whatDescription[MaxErrorMessageLength];
	public:
		ADCException(Result r)
			: std::exception("", r), adcResult(r) {
			wcscpy_s(description, MaxErrorMessageLength, L"ADCException! Description: ");
			wcscat_s(description, MaxErrorMessageLength, ErrorMessages[r]);
		}
		~ADCException() {};
		virtual const char* what() const throw() {
			size_t charsConverted;
			wcstombs_s(&charsConverted, (char*)whatDescription, (size_t)MaxErrorMessageLength, (wchar_t*)description, (size_t)MaxErrorMessageLength);
			return whatDescription;
		}
		wchar_t* GetErrorMessageW() { return description; }
	};

#ifdef USE_MCP3008_DOUBLE_TO_ABSTRACT_HW
	// Create double of the MCP3008 Physical HW interface.
	// Store all reads/writes to this interface
	class ADCHAL
	{
		static const int ADCHAL_ERROR_BUFFER_FULL = -1;
		static const int ADCHAL_OK = 0;
		static const unsigned NodeBufferSize = 1000;
		unsigned char writeToHwNodeBuffer[NodeBufferSize];
		unsigned char writeIndex;
	public:
		ADCHAL() {}
		virtual ~ADCHAL() {}

		virtual int write(const unsigned char* const pData, unsigned bytes, unsigned* const bytesWritten) {
			if (writeIndex == NodeBufferSize) {
				*bytesWritten = 0;
				return ADCHAL_ERROR_BUFFER_FULL;
			}
			unsigned char* pNextByte = &writeToHwNodeBuffer[0] + writeIndex;
			for (unsigned i=0; i<bytes; i++)
			{
				*pNextByte = pData[i];
				if (++writeIndex < NodeBufferSize) {
					pNextByte++;
				}
				else {
					*bytesWritten = pNextByte - &writeToHwNodeBuffer[0];
					return ADCHAL_ERROR_BUFFER_FULL;
				}
			}
			*bytesWritten = pNextByte - &writeToHwNodeBuffer[0];
			return ADCHAL_OK;
		}
	};
#endif
}
