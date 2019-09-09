const SDRPlay = require('./build/Release/sdrplay');

const ErrT = {
	mir_sdr_Success : 0,
	mir_sdr_Fail : 1,
	mir_sdr_InvalidParam : 2,
	mir_sdr_OutOfRange : 3,
	mir_sdr_GainUpdateError : 4,
	mir_sdr_RfUpdateError : 5,
	mir_sdr_FsUpdateError : 6,
	mir_sdr_HwError : 7,
	mir_sdr_AliasingError : 8,
	mir_sdr_AlreadyInitialised : 9,
	mir_sdr_NotInitialised : 10,
	mir_sdr_NotEnabled : 11,
	mir_sdr_HwVerError : 12,
	mir_sdr_OutOfMemError : 13
};

const Bw_MHzT = {
	mir_sdr_BW_0_200 : 200,
	mir_sdr_BW_0_300 : 300,
	mir_sdr_BW_0_600 : 600,
	mir_sdr_BW_1_536 : 1536,
	mir_sdr_BW_5_000 : 5000,
	mir_sdr_BW_6_000 : 6000,
	mir_sdr_BW_7_000 : 7000,
	mir_sdr_BW_8_000 : 8000	
};

const If_kHzT =  {
	mir_sdr_IF_Zero : 0,
	mir_sdr_IF_0_450 : 450,
	mir_sdr_IF_1_620 : 1620,
	mir_sdr_IF_2_048 : 2048	
};

const TransferModeT = {
	mir_sdr_ISOCH : 0,
	mir_sdr_BULK : 1
}

const ReasonForReinitT = {
	mir_sdr_CHANGE_NONE : 0x00,
	mir_sdr_CHANGE_GR : 0x01,
	mir_sdr_CHANGE_FS_FREQ : 0x02,
	mir_sdr_CHANGE_RF_FREQ : 0x04,
	mir_sdr_CHANGE_BW_TYPE : 0x08,
	mir_sdr_CHANGE_IF_TYPE : 0x10,
	mir_sdr_CHANGE_LO_MODE : 0x20,
	mir_sdr_CHANGE_AM_PORT : 0x40
};

const LoModeT = {
	mir_sdr_LO_Undefined : 0,
	mir_sdr_LO_Auto : 1, // 1st LO is automatically selected to provide appropriate coverage
	// across all tuner frequency ranges
	mir_sdr_LO_120MHz : 2, // 1st LO is set to 120MHz (coverage gap between 370MHZ and 420MHz)
	mir_sdr_LO_144MHz : 3, // 1st LO is set to 144MHz (coverage gap between 250MHZ and 255MHz
	// and between 400MHz and 420MHz)
	mir_sdr_LO_168MHz : 4 // 1st LO is set to 168MHz (coverage gap between 250MHZ and 265MHz)
};

const SetGrModeT = {
	mir_sdr_USE_SET_GR : 0,
	mir_sdr_USE_SET_GR_ALT_MODE : 1,
	mir_sdr_USE_RSP_SET_GR : 2
};

const RSPII_BandT = {
	mir_sdr_RSPII_BAND_UNKNOWN : 0,
	mir_sdr_RSPII_BAND_AM_LO : 1,
	mir_sdr_RSPII_BAND_AM_MID : 2,
	mir_sdr_RSPII_BAND_AM_HI : 3,
	mir_sdr_RSPII_BAND_VHF : 4,
	mir_sdr_RSPII_BAND_3 : 5,
	mir_sdr_RSPII_BAND_X_LO : 6,
	mir_sdr_RSPII_BAND_X_MID : 7,
	mir_sdr_RSPII_BAND_X_HI : 8,
	mir_sdr_RSPII_BAND_4_5 : 9,
	mir_sdr_RSPII_BAND_L : 10	
}

const RSPII_AntennaSelectT = {
	mir_sdr_RSPII_ANTENNA_A : 5,
	mir_sdr_RSPII_ANTENNA_B : 6
};

const AgcControlT = {
	mir_sdr_AGC_DISABLE : 0,
	mir_sdr_AGC_100HZ : 1,
	mir_sdr_AGC_50HZ : 2,
	mir_sdr_AGC_5HZ : 3
};

const GainMessageIdT = {
	mir_sdr_GAIN_MESSAGE_START_ID : 0x80000000,
	mir_sdr_ADC_OVERLOAD_DETECTED : this.mir_sdr_GAIN_MESSAGE_START_ID + 1,
	mir_sdr_ADC_OVERLOAD_CORRECTED : this.mir_sdr_GAIN_MESSAGE_START_ID + 2
};

const MinGainReductionT = {
	mir_sdr_EXTENDED_MIN_GR : 0, // 0 to 59
	mir_sdr_NORMAL_MIN_GR : 20 // 20 to 59
};

SDRPlay.ErrT = ErrT;
SDRPlay.Bw_MHzT = Bw_MHzT;
SDRPlay.If_kHzT = If_kHzT;
SDRPlay.TransferModeT = TransferModeT;
SDRPlay.ReasonForReinitT = ReasonForReinitT;
SDRPlay.LoModeT = LoModeT;
SDRPlay.SetGrModeT = SetGrModeT;
SDRPlay.RSPII_BandT = RSPII_BandT;
SDRPlay.RSPII_AntennaSelectT = RSPII_AntennaSelectT;
SDRPlay.AgcControlT = AgcControlT;
SDRPlay.GainMessageIdT = GainMessageIdT;
SDRPlay.MinGainReductionT = MinGainReductionT;

module.exports = SDRPlay;