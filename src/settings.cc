#include "sdrplay.h"

namespace sdrplay {
	void SetDeviceIdx(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() !=1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetDeviceIdx(args[0]->Uint32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to set device index.")));
	    return;
	  }
	}

	void ReleaseDeviceIdx(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() >0) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_ReleaseDeviceIdx();
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to release device index.")));
	    return;
	  }
	}


	void SetDcMode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() !=2) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetDcMode(args[0]->IntegerValue(), args[1]->IntegerValue());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to set DC mode.")));
	    return;
	  }
	}

	void SetDcTrackTime(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetDcTrackTime(args[0]->IntegerValue());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to set DC track time value.")));
	    return;
	  }
	}

	void DCoffsetIQimbalanceControl(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 2) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_DCoffsetIQimbalanceControl(args[0]->IntegerValue(), args[1]->IntegerValue());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to set DC track time value.")));
	    return;
	  }
	}

	void SetPpm(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetPpm(args[0]->NumberValue());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to set PPM value.")));
	    return;
	  }
	}

	void SetLoMode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetLoMode((mir_sdr_LoModeT )args[0]->IntegerValue());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to set PPM value.")));
	    return;
	  }
	}

	void SetTransferMode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetTransferMode((mir_sdr_TransferModeT)args[0]->IntegerValue());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::Error(
	        String::NewFromUtf8(isolate, "Unable to set PPM value.")));
	    return;
	  }
	}
}