
#include <STDIO.H>									// needed for NULL

#include "ExampleMCppBridge.h"
#include "ExampleUnmanagedDLL.h"

using namespace System::Runtime::InteropServices;	// needed for Marshal



CExampleMCppBridge::CExampleMCppBridge()
	: m_pUnmanagedTestClass(NULL)
{
	this->m_pUnmanagedTestClass = new CUnmanagedTestClass();
}

CExampleMCppBridge::~CExampleMCppBridge()
{
	delete this->m_pUnmanagedTestClass;
	this->m_pUnmanagedTestClass = NULL;
}

void CExampleMCppBridge::PassInt(int nValue)
{
	this->m_pUnmanagedTestClass->PassInt(nValue);
}

void CExampleMCppBridge::PassString(String^ strValue)
{
	IntPtr pString = Marshal::StringToHGlobalAnsi(strValue);
	try
	{
		char* pchString = static_cast<char *>(pString.ToPointer());
		this->m_pUnmanagedTestClass->PassString(pchString);
	}
	finally
	{
		Marshal::FreeHGlobal(pString);
	}
}

String^ CExampleMCppBridge::ReturnString()
{
	char* pchValue = this->m_pUnmanagedTestClass->ReturnString();
	String^ strValue = Marshal::PtrToStringAnsi(static_cast<IntPtr>(pchValue));
	return strValue;
}

