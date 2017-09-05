const sdrplay = require('../index.js');
// Node debug
sdrplay.DebugEnable(0);
// Get API version
console.log("API version : "+sdrplay.ApiVersion());
var devices = sdrplay.GetDevices(4);
// Check first device
for (var i=0; i< devices.length;i++) {
	console.log("Serial No : "+devices[i].SerNo+ "\nDevice number : "+devices[i].DevNm+"\nHW version : "+devices[i].hwVer+"\nAvailable : "+devices[i].devAvail);
}
// Check hardware version
console.log("Single method HW version : "+sdrplay.GetHwVersion());
// Claim first device
sdrplay.SetDeviceIdx(0);
// Start stream handle
sdrplay.StreamInit(38, 2.100000, 105.0,
	sdrplay.Bw_MHzT.mir_sdr_BW_1_536,
	sdrplay.If_kHzT.mir_sdr_IF_Zero,
	0,
	28,
	sdrplay.SetGrModeT.mir_sdr_USE_SET_GR,
	128, 
	function(xi,xq,firstSampleNum,grChanged,rfChanged,fsChanged,numSamples, reset) {
		// console.log("firstSampleNum="+firstSampleNum+" xi="+xi.length+" numSamples="+numSamples);
	}, function(gRdB, lnagRdB) {
		console.log("gRdb="+gRdB+" lnagRdB="+lnagRdB);
	}
);
// setTimeout(function() {
// 	sdrplay.SetGr(38,1,0);
// }, 2000);

setTimeout(function() {
	console.log("StreamUninit");
	sdrplay.StreamUninit();
	// Release device
	console.log("ReleaseDevice");
	sdrplay.ReleaseDeviceIdx();
    console.log('program exit...');
}, 5000);// stop stream
