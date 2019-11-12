const chai = require('chai');
const mlog = require('mocha-logger');
const expect = chai.expect; // we are using the "expect" style of Chai
const should = chai.should; // we are using the "should" style of Chai
const sdrplay = require('../index.js');

describe('SetDeviceIdx', () => {
	  it('SetDeviceIdx() should throw an error "Wrong arguments count."', () => {
		      expect(() => sdrplay.SetDeviceIdx()).to.throw('Wrong arguments count.'); //
		    });
	  it('SetDeviceIdx(0) should work', function() {
	  		  let devices = sdrplay.GetDevices(1);
	  		  expect(devices.length).to.equal(1);
		      expect(sdrplay.SetDeviceIdx(0)).to.equal(sdrplay.ErrT.mir_sdr_Success);
	  		  sdrplay.ReleaseDeviceIdx();
		    });
	  it('SetDeviceIdx(99) should throw an error "Unable to set device index."', function() {
		      expect(sdrplay.SetDeviceIdx(99)).to.equal(sdrplay.ErrT.mir_sdr_HwError);
		    });
});

describe('ReleaseDeviceIdx', () => {
	  it('ReleaseDeviceIdx() should free device', function() {
	  		  let devices = sdrplay.GetDevices(4);
	  		  expect(devices.length).to.equal(1);
	  		  expect(devices[0]).to.have.property('devAvail').to.equal(1);
		      expect(sdrplay.SetDeviceIdx(0)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		      devices = sdrplay.GetDevices(4);
	  		  expect(devices.length).to.equal(1);
	  		  expect(devices[0]).to.have.property('devAvail').to.equal(0);
	  		  sdrplay.ReleaseDeviceIdx();
		      devices = sdrplay.GetDevices(4);
	  		  expect(devices.length).to.equal(1);
	  		  expect(devices[0]).to.have.property('devAvail').to.equal(1);
		    });
});

describe('Settings Without Initialization', () => {
	  it('SetLoMode(mir_sdr_LO_120MHz) should work', function() {
	  		  expect(sdrplay.SetLoMode(sdrplay.LoModeT.mir_sdr_LO_120MHz)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
	});

describe('Settings With Initialization', () => {
	  before(() => {
	  		  // sdrplay.DebugEnable(1);
	  		  return new Promise( (resolve) => {
	  		  sdrplay.SetDeviceIdx(0);
	  		  let done = false;
	  		  sdrplay.StreamInit(38, 2.100000, 105.0,
					sdrplay.Bw_MHzT.mir_sdr_BW_1_536,
					sdrplay.If_kHzT.mir_sdr_IF_Zero,
					0,
					28,
					sdrplay.SetGrModeT.mir_sdr_USE_SET_GR,
					128, 
					(iqbuffer,bufferSize,firstSampleNum,grChanged,rfChanged,fsChanged,numSamples, reset) => {
						resolve();
						//console.log("firstSampleNum="+firstSampleNum+" xi="+xi.length+" numSamples="+numSamples);
						//done = true;
					}, (gRdB, lnagRdB) => {
						mlog.log("gRdb="+gRdB+" lnagRdB="+lnagRdB);
					},
					16*16384, // Buffer size
					15 ); // Buffer count		
			  });			
	  		});
	  after(() => {
	  		  sdrplay.StreamUninit();
	  		  sdrplay.ReleaseDeviceIdx();
	  		});
	  it('SetDcMode(1,1) should work', function() {
	  		  expect(sdrplay.SetDcMode(1,1)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
	  it('SetDcMode(9,1) should return an out of range error', function() {
	  		  expect(sdrplay.SetDcMode(9,1)).to.equal(sdrplay.ErrT.mir_sdr_OutOfRange);
		    });
	  it('SetDcTrackTime(30) should work', function() {
	  		  expect(sdrplay.SetDcTrackTime(30)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
	  it('DCoffsetIQimbalanceControl(1,1) should work', function() {
	  		  expect(sdrplay.DCoffsetIQimbalanceControl(1,1)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
	  it('SetTransferMode(mir_sdr_BULK) should work', function() {
	  		  expect(sdrplay.SetTransferMode(sdrplay.TransferModeT.mir_sdr_BULK)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
});