#include <iostream>
#include <string.h>
#include <nan.h>
#include <uv.h>
#include <mirsdrapi-rsp.h>
#ifndef _WIN32
#include <unistd.h>
#include <thread>
#define Sleep(x) usleep((x)*1000)
#endif
namespace sdrplay {
	using namespace v8;
	using namespace Nan;
	using namespace std;

	struct sdrplay_t {
		short *xi;
		short *xq;
		int firstSampleNum;
		int grChanged;
		int rfChanged;
		int fsChanged;
		int numSamples;
		int reset;
		unsigned int gRdB;
		unsigned int lnaGRdB;
	};
	/* Device Info */
	void GetDevices(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void DebugEnable(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void GetHwVersion(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void ApiVersion(const Nan::FunctionCallbackInfo<v8::Value>& args);
	/* Stream */
	void StreamInit(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void StreamUninit(const Nan::FunctionCallbackInfo<v8::Value>& args);

	/* device selection */
	void SetDeviceIdx(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void ReleaseDeviceIdx(const Nan::FunctionCallbackInfo<v8::Value>& args);

	/* Settings */
	void SetPpm(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void SetDcMode(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void SetDcTrackTime(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void DCoffsetIQimbalanceControl(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void SetLoMode(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void SetTransferMode(const Nan::FunctionCallbackInfo<v8::Value>& args);

	/* RSP 2 */
	void RSPII_AntennaControl(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void RSPII_ExternalReferenceControl(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void RSPII_BiasTControl(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void RSPII_RfNotchEnable(const Nan::FunctionCallbackInfo<v8::Value>& args);
	void AmPortSelect(const Nan::FunctionCallbackInfo<v8::Value>& args);
}