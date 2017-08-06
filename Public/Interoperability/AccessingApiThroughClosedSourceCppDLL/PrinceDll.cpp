/*-------------------------------------------------------------------------------------------------------

    PrinceDll.cpp
    AUTHOR:	  Robert Bermani
              Continuum
              1220 Washington Street
              West Newton, MA 02465
              (617) 969-5400


    REVISION HISTORY:
    Date            Author      Description of change
    -----------------------------------------------------------
    01 Jan 2012     R. Bermani   initial edit

    DESCRIPTION:  Defines the exported functions for the DLL application.
	
    Copyright (c) 2009, Continuum.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted 
    provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of 
          conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of 
          conditions and the following disclaimer in the documentation and/or other materials provided 
          with the distribution.
        * Neither the name of Continuum nor the names of its contributors may be used to endorse or 
          promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY 
    WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

------------------------------------------------------------------------------------------------------*/

//-------------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "PrinceDll.h"
#include <QString>

//-------------------------------------------------------------------------------------------------------
// Function Pointer Typedefs
//-------------------------------------------------------------------------------------------------------
typedef long (__thiscall *_SetFaceTracking)(DWORD *pThis, unsigned long, long boolValue);
typedef long (__thiscall *_GetFaceTracking)(DWORD *pThis, unsigned long, long *boolValue);
typedef void (__thiscall *_CameraControls_Core)(DWORD *pThis);
typedef bool (__thiscall *_Init)(DWORD *pThis, class QString param1);
typedef long (__thiscall *_GetCurrentVideoDevice)(DWORD *pThis, unsigned long *param);
typedef long (__thiscall *_SetCurrentVideoDevice)(DWORD *pThis, unsigned long param);

//-------------------------------------------------------------------------------------------------------
// Static Variables
//-------------------------------------------------------------------------------------------------------

static BOOL				freeResult, objectInitialized = FALSE;
static _SetFaceTracking	LogitechSetFaceTracking;
static _GetFaceTracking LogitechGetFaceTracking;
static DWORD			dwFakeObject[512];
static HINSTANCE		CameraControlDllHandle = NULL;
static unsigned long	currentVideoDevice = 0;


PRINCEDLL_API HRESULT APIENTRY SetFaceTracking ( long boolValue )
{
	long result = 0;

	if (LogitechSetFaceTracking != NULL && objectInitialized == TRUE)
	{
		result = LogitechSetFaceTracking (dwFakeObject, currentVideoDevice, boolValue);

		return S_OK;
	} else return E_POINTER;

}

PRINCEDLL_API HRESULT APIENTRY GetFaceTracking ( long *boolValue )
{
	long result = 0;

	if (LogitechGetFaceTracking != NULL && objectInitialized == TRUE)
	{
		result = LogitechGetFaceTracking (dwFakeObject, currentVideoDevice, boolValue);

		return S_OK;
	} else return E_POINTER;

}


PRINCEDLL_API HRESULT APIENTRY InitLibrary()
{
	long retValue = 0;
	QString inputString = "";

	CameraControlDllHandle = LoadLibrary(L"CameraControls_Core.dll");
	
	if (CameraControlDllHandle == NULL)
		return E_FAIL;

	_CameraControls_Core LogitechImportedConstructor = (_CameraControls_Core) GetProcAddress(CameraControlDllHandle,"??0CameraControls_Core@CameraControls@LWS@@QAE@XZ");

	if (LogitechImportedConstructor == NULL)
		return E_FAIL;

	LogitechSetFaceTracking = (_SetFaceTracking) GetProcAddress (CameraControlDllHandle,"?SetFaceTracking@CameraControls_Core@CameraControls@LWS@@QAEJKJ@Z");

	if (LogitechSetFaceTracking == NULL)
		return E_FAIL;

	LogitechGetFaceTracking = (_GetFaceTracking) GetProcAddress (CameraControlDllHandle, "?GetFaceTracking@CameraControls_Core@CameraControls@LWS@@QAEJKPAJ@Z");

	if (LogitechGetFaceTracking == NULL)
		return E_FAIL;

	_Init LogitechInitializer = (_Init) GetProcAddress (CameraControlDllHandle, "?Init@CameraControls_Core@CameraControls@LWS@@QAE_NVQString@@@Z");

	if (LogitechInitializer == NULL)
		return E_FAIL;

	_SetCurrentVideoDevice LogSetCurrentVideoDevice = (_SetCurrentVideoDevice) GetProcAddress (CameraControlDllHandle, "?SetCurrentVideoDevice@CameraControls_Core@CameraControls@LWS@@QAEJK@Z");

	if (LogSetCurrentVideoDevice == NULL)
		return E_FAIL;

	_GetCurrentVideoDevice LogiGetCurrentVidDevice = (_GetCurrentVideoDevice) GetProcAddress (CameraControlDllHandle, "?GetCurrentVideoDevice@CameraControls_Core@CameraControls@LWS@@QAEJPAK@Z");

	if (LogiGetCurrentVidDevice == NULL)
		return E_FAIL;

	memset (dwFakeObject, 0x00, 512);
	objectInitialized = TRUE;

	LogitechImportedConstructor(dwFakeObject);
	retValue = LogitechInitializer(dwFakeObject, inputString);

	retValue = LogiGetCurrentVidDevice (dwFakeObject, &currentVideoDevice);
	retValue = LogSetCurrentVideoDevice (dwFakeObject, currentVideoDevice);

	return S_OK;
}

