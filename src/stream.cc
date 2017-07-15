#include "sdrplay.h"

namespace sdrplay {

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
	  sdrplay_t *sdrplay;
	  Callback* streamCallback;
	  Callback* gainCallback; 
	  bool fromGainCallback = false;
	  bool fromStreamCallback = false;
	  thread* streamWorker;
	};

	static bool isStreamRunning = false;

	void sdrplay_gainCallback(unsigned int gRdB,unsigned int lnaGRdB, void *cbContext) {
		// cout << "gRdB=" << gRdB << " lnaGRdB=" << lnaGRdB;
		request_t *request = (request_t*)cbContext;
		int err = uv_mutex_trylock(&request->async_lock);  
		if (err) {
			// Mutex not initliaed so initialized it in this thread
			uv_mutex_init(&request->async_lock);
			uv_mutex_lock(&request->async_lock);
		}
		sdrplay_t * SDRPlay = new sdrplay_t();
		SDRPlay->gRdB = gRdB;
		SDRPlay->lnaGRdB = lnaGRdB;
		request->fromGainCallback = true;
		request->fromStreamCallback = false;
		request->sdrplay = SDRPlay;
		request->async->data =  request;
		uv_mutex_unlock(&request->async_lock);
		// cout << "Calling progress callback !! with numSamples=" << numSamples << "\r\n";  
		uv_async_send(request->async);
	}


	void sdrplay_streamCallback(short *xi, short *xq, unsigned int firstSampleNum,int grChanged, int rfChanged, int fsChanged, unsigned int numSamples, unsigned int reset,void *cbContext) {
	  if (numSamples == 0) {
	    return;
	  }
	  // cout << "sdrplay_streamCallback\r\n";
	  request_t *request = (request_t*)cbContext;
	  int err = uv_mutex_trylock(&request->async_lock);  
	  if (err) {
	  	// Mutex not initliaed so initialized it in this thread
	  	uv_mutex_init(&request->async_lock);
	  	uv_mutex_lock(&request->async_lock);
	  }
	  sdrplay_t * SDRPlay = new sdrplay_t();
	  // Fill data struct
	  SDRPlay->grChanged = grChanged;
	  SDRPlay->rfChanged = rfChanged;
	  SDRPlay->fsChanged = fsChanged;
	  SDRPlay->numSamples = numSamples;
	  SDRPlay->firstSampleNum = firstSampleNum;
	  SDRPlay->reset = reset;
	  // Create data struct
	  SDRPlay->xi = new short[numSamples];
	  SDRPlay->xq = new short[numSamples];
	  // Copy data struct
	  memcpy(SDRPlay->xi, xi, numSamples*sizeof(short));
	  memcpy(SDRPlay->xq, xq, numSamples*sizeof(short));
	  request->fromStreamCallback = true;
	  request->fromGainCallback = false;
	  request->sdrplay = SDRPlay;
	  request->async->data =  request;
	  uv_mutex_unlock(&request->async_lock);
	   // cout << "Calling progress callback !! with numSamples=" << numSamples << "\r\n";  
	  uv_async_send(request->async);
	  // progress.Signal();
	  // cout << "Done !!\r\n";
	}


	NAUV_WORK_CB(HandleStreamCallback) {
		// cout << "Filling javascript parameters !!\r\n";
		request_t *request = (request_t*)async->data;
		sdrplay_t *SDRPlay = request->sdrplay;
		Nan::HandleScope scope;
		if (request->fromGainCallback) {
			const unsigned argc = 2;
			Local<Value> argv[] = { 
					  New<v8::Integer>( SDRPlay->gRdB),
					  New<v8::Integer>( SDRPlay->lnaGRdB)}; 
			// cout << "Calling javascript gain callback !!\r\n";
			request->gainCallback->Call(argc, argv);

		} else if (request->fromStreamCallback) {
			const unsigned argc = 8;
			Local<Value> argv[] = { CopyBuffer((char*)SDRPlay->xi, SDRPlay->numSamples).ToLocalChecked(),  
					  CopyBuffer((char*)SDRPlay->xq, SDRPlay->numSamples).ToLocalChecked(),
					  New<v8::Integer>( SDRPlay->firstSampleNum),
					  New<v8::Integer>( SDRPlay->grChanged), 
					  New<v8::Integer>( SDRPlay->rfChanged), 
					  New<v8::Integer>( SDRPlay->fsChanged), 
					  New<v8::Integer>( SDRPlay->numSamples),
					  New<v8::Integer>( SDRPlay->reset)}; 
			// cout << "Calling javascript stream callback !!\r\n";
			request->streamCallback->Call(argc, argv);
			// cout << "Freeing memory\r\n";
			// uv_mutex_lock(&request->async_lock);
			// delete[] SDRPlay->xi;
			// delete[] SDRPlay->xq;
			// delete SDRPlay;
			// uv_mutex_unlock(&request->async_lock);
		}
	}

	void startStream (stream_params_t streamParams, request_t request) {
		// cout << "Execute\r\n";
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
		// Keep running thread
		while(isStreamRunning) {
			// Pause more time
			Sleep(10000);
		}
		// terminate();
		delete request.streamWorker;
	}

	void StreamInit(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Nan::HandleScope scope;
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() != 11) {
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
	  // Create thread

	  // Context
	  request.streamCallback = new Nan::Callback(args[9].As<Function>());
	  request.gainCallback = new Nan::Callback(args[10].As<Function>());
	  isStreamRunning = true;	  
	  uv_async_init(
		    uv_default_loop()
		  , request.async
		  , &HandleStreamCallback
	  );
	  thread* streamWorker = new thread(startStream,streamParams, request);
	  request.streamWorker = streamWorker;
	  streamWorker->detach();
	}

	void StreamUninit(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	  Isolate* isolate = args.GetIsolate();
	  if (args.Length() > 0) {
	    // Throw an Error that is passed back to JavaScript
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "No argument needed.")));
	    return;
	  }  
	  isStreamRunning = false;
	  mir_sdr_ErrT error = mir_sdr_StreamUninit();
	  if (error!= mir_sdr_Success) {
	    isolate->ThrowException(Exception::TypeError(
	        String::NewFromUtf8(isolate, "Unable to deinitialize stream.")));
	    return;
	  }
	}
}