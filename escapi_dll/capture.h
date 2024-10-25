#pragma once


class CaptureClass : public IMFSourceReaderCallback
{
public:

	CaptureClass();
	~CaptureClass();
	STDMETHODIMP QueryInterface(REFIID aRiid, void** aPpv);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP OnReadSample(
		HRESULT aStatus,
		DWORD aStreamIndex,
		DWORD aStreamFlags,
		LONGLONG aTimestamp,
		IMFSample *aSample
		);
	STDMETHODIMP OnEvent(DWORD, IMFMediaEvent *);
	STDMETHODIMP OnFlush(DWORD);
	int escapiPropToMFProp(int aProperty);
	int setProperty(int aProperty, float aValue, int aAuto);
	int getProperty(int aProperty, float &aValue, int &aAuto);
	int getPropertyRaw(int aProperty, long* aValue, long* aAuto);
	int setPropertyRaw(int aProperty, long aValue, long aAuto);
	int getPropertyRange(int aProperty, long* minimum, long* maximum, long* step, long* def, long* flags);
	BOOL isFormatSupported(REFGUID aSubtype) const;
	HRESULT getFormat(DWORD aIndex, GUID *aSubtype) const;
	HRESULT setConversionFunction(REFGUID aSubtype);
	HRESULT setVideoType(IMFMediaType *aType);
	int isMediaOk(IMFMediaType *aType, int aIndex);
	int scanMediaTypes(unsigned int aWidth, unsigned int aHeight);
	HRESULT initCapture(int aDevice, int selectedMode);
	HRESULT listModes(int aDevice, struct CaptureModeParam* modes, int *count);
	void deinitCapture();

	long                    mRefCount;        // Reference count.
	CRITICAL_SECTION        mCritsec;

	IMFSourceReader         *mReader;
	IMFMediaSource			*mSource;

	LONG                    mDefaultStride;
	IMAGE_TRANSFORM_FN      mConvertFn;    // Function to convert the video to RGB32

	unsigned int			*mCaptureBuffer;
	unsigned int			mCaptureBufferWidth, mCaptureBufferHeight;
	int						mErrorLine;
	int						mErrorCode;
	int						mWhoAmI;
	unsigned int			*mBadIndex;
	unsigned int			mBadIndices;
	unsigned int			mMaxBadIndices;
	unsigned int			mUsedIndex;
	int						mRedoFromStart;
};
