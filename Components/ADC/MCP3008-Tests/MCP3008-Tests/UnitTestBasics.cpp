#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../ADCDriver/Mcp3008Driver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ADCDriver;

namespace MCP3008Tests
{
	TEST_CLASS(UnitTestBasics)
	{
		Mcp3008Driver* drv;
	public:

		TEST_METHOD(TestCreateMCP3008DriverWithDefaultName)
		{
			drv = new Mcp3008Driver();
			Assert::IsNotNull(drv, L"Expected valid handle");
			std::string name;
			Result result = drv->GetDriverName(name);
			std::string defaultName = "Mcp3008Driver";
			Assert::AreEqual(defaultName, name, L"Expected correct default driver name");
			delete drv;
		}

		TEST_METHOD(TestCreateMCP3008DriverWithSpecifiedName)
		{
			std::string expectedDriverName = "MCP3008";
			drv = new Mcp3008Driver(expectedDriverName);
			Assert::IsNotNull(drv, L"Expected valid handle");
			std::string name;
			Result result = drv->GetDriverName(name);
			Assert::AreEqual(expectedDriverName, name, L"Expected correct default driver name");
			delete drv;
		}

		TEST_METHOD(TestInitializeMCP3008DriverWithDefaults)
		{
			drv = new Mcp3008Driver();
			Assert::AreEqual((int)AudioDriverConfig::ADCSingleEndedCh0, (int)drv->GetAudioDriverConfiguration(), L"Expected default configuration to be ADCSingleEndedCh0");
		}

		TEST_METHOD(TestInitializeMCP3008DriverAsDifferentialCh2PosCh3Neg)
		{
			drv = new Mcp3008Driver();
			drv->ConfigureAudioDriver(AudioDriverConfig::ADCDifferentialCh2PosCh3Neg);
			Assert::AreEqual((int)AudioDriverConfig::ADCDifferentialCh2PosCh3Neg, (int)drv->GetAudioDriverConfiguration(), L"Expected Differential Ch2+Ch3- configuration");
		}

	};
}