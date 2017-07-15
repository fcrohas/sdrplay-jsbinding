#include "sdrplay.h"

namespace sdrplay {

  NAN_MODULE_INIT(init) {
    SetMethod(target, "ApiVersion", ApiVersion);
    SetMethod(target, "StreamUninit", StreamUninit);
    SetMethod(target, "StreamInit", StreamInit);
    SetMethod(target, "DebugEnable", DebugEnable);
    SetMethod(target, "SetPpm", SetPpm);
    SetMethod(target, "SetDcTrackTime", SetDcTrackTime);
    SetMethod(target, "SetDcMode", SetDcMode);
    SetMethod(target, "DCoffsetIQimbalanceControl", DCoffsetIQimbalanceControl);
    SetMethod(target, "SetLoMode", SetLoMode);
    SetMethod(target, "SetTransferMode", SetTransferMode);
    
    SetMethod(target, "GetHwVersion", GetHwVersion);
    SetMethod(target, "GetDevices", GetDevices);

    SetMethod(target, "ReleaseDeviceIdx", ReleaseDeviceIdx);
    SetMethod(target, "SetDeviceIdx", SetDeviceIdx);
    // Specific RSP 2
    SetMethod(target, "RSPII_AntennaControl", RSPII_AntennaControl);
    SetMethod(target, "RSPII_ExternalReferenceControl", RSPII_ExternalReferenceControl);
    SetMethod(target, "RSPII_BiasTControl", RSPII_BiasTControl);
    SetMethod(target, "RSPII_RfNotchEnable", RSPII_RfNotchEnable);
    SetMethod(target, "AmPortSelect", AmPortSelect);
    
    
  }

  NODE_MODULE(sdrplay, init)

}  
