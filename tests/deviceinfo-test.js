const chai = require('chai');
const mlog = require('mocha-logger');
const expect = chai.expect; // we are using the "expect" style of Chai
const should = chai.should; // we are using the "should" style of Chai
const sdrplay = require('../index.js');

describe('ApiInfo', function() {
	  it('ApiInfo() should return a valid version', function() {
			  const apiVersion = new String(sdrplay.ApiVersion());
			  const isFilled = apiVersion !== null;
			  const versions = apiVersion.split('.');
		      expect(versions[0]).to.equal('2');
		    });
});

describe('GetDevices', function() {
	  it('GetDevices() should return a valid device count', function() {
		      const devices = sdrplay.GetDevices(4);
		      expect(devices.length).to.equal(1);
		    });
	  it('GetDevices() should return no device count', function() {
		      const devices = sdrplay.GetDevices(0);
		      expect(devices.length).to.equal(0);
		    });
	  it('GetDevices() should return device information', function() {
		      const devices = sdrplay.GetDevices(1);
		      for (var i=0; i< devices.length;i++) {
				expect(devices[i]).to.have.property('SerNo').with.lengthOf(10);
				expect(devices[i]).to.have.property('DevNm').be.a('string');
				expect(devices[i]).to.have.property('hwVer').be.a('number');
				expect(devices[i]).to.have.property('devAvail').be.a('number');
				mlog.log('Serial No is ', devices[i].SerNo);
				mlog.log('DevNm is ', devices[i].DevNm);
				mlog.log('Hardware version is ', devices[i].hwVer);
				mlog.log('Device available is ', devices[i].devAvail);
			  }	
		    });
	  it('GetHwVersion() should return device version', function() {
		      const devices = sdrplay.GetDevices(1);
			  expect(sdrplay.GetHwVersion()).be.a('number');
			  mlog.log('Harware version is ', sdrplay.GetHwVersion());
		    });
});
