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
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to select antenna.")));
	    return;
	  }
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
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to select external reference.")));
	    return;
	  }
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
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to select bias-t.")));
	    return;
	  }
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
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to select notch filter.")));
	    return;
	  }
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
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to select am port.")));
	    return;
	  }
	}
}
