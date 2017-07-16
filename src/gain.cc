#include "sdrplay.h"

namespace sdrplay {

	void SetGr(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 3) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetGr(args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value());
	  if (error!= mir_sdr_Success) {
	  	cout << "error=" << error << "\r\n";
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to set gain.")));
	    return;
	  }
	}

	void SetGrParams(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 2) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetGrParams(args[0]->Int32Value(), args[1]->Int32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to deinitialize stream.")));
	    return;
	  }
	}

	void AgcControl(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 7) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_AgcControl((mir_sdr_AgcControlT)args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value(), 
	  											args[3]->Uint32Value(), args[4]->Uint32Value(), args[5]->Int32Value(),
	  											args[6]->Int32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to deinitialize stream.")));
	    return;
	  }
	}

	void GetCurrentGain(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  

	  mir_sdr_GainValuesT gain;
	  mir_sdr_ErrT error = mir_sdr_GetCurrentGain(&gain);
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to deinitialize stream.")));
	    return;
	  }
	}

	void RSP_SetGr(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 4) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_RSP_SetGr(args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value(), args[3]->Int32Value());
	  if (error!= mir_sdr_Success) {
	  	cout << "error=" << error << "\r\n";
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to set RSP gain.")));
	    return;
	  }
	}

	void RSP_SetGrLimits(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_RSP_SetGrLimits((mir_sdr_MinGainReductionT)args[0]->Int32Value());
	  if (error!= mir_sdr_Success) {
	  	cout << "error=" << error << "\r\n";
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to set RSP gain limits.")));
	    return;
	  }
	}

}