#include "sdrplay.h"

namespace sdrplay {

  NAN_MODULE_INIT(init) {
    // Stream
    SetMethod(target, "StreamUninit", StreamUninit);
    SetMethod(target, "StreamInit", StreamInit);
    SetMethod(target, "SetRf", SetRf);
    SetMethod(target, "SetFs", SetFs);
    SetMethod(target, "ResetUpdateFlags", ResetUpdateFlags);
    SetMethod(target, "DecimateControl", DecimateControl);
    SetMethod(target, "SetSyncUpdateSampleNum", SetSyncUpdateSampleNum);
    SetMethod(target, "SetSyncUpdatePeriod", SetSyncUpdatePeriod);
    SetMethod(target, "ReInit", ReInit);
    // Settings
    SetMethod(target, "DebugEnable", DebugEnable);
    SetMethod(target, "SetPpm", SetPpm);
    SetMethod(target, "SetDcTrackTime", SetDcTrackTime);
    SetMethod(target, "SetDcMode", SetDcMode);
    SetMethod(target, "DCoffsetIQimbalanceControl", DCoffsetIQimbalanceControl);
    SetMethod(target, "SetLoMode", SetLoMode);
    SetMethod(target, "SetTransferMode", SetTransferMode);
    // device check
    SetMethod(target, "ApiVersion", ApiVersion);    
    SetMethod(target, "GetHwVersion", GetHwVersion);
    SetMethod(target, "GetDevices", GetDevices);
    // Device
    SetMethod(target, "ReleaseDeviceIdx", ReleaseDeviceIdx);
    SetMethod(target, "SetDeviceIdx", SetDeviceIdx);
    // Specific RSP 2
    SetMethod(target, "RSPII_AntennaControl", RSPII_AntennaControl);
    SetMethod(target, "RSPII_ExternalReferenceControl", RSPII_ExternalReferenceControl);
    SetMethod(target, "RSPII_BiasTControl", RSPII_BiasTControl);
    SetMethod(target, "RSPII_RfNotchEnable", RSPII_RfNotchEnable);
    SetMethod(target, "AmPortSelect", AmPortSelect);
    // Gain
    SetMethod(target, "SetGr", SetGr);
    SetMethod(target, "SetGrParams", SetGrParams);
    SetMethod(target, "RSP_SetGr", RSP_SetGr);
    SetMethod(target, "RSP_SetGrLimits", RSP_SetGrLimits);
    
    SetMethod(target, "AgcControl", AgcControl);
    SetMethod(target, "GetCurrentGain", GetCurrentGain);
  }

  NODE_MODULE(sdrplay, init)

}  
