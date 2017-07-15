#include "sdrplay.h"

namespace sdrplay {

	void DebugEnable(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_DebugEnable(args[0]->Uint32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to enable debug mode.")));
	    return;
	  }
	}

	void GetDevices(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Missing max device argument.")));
	    return;
	  }  
	  mir_sdr_DeviceT devices[args[0]->IntegerValue()];
	  unsigned int numDevs;
	  mir_sdr_ErrT error = mir_sdr_GetDevices(&devices[0], &numDevs, args[0]->IntegerValue());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to get list of connected devices.")));
	    return;
	  }
	  Local<Array> devArray = Array::New(isolate);
	  // Fill result array
	  for (uint i=0;i<numDevs;i++) {
	  	Handle<Object> devObj = Object::New(isolate);
	  	devObj->Set(String::NewFromUtf8(isolate,"SerNo"), String::NewFromUtf8(isolate,devices[0].SerNo));
	  	devObj->Set(String::NewFromUtf8(isolate,"DevNm"), String::NewFromUtf8(isolate,devices[0].DevNm));
	  	devObj->Set(String::NewFromUtf8(isolate,"hwVer"), Number::New(isolate,devices[0].hwVer));
	  	devObj->Set(String::NewFromUtf8(isolate,"devAvail"), Number::New(isolate,devices[0].devAvail));
	  	devArray->Set(i,devObj);
	  }
	  args.GetReturnValue().Set(devArray);
	}

	void GetHwVersion(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() > 0) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "No argument needed.")));
	    return;
	  }  
	  unsigned char hwVersion;
	  mir_sdr_ErrT error = mir_sdr_GetHwVersion(&hwVersion);
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to get Hardware version.")));
	    return;
	  }
	  args.GetReturnValue().Set(Number::New(isolate, hwVersion));
	}


	void ApiVersion(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() > 0) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "No argument needed.")));
	    return;
	  }  
	  float version;
	  mir_sdr_ErrT error = mir_sdr_ApiVersion(&version);
	  if ((error!= mir_sdr_Success) || (version!= MIR_SDR_API_VERSION)) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to retrieve api version.")));
	    return;
	  }
	  Local<Number> num = Number::New(isolate, version);
	  args.GetReturnValue().Set(num);
	}
}