#include "sdrplay.h"

namespace sdrplay {

	void RSPII_AntennaControl(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Missing argument.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_RSPII_AntennaControl((mir_sdr_RSPII_AntennaSelectT)args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}

	void RSPII_ExternalReferenceControl(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Missing argument.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_RSPII_ExternalReferenceControl(args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}

	void RSPII_BiasTControl(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Missing argument.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_RSPII_BiasTControl(args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}

	void RSPII_RfNotchEnable(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Missing argument.")));
	    return;
	  }
	  mir_sdr_ErrT error = mir_sdr_RSPII_RfNotchEnable(args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust());	  
	  args.GetReturnValue().Set(error);
	}

	void AmPortSelect(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Missing argument.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_AmPortSelect(args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}
}
