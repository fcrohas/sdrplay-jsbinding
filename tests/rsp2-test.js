const chai = require('chai');
const mlog = require('mocha-logger');
const expect = chai.expect; // we are using the "expect" style of Chai
const should = chai.should; // we are using the "should" style of Chai
const sdrplay = require('../index.js');

describe('RSP2 Settings Without Initialization', () => {
	  it('RSPII_ExternalReferenceControl(0)', function() {
	  		  sdrplay.SetDeviceIdx(0);
	  		  expect(sdrplay.RSPII_ExternalReferenceControl(0)).to.equal(sdrplay.ErrT.mir_sdr_Success);
	  		  sdrplay.ReleaseDeviceIdx();
		    });
	});

describe('RSP2 Settings With Initialization', () => {
	  before(() => {
	  		  // sdrplay.DebugEnable(1);
	  		  return new Promise( (resolve) => {
	  		  sdrplay.SetDeviceIdx(0);
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
	  it('RSPII_AntennaControl(mir_sdr_RSPII_ANTENNA_A) should work', function() {
	  		  expect(sdrplay.RSPII_AntennaControl(sdrplay.RSPII_AntennaSelectT.mir_sdr_RSPII_ANTENNA_A)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
	  it('RSPII_BiasTControl(0) should work', function() {
	  		  expect(sdrplay.RSPII_BiasTControl(0)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
	  it('RSPII_RfNotchEnable(1) should work', function() {
	  		  expect(sdrplay.RSPII_RfNotchEnable(1)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
	  it('AmPortSelect(0) should work', function() {
	  		  expect(sdrplay.AmPortSelect(0)).to.equal(sdrplay.ErrT.mir_sdr_Success);
		    });
});