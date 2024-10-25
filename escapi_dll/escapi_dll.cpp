#include "windows.h"
#define ESCAPI_DEFINITIONS_ONLY
#include "escapi.h"
#include <stdio.h>


#define MAXDEVICES 16

extern struct SimpleCapParams gParams[];
extern int gDoCapture[];
extern int gOptions[];

extern HRESULT InitDevice(int device, int selectedMode);
extern void CleanupDevice(int device);
extern int CountCaptureDevices();
extern void GetCaptureDeviceName(int deviceno, char * namebuffer, int bufferlength);
extern void CheckForFail(int device);
extern int GetErrorCode(int device);
extern int GetErrorLine(int device);
extern float GetProperty(int device, int prop);
extern int GetPropertyAuto(int device, int prop);
extern int SetProperty(int device, int prop, float value, int autoval);
extern int GetPropertyRange(int aDevice, int aProp, long* minimum, long* maximum, long* step, long* def, long* flags);
extern int GetPropertyRaw(int aDevice, int aProp, long* aValue, long* aAuto);
extern int SetPropertyRaw(int aDevice, int aProp, long aValue, long aAuto);
extern HRESULT ListModes(int aDevice, struct CaptureModeParam* modes, int* count);

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	return TRUE;
}


extern "C" void __declspec(dllexport) getCaptureDeviceName(unsigned int deviceno, char *namebuffer, int bufferlength)
{
	if (deviceno > MAXDEVICES)
		return;

	GetCaptureDeviceName(deviceno, namebuffer, bufferlength);
}

extern "C" int __declspec(dllexport) ESCAPIDLLVersion()
{
	return 0x200; // due to mess up, earlier programs check for exact version; this needs to stay constant
}

extern "C" int __declspec(dllexport) ESCAPIVersion()
{
	return 0x301; // ...and let's hope this one works better
}

extern "C" int __declspec(dllexport) countCaptureDevices()
{
	int c = CountCaptureDevices();
	return c;
}

extern "C" void __declspec(dllexport) initCOM()
{
	CoInitialize(NULL);
}

extern "C" int __declspec(dllexport) initCapture(unsigned int deviceno, struct SimpleCapParams *aParams)
{
	if (deviceno > MAXDEVICES)
		return 0;
	if (aParams == NULL || aParams->mHeight <= 0 || aParams->mWidth <= 0 || aParams->mTargetBuf == 0)
		return 0;
	gDoCapture[deviceno] = 0;
	gParams[deviceno] = *aParams;
	gOptions[deviceno] = 0;
	int mode = aParams->selectedMode;
	if (!aParams->forceOverride)mode = -1;
	if (FAILED(InitDevice(deviceno, mode))) return 0;
	return 1;
}

extern "C" void __declspec(dllexport) deinitCapture(unsigned int deviceno)
{
	if (deviceno > MAXDEVICES)
		return;
	CleanupDevice(deviceno);
}

extern "C" void __declspec(dllexport) doCapture(unsigned int deviceno)
{
	if (deviceno > MAXDEVICES)
		return;
	CheckForFail(deviceno);
	gDoCapture[deviceno] = -1;
}

extern "C" int __declspec(dllexport) isCaptureDone(unsigned int deviceno)
{
	if (deviceno > MAXDEVICES)
		return 0;
	CheckForFail(deviceno);
	if (gDoCapture[deviceno] == 1)
		return 1;
	return 0;
}

extern "C" int __declspec(dllexport) getCaptureErrorLine(unsigned int deviceno)
{
	if (deviceno > MAXDEVICES)
		return 0;
	return GetErrorLine(deviceno);
}

extern "C" int __declspec(dllexport) getCaptureErrorCode(unsigned int deviceno)
{
	if (deviceno > MAXDEVICES)
		return 0;
	return GetErrorCode(deviceno);
}

extern "C" float __declspec(dllexport) getCapturePropertyValue(unsigned int deviceno, int prop)
{
	if (deviceno > MAXDEVICES)
		return 0;
	return GetProperty(deviceno, prop);
}

extern "C" int __declspec(dllexport) getCapturePropertyAuto(unsigned int deviceno, int prop)
{
	if (deviceno > MAXDEVICES)
		return 0;
	return GetPropertyAuto(deviceno, prop);
}

extern "C" int __declspec(dllexport) setCaptureProperty(unsigned int deviceno, int prop, float value, int autoval)
{
	if (deviceno > MAXDEVICES)
		return 0;
	return SetProperty(deviceno, prop, value, autoval);
}

extern "C" int __declspec(dllexport) getCapturePropertyRange(unsigned int deviceno, int aProp, long* minimum, long* maximum, long* step, long* def, long* flags)
{
	if (deviceno > MAXDEVICES)
		return 0;
	return GetPropertyRange(deviceno, aProp, minimum, maximum, step, def, flags);
}

extern "C" int __declspec(dllexport) getCapturePropertyRaw(unsigned int deviceno, int aProp, long* aValue, long* aAuto)
{
	if (deviceno > MAXDEVICES)
		return 0;
	return GetPropertyRaw(deviceno, aProp, aValue, aAuto);
}

extern "C" int __declspec(dllexport) setCapturePropertyRaw(unsigned int deviceno, int aProp, long aValue, long aAuto)
{
	if (deviceno > MAXDEVICES)
		return 0;
	//printf("setting property raw.........................................\n");
	return SetPropertyRaw(deviceno, aProp, aValue, aAuto);
}


extern "C" int __declspec(dllexport) initCaptureWithOptions(unsigned int deviceno, struct SimpleCapParams *aParams, unsigned int aOptions)
{
	if (deviceno > MAXDEVICES)
		return 0;
	if (aParams == NULL || aParams->mHeight <= 0 || aParams->mWidth <= 0 || aParams->mTargetBuf == 0)
		return 0;
	if ((aOptions & CAPTURE_OPTIONS_MASK) != aOptions)
		return 0;
	gDoCapture[deviceno] = 0;
	gParams[deviceno] = *aParams;
	gOptions[deviceno] = aOptions;
	int mode = aParams->selectedMode;
	if (!aParams->forceOverride)mode = -1;
	if (FAILED(InitDevice(deviceno, mode))) return 0;
	return 1;
}

extern "C" int __declspec(dllexport) listCaptureModes(unsigned int deviceno, struct CaptureModeParam* modes, int* count)
{
	if (deviceno > MAXDEVICES)
		return 0;
	//printf("listCaptureModes.........................................\n");
	//fflush(stdout);
	return ListModes(deviceno, modes, count);
}