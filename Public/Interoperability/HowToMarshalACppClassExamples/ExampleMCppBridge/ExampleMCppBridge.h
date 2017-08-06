#ifndef __TestClassDLLBridge_h__
#define __TestClassDLLBridge_h__

using namespace System;


/*namespace ExampleMCppBridge {

	public ref class Class1
	{
		// TODO: Add your methods for this class here.
	};
}*/


// Forward declariation
class CUnmanagedTestClass;

public ref class CExampleMCppBridge
{
public:
	CExampleMCppBridge();
	virtual ~CExampleMCppBridge();
	void PassInt(int nValue);
	void PassString(String^ strValue);
	String^ ReturnString();

private:
	CUnmanagedTestClass* m_pUnmanagedTestClass;
};


#endif	// __TestClassDLLBridge_h__
