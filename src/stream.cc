#include "sdrplay.h"

namespace sdrplay {
	// static uv_mutex_t mutex;
	static uv_cond_t isStreamRunning;
	struct stream_params_t {
		int gRdB;
		double fsMHz;
		double rfMHz;
		mir_sdr_Bw_MHzT bwType;
		mir_sdr_If_kHzT ifType;
		int LNAState;
		int gRdBsystem;
		mir_sdr_SetGrModeT setGrMode;
		int samplesPerPacket;
		int isRunning;
	};

	struct request_t {
	  uv_async_t *async;
	  uv_mutex_t async_lock; 
	  sdrplay_t sdrplay;
	  Callback* streamCallback;
	  Callback* gainCallback; 
	  bool fromGainCallback = false;
	  bool fromStreamCallback = false;
	  thread* streamWorker;
	};

	void sdrplay_gainCallback(unsigned int gRdB,unsigned int lnaGRdB, void *cbContext) {
		// cout << "gRdB=" << gRdB << " lnaGRdB=" << lnaGRdB;
		request_t *request = (request_t*)cbContext;
		uv_mutex_lock(&request->async_lock);  
		request->sdrplay.gRdB = gRdB;
		request->sdrplay.lnaGRdB = lnaGRdB;
		request->fromGainCallback = true;
		request->fromStreamCallback = false;
		request->async->data =  request;
		uv_mutex_unlock(&request->async_lock);
		// cout << "Calling progress callback !! with numSamples=" << numSamples << "\r\n";  
		uv_async_send(request->async);
	}


	void sdrplay_streamCallback(short *xi, short *xq, unsigned int firstSampleNum,int grChanged, int rfChanged, int fsChanged, unsigned int numSamples, unsigned int reset,unsigned int hwRemoved,void *cbContext) {
	  if (numSamples == 0) {
	    return;
	  }
	  // cout << "sdrplay_streamCallback\r\n";
	  request_t *request = (request_t*)cbContext;
	  uv_mutex_lock(&request->async_lock);  
	  // Fill data struct
	  request->sdrplay.grChanged = grChanged;
	  request->sdrplay.rfChanged = rfChanged;
	  request->sdrplay.fsChanged = fsChanged;
	  request->sdrplay.numSamples = numSamples;
	  request->sdrplay.firstSampleNum = firstSampleNum;
	  request->sdrplay.reset = reset;
	  if (request->sdrplay.mode) {
	  	sdrplay_t *sdrplay = &request->sdrplay;
	  	// buffering
	  	sdrplay->data_end = sdrplay->data_index + numSamples * 2;
	  	int count2 = sdrplay->data_end - sdrplay->buffer_size * sdrplay->buffer_count; // count2 is samples wrapping around to start of buf
	  	if (count2 < 0) count2 = 0;  
	  	int count1 = numSamples * 2 - count2; // count1 is samples fitting before the end of buf
		// flag is set if this packet takes us past a multiple of ASYNC_BUF_SIZE
	  	int new_buffer_flag = (sdrplay->data_index / sdrplay->buffer_size) == (sdrplay->data_end / sdrplay->buffer_size ) ? 0 : 1;
		// now interleave data from I/Q into circular buffer
	  	int input_index = 0;
		for (int i = 0, j = sdrplay->data_index; i < count1 / 2; i++) {
	  		sdrplay->data_buffer[j++] = xi[input_index];
	  		sdrplay->data_buffer[j++] = xq[input_index];
	  		input_index++;
	  	}

		sdrplay->data_index += count1;

	  	if (sdrplay->data_index >= sdrplay->buffer_size * sdrplay->buffer_count) {
	  		sdrplay->data_index = 0;
	  	}

		for (int i = 0, j = sdrplay->data_index; i < count2 / 2; i++) {
	  		sdrplay->data_buffer[j++] = xi[input_index];
	  		sdrplay->data_buffer[j++] = xq[input_index];
	  		input_index++;
	  	}

		sdrplay->data_index += count2;

		// send ASYNC_BUF_SIZE samples downstream, if available
	  	if(new_buffer_flag) {
			/* go back by one buffer length, then round down further to start of buffer */
			sdrplay->data_end = sdrplay->data_index - sdrplay->buffer_size;
			if (sdrplay->data_end<0) sdrplay->data_end += sdrplay->buffer_size * sdrplay->buffer_count;
			sdrplay->data_end -= sdrplay->data_end % sdrplay->buffer_size;
		        for (int i = 0; i < sdrplay->buffer_size ; i++) {
			  if (sdrplay->data_end + i > sdrplay->buffer_size * sdrplay->buffer_count) {
			    sdrplay->current_buffer[i] = sdrplay->data_buffer[sdrplay->data_end + i - sdrplay->buffer_size * sdrplay->buffer_count];
			  } else {
			    sdrplay->current_buffer[i] = sdrplay->data_buffer[i];
			  }	  
			}

			request->fromStreamCallback = true;
			request->fromGainCallback = false;
			request->async->data = request;
  		    uv_mutex_unlock(&request->async_lock);			
			uv_async_send(request->async);
	  	} else {
  		    uv_mutex_unlock(&request->async_lock);	  		
	  	}
	  } else {
		  // Copy data struct
		  memcpy(request->sdrplay.xi, xi, numSamples*sizeof(short));
		  memcpy(request->sdrplay.xq, xq, numSamples*sizeof(short));
		  request->fromStreamCallback = true;
		  request->fromGainCallback = false;
		  request->async->data = request;
		  uv_mutex_unlock(&request->async_lock);
		  uv_async_send(request->async);
	  }
	}

	NAUV_WORK_CB(HandleStreamCallback) {
		// cout << "Filling javascript parameters !!\r\n";

		request_t *request = (request_t*)async->data;
		uv_mutex_lock(&request->async_lock);
		Nan::HandleScope scope;
		if (request->fromGainCallback) {
			const unsigned argc = 2;
			Local<Value> argv[] = { 
					  New<v8::Integer>( request->sdrplay.gRdB),
					  New<v8::Integer>( request->sdrplay.lnaGRdB)}; 
			// cout << "Calling javascript gain callback !!\r\n";
			request->gainCallback->Call(argc, argv);

		} else if (request->fromStreamCallback) {
			const unsigned argc = 8;
			if (request->sdrplay.mode) {
				Local<Value> argv[] = { CopyBuffer((char*)&request->sdrplay.current_buffer[0], request->sdrplay.buffer_size * 2).ToLocalChecked(),  
					  New<v8::Integer>( request->sdrplay.buffer_size),
					  New<v8::Integer>( request->sdrplay.firstSampleNum),
					  New<v8::Integer>( request->sdrplay.grChanged), 
					  New<v8::Integer>( request->sdrplay.rfChanged), 
					  New<v8::Integer>( request->sdrplay.fsChanged), 
					  New<v8::Integer>( request->sdrplay.numSamples),
					  New<v8::Integer>( request->sdrplay.reset)}; 
				request->streamCallback->Call(argc, argv);					  
			} else {
				Local<Value> argv[] = { CopyBuffer((char*)request->sdrplay.xi, request->sdrplay.numSamples*sizeof(short)).ToLocalChecked(),  
					  CopyBuffer((char*)request->sdrplay.xq, request->sdrplay.numSamples*sizeof(short)).ToLocalChecked(),
					  New<v8::Integer>( request->sdrplay.firstSampleNum),
					  New<v8::Integer>( request->sdrplay.grChanged), 
					  New<v8::Integer>( request->sdrplay.rfChanged), 
					  New<v8::Integer>( request->sdrplay.fsChanged), 
					  New<v8::Integer>( request->sdrplay.numSamples),
					  New<v8::Integer>( request->sdrplay.reset)}; 
				request->streamCallback->Call(argc, argv);					  
			}
		}
		uv_mutex_unlock(&request->async_lock);
	}

	void startStream (stream_params_t streamParams, request_t request) {
		// Create data struct
		// Initialize array content
		if (request.sdrplay.mode) {
			request.sdrplay.data_buffer = (short *)calloc(request.sdrplay.buffer_size * request.sdrplay.buffer_count, sizeof(short));
			request.sdrplay.current_buffer = (short *)calloc(request.sdrplay.buffer_size, sizeof(short));
			request.sdrplay.data_index = 0;
		} else {
			request.sdrplay.xi = (short *)calloc(streamParams.samplesPerPacket, sizeof(short));
			request.sdrplay.xq = (short *)calloc(streamParams.samplesPerPacket, sizeof(short));
		}

		mir_sdr_ErrT error =  mir_sdr_StreamInit(&streamParams.gRdB,
			streamParams.fsMHz, 
			streamParams.rfMHz, 
			streamParams.bwType, 
			streamParams.ifType, 
			streamParams.LNAState, 
			&streamParams.gRdBsystem, 
			streamParams.setGrMode, 
			&streamParams.samplesPerPacket, 
			sdrplay_streamCallback, sdrplay_gainCallback, // Callbacks
			&request);
		if (error!= mir_sdr_Success) {
			cout << "Error while initializing stream. Error : " << error << "\r\n";
			return;
		}

		uv_cond_wait(&isStreamRunning, &request.async_lock);
		uv_mutex_unlock(&request.async_lock);		
		uv_close((uv_handle_t*)request.async, NULL);
		if (request.sdrplay.mode) {
			free(request.sdrplay.data_buffer);
			free(request.sdrplay.current_buffer);
		} else {
			free(request.sdrplay.xi);
			free(request.sdrplay.xq);
		}
	}

	void StreamInit(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Nan::HandleScope scope;
	  Isolate* isolate = args.GetIsolate();
	  if ((args.Length() != 11) && (args.Length() != 13)) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  // Initialize parameters
	  request_t request;
	  request.async = new uv_async_t();
	  stream_params_t streamParams;
	  streamParams.gRdB = args[0]->Uint32Value();
	  streamParams.fsMHz = args[1]->NumberValue();
	  streamParams.rfMHz = args[2]->NumberValue();
	  streamParams.bwType = static_cast<mir_sdr_Bw_MHzT>(args[3]->Uint32Value());
	  streamParams.ifType = static_cast<mir_sdr_If_kHzT>(args[4]->Uint32Value());
	  streamParams.LNAState = args[5]->Uint32Value();
	  streamParams.gRdBsystem = args[6]->Uint32Value();
	  streamParams.setGrMode = static_cast<mir_sdr_SetGrModeT>(args[7]->Uint32Value());
	  streamParams.samplesPerPacket = args[8]->Uint32Value();
	  // Context
	  request.streamCallback = new Nan::Callback(args[9].As<Function>());
	  request.gainCallback = new Nan::Callback(args[10].As<Function>());
	  request.sdrplay.mode = args.Length() == 13 ? true : false;
	  if (request.sdrplay.mode) {
	  	request.sdrplay.buffer_size = args[11]->Uint32Value();
	  	request.sdrplay.buffer_count = args[12]->Uint32Value();
	  }
	  uv_cond_init(&isStreamRunning);
	  uv_mutex_init(&request.async_lock);
	  uv_async_init(
		    uv_default_loop()
		  , request.async
		  , &HandleStreamCallback
	  );
	  thread* streamWorker = new thread(startStream,streamParams, request);
	  request.streamWorker = streamWorker;
	  streamWorker->detach();
	  args.GetReturnValue().SetNull();
	}

	void StreamUninit(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() > 0) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "No argument needed.")));
	    return;
	  }  
	  //streamWorker->attach();
	  mir_sdr_ErrT error = mir_sdr_StreamUninit();
	  if (error!= mir_sdr_Success) {
		cout << "Error while deinitializing stream. Error : " << error << "\r\n";			
		return;
	  }
	  uv_cond_signal(&isStreamRunning);
	  uv_cond_destroy(&isStreamRunning);
	}

	void SetRf(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 3) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetRf(args[0]->NumberValue(), args[1]->Int32Value(), args[2]->Int32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to SetRf stream.")));
	    return;
	  }
	}

	void SetFs(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 4) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetFs(args[0]->NumberValue(), args[1]->Int32Value(), args[2]->Int32Value(), args[3]->Int32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to SetFs stream.")));
	    return;
	  }
	}

	void ResetUpdateFlags(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 3) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_ResetUpdateFlags(args[0]->Int32Value(), args[1]->Int32Value(), args[2]->Int32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to ResetUpdateFlags stream.")));
	    return;
	  }
	}

	void SetSyncUpdateSampleNum(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetSyncUpdateSampleNum(args[0]->Uint32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to SetSyncUpdateSampleNum stream.")));
	    return;
	  }
	}

	void SetSyncUpdatePeriod(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 1) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_SetSyncUpdatePeriod(args[0]->Uint32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to SetSyncUpdatePeriod stream.")));
	    return;
	  }
	}

	void DecimateControl(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 3) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong argument count.")));
	    return;
	  }  
	  mir_sdr_ErrT error = mir_sdr_DecimateControl(args[0]->Uint32Value(), args[1]->Uint32Value(), args[2]->Uint32Value());
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to DecimateControl stream.")));
	    return;
	  }
	}

	void ReInit(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Nan::HandleScope scope;
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 11) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Wrong arguments count.")));
	    return;
	  }  
	  // Initialize parameters
	  int gRdB = args[0]->Uint32Value();
	  double fsMHz = args[1]->NumberValue();
	  double rfMHz = args[2]->NumberValue();
	  mir_sdr_Bw_MHzT bwType = static_cast<mir_sdr_Bw_MHzT>(args[3]->Uint32Value());
	  mir_sdr_If_kHzT ifType = static_cast<mir_sdr_If_kHzT>(args[4]->Uint32Value());
	  mir_sdr_LoModeT loMode = static_cast<mir_sdr_LoModeT>(args[5]->Uint32Value());
	  int LNAState = args[6]->Uint32Value();
	  int gRdBsystem = args[7]->Uint32Value();
	  mir_sdr_SetGrModeT setGrMode = static_cast<mir_sdr_SetGrModeT>(args[8]->Uint32Value());
	  int samplesPerPacket = args[9]->Uint32Value();
	  mir_sdr_ReasonForReinitT reasonForReinit = static_cast<mir_sdr_ReasonForReinitT>(args[10]->Uint32Value());
	  mir_sdr_ErrT error = mir_sdr_Reinit(&gRdB, fsMHz, rfMHz, bwType, ifType, loMode, LNAState, &gRdBsystem, setGrMode, &samplesPerPacket, reasonForReinit);
	  // if (error!= mir_sdr_Success) {
	  //   isolate->ThrowException(Exception::TypeError(
	  //       String::NewFromUtf8(isolate, "Unable to ReInit stream.")));
	  //   return;
	  // }
	  args.GetReturnValue().Set(error);	  
	}

}
