const sdrplay = require('./build/Debug/sdrplay');
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
sdrplay.StreamInit(38, 2.100000, 105.0,1536,0,0,28,0,128, function(xi,xq,firstSampleNum,grChanged,rfChanged,fsChanged,numSamples, reset) {
	console.log("firstSampleNum="+firstSampleNum+" xi="+xi.length+" numSamples="+numSamples);
}, function(gRdB, lnagRdB) {
	console.log("gRdb="+gRdB+" lnagRdB="+lnagRdB);
});

setTimeout(function() {
	sdrplay.StreamUninit();
	// Release device
	sdrplay.ReleaseDeviceIdx();
    console.log('program exit...');
}, 3000);// stop stream
