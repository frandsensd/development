using System;


namespace ExampleTestAppUsingMCppBridge
{
	class Program
	{
		static void Main(string[] args)
		{
			CExampleMCppBridge example = new CExampleMCppBridge();
			example.PassInt(42);
			example.PassString("forty-two");
			string strReturn = example.ReturnString();
			System.Console.WriteLine("The sting returned was: " + strReturn);

			example.Dispose();
		}
	}
}
