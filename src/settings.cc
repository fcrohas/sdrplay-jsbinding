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
	  mir_sdr_ErrT error = mir_sdr_SetDeviceIdx(args[0]->Uint32Value(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
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
	  args.GetReturnValue().Set(error);
	}


	void SetDcMode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() !=2) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetDcMode(args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust(), args[1]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}

	void SetDcTrackTime(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetDcTrackTime(args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}

	void DCoffsetIQimbalanceControl(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 2) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_DCoffsetIQimbalanceControl(args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust(), args[1]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}

	void SetPpm(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetPpm(args[0]->NumberValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}

	void SetLoMode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetLoMode((mir_sdr_LoModeT )args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}

	void SetTransferMode(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetTransferMode((mir_sdr_TransferModeT)args[0]->IntegerValue(Nan::GetCurrentContext()).FromJust());
	  args.GetReturnValue().Set(error);
	}
}
