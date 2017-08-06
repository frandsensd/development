#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/*
* MCP3008 is a 8-input ADC, each line sampling in 10-bit resolution at max 200kS/s.
* MCP3008 is controlled via SPI.
* VDD range [2.7-5.5]VDC.
* Chip pinout:
*	1	CH0		16	VDD
*	2	CH1		15	VREF
*	3	CH2		14	AGND
*	4	CH3		13	CLK
*	5	CH4		12	DOUT
*	6	CH5		11	DIN
*	7	CH6		10	!CS/SHDN
*	8	CH7		 9	DGND
*
* Serial Interface Timing:
* Legend: HI(-), LO(_), Tri-state(~), Don't care(*), HL(\), LH(/), Negated(!)
* !CS --\_________________________________/---
* CLK ----\__/-..-\__/--\__/--\__/--\__/--\__/
* DIN ----|MSB|...|LSB|***********************
* DOUT~~~~~~~~~~~~~~~~~~~\_0_/|MSB|...|LSB|~~~
* CLK: 3.6MHz@5VDD, 1.35MHz@2.7VDD
* TSU: from DIN ready to CLK HI  >= 50ns
* TSUCS: from !CS LO to 1st CLK HI >= 100ns
* THD: DIN hold time from CLK HI >= 50 ns
* TEN: CLK LO to Output Enable <= 125ns@5V,200ns@2.7V
* TDO: CLK LO go Output data ready <= 125ns@5V,200ns@2.7V
* TCLKHI/TCLKLO >= 125ns
* TCSHI: !CS Disable time >= 270ns
* TDIS: !CS HI until output disable <=100ns.
*
* Start bit is !CS=0, DIN=1 on first clock.
*
* Four bits (Single/Diff, D2, D1, D0) defines the input
* bits on DIN with Single/Diff as MSB.
*  Single/Diff selects Single/Differential mode
*  D2-D1-D0 selects CHx, x=D2|D1|D0
* Now after this one clock cycle will be used for Sample/Hold.
* Next clock cycle will output a NULL as a first
*
* A full sampling cycle:
*             |Startbit (!CS=0, DIN=1, CLK/)
*             |                             |Sample/Hold
* !CS --\_____|_____________________________|____________________________________________________________________/-----...
* CLK -----\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\__/--\..
* DIN -------------|SD|  |D2|  |D1|  |D0|----------------------------------
* DOUT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\_0_/|Bit9||Bit8||Bit7||Bit6||Bit5||Bit4||Bit3||Bit2||Bit1||Bit0|~~~~
*
* LSB Size = VREF / 1024
* Output: 1024 * VIN / VREF
*
* Host CPU configuration:
* GPIO_CHIP_SELECT_ACTIVE_LOW -> !CS
* GPIO_CLK                    -> CLK
* CPIO_DIN                    -> DIN (SPI MOSI)
* GPIO_DOUT                   -> DOUT (SPI MISO)
*
* Sample rate 200kS/s => fCLK = 18 * 200kS/s = 3.6MHz => Tperiod ~ 0.27us => THI=TLO~0.13us~130ns.
* CPU must be able to toggle a GPIO this fast along with controlling the other three GPIO's to achieve the max sampling rate for MCP3008.
* Raspberry Pi - consider using memory mapped GPIO.
*
*/
#include "../ADCComponent/MCP3008.h"
using namespace ADCComponent;

namespace UnitTests
{		
	TEST_CLASS(ADCMCP3008Component)
	{
	public:
		
		TEST_METHOD(TestCreateMCP3008)
		{
			auto adc = new MCP3008();
			Assert::IsNotNull(adc, L"Expected a valid instance");
		}

		TEST_METHOD(TestCreateAndDestroyMCP3008)
		{
			auto adc = new MCP3008();
			adc->~MCP3008();
			Assert::IsNull(adc, L"Expected a NULL instance");
		}

	};
}