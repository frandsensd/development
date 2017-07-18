#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MCP3008Tests
{		
	TEST_CLASS(UnitTestCommunication)
	{
	public:

		TEST_METHOD(TestWriteStartSequence_Mocked_CS_SCLK_DIN)
		{
			// Arrange: Create Mcp3008 mock which will capture writes to CS, SCLK and DIN in three separate buffers, 
			//			time aligned based on CS and SCLK, simulating MCP3008 behavior.
			// Act:		Simulate a memory mapped GPIO pin write by writing to the mock for CS, SCLK and DIN.
			// Assert:	Fetch captured bits on CS, SCLK and DIN and write each stream on one line as 0/1 for approval/assertion.
			Assert::Fail(L"Not implemented");
		}

		TEST_METHOD(TestWriteStartSequenceAndConfigureForFirstCapture_Mocked_CS_SCLK_DIN_DOUT)
		{
			// Arrange: Create Mcp3008 mock which will capture writes to CS, SCLK and DIN in three separate buffers, 
			//			time aligned based on CS and SCLK, simulating MCP3008 behavior. Will also generate an output
			//			based on input configuration which can be approved upon.
			// Act:		Simulate a memory mapped GPIO pin write by writing to the mock for CS, SCLK and DIN.
			// Assert:	Fetch captured bits on CS, SCLK, DIN and DOUT and write each stream on one line as 0/1 for approval/assertion.
			Assert::Fail(L"Not implemented");
		}

	};
}