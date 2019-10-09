/**********
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********/
/*
    A C++ program to parse DJI's ".txt" log files (recorded by the "DJI Go 4" app).
    Version 2019-02-08

    Copyright (c) 2019 Live Networks, Inc.  All rights reserved.
    For the latest version of this program (and more information), visit http://djilogs.live555.com
    For the latest version of this program (and more information), visit http://djilogs.live555.com

    Setting up our 'interpretation tables', each mapping integers to strings.
    Implementation.
*/

#include "FieldDatabase.hh"

void FieldDatabase::initializeInterpretationTables() {
  InterpretationTable* it;

  ////////// OSD.flycState ////////
  it = newInterpretationTable("OSD.flycState", "Other");
  it->add(0, "Manual");
  it->add(1, "Atti");
  it->add(2, "Atti_CL");
  it->add(3, "Atti_Hover");
  it->add(4, "Hover");
  it->add(5, "GPS_Blake");
  it->add(6, "GPS_Atti");
  it->add(7, "GPS_CL");
  it->add(8, "GPS_HomeLock");
  it->add(9, "GPS_HotPoint");
  it->add(10, "AssistedTakeoff");
  it->add(11, "AutoTakeoff");
  it->add(12, "AutoLanding");
  it->add(13, "AttiLanding");
  it->add(14, "NaviGo");
  it->add(15, "GoHome");
  it->add(16, "ClickGo");
  it->add(17, "Joystick");
  it->add(18, "GPS_Atti_Wristband");
  it->add(19, "Cinematic");
  it->add(23, "Atti_Limited");
  it->add(24, "GPS_Atti_Limited");
  it->add(25, "NaviMissionFollow");
  it->add(26, "NaviSubMode_Tracking");
  it->add(27, "NaviSubMode_Pointing");
  it->add(28, "PANO");
  it->add(29, "Farming");
  it->add(30, "FPV");
  it->add(31, "Sport");
  it->add(32, "Novice");
  it->add(33, "ForceLanding");
  it->add(35, "TerrainTracking");
  it->add(36, "NaviAdvGoHome");
  it->add(37, "NaviAdvLanding");
  it->add(38, "TripodGPS");
  it->add(39, "TrackHeadlock");
  it->add(41, "EngineStart");
  it->add(43, "GentleGPS");

  ////////// OSD.flycCommand ////////
  it = newInterpretationTable("OSD.flycCommand", "Other");
  it->add(1, "AutoFly");
  it->add(2, "AutoLanding");
  it->add(3, "HomePointNow");
  it->add(4, "HomePointHot");
  it->add(5, "HomePointLock");
  it->add(6, "GoHome");
  it->add(7, "StartMotor");
  it->add(8, "StopMotor");
  it->add(9, "Calibration");
  it->add(10, "DeformProtecClose");
  it->add(11, "DeformProtecOpen");
  it->add(12, "DropGoHome");
  it->add(13, "DropTakeOff");
  it->add(14, "DropLanding");
  it->add(15, "DynamicHomePointOpen");
  it->add(16, "DynamicHomePointClose");
  it->add(17, "FollowFunctionOpen");
  it->add(18, "FollowFunctionClose");
  it->add(19, "IOCOpen");
  it->add(20, "IOCClose");
  it->add(21, "DropCalibration");
  it->add(22, "PackMode");
  it->add(23, "UnPackMode");
  it->add(24, "EnterManualMode");
  it->add(25, "StopDeform");
  it->add(28, "DownDeform");
  it->add(29, "UpDeform");
  it->add(30, "ForceLanding");
  it->add(31, "ForceLanding2");

  ////////// OSD.groundOrSky ////////
  it = newInterpretationTable("OSD.groundOrSky", "Unknown");
  it->add(0, "Ground");
  it->add(1, "Ground");
  it->add(2, "Sky");
  it->add(3, "Sky");

  ////////// OSD.goHomeStatus ////////
  it = newInterpretationTable("OSD.goHomeStatus", "Other");
  it->add(0, "Standby");
  it->add(1, "Preascending");
  it->add(2, "Align");
  it->add(3, "Ascending");
  it->add(4, "Cruise");
  it->add(5, "Braking");
  it->add(6, "Bypassing");

  ////////// OSD.batteryType ////////
  it = newInterpretationTable("OSD.batteryType", "Unknown");
  it->add(1, "NonSmart");
  it->add(2, "Smart");

  ////////// OSD.flightAction ////////
  it = newInterpretationTable("OSD.flightAction", "Unknown");
  it->add(0, "None");
  it->add(1, "WarningPowerGoHome");
  it->add(2, "WarningPowerLanding");
  it->add(3, "SmartPowerGoHome");
  it->add(4, "SmartPowerLanding");
  it->add(5, "LowVoltageLanding");
  it->add(6, "LowVoltageGoHome");
  it->add(7, "SeriousLowVoltageLanding");
  it->add(8, "RC_OnekeyGoHome");
  it->add(9, "RC_AssistantTakeoff");
  it->add(10, "RC_AutoTakeoff");
  it->add(11, "RC_AutoLanding");
  it->add(12, "AppAutoGoHome");
  it->add(13, "AppAutoLanding");
  it->add(14, "AppAutoTakeoff");
  it->add(15, "OutOfControlGoHome");
  it->add(16, "ApiAutoTakeoff");
  it->add(17, "ApiAutoLanding");
  it->add(18, "ApiAutoGoHome");
  it->add(19, "AvoidGroundLanding");
  it->add(20, "AirportAvoidLanding");
  it->add(21, "TooCloseGoHomeLanding");
  it->add(22, "TooFarGoHomeLanding");
  it->add(23, "App_WP_Mission");
  it->add(24, "WP_AutoTakeoff");
  it->add(25, "GoHomeAvoid");
  it->add(26, "pGoHomeFinish");
  it->add(27, "VertLowLimitLanding");
  it->add(28, "BatteryForceLanding");
  it->add(29, "MC_ProtectGoHome");
  it->add(30, "MotorblockLanding");
  it->add(31, "AppRequestForceLanding");
  it->add(32, "FakeBatteryLanding");
  it->add(33, "RTH_ComingObstacleLanding");
  it->add(34, "IMUErrorRTH");

  ////////// OSD.motorStartFailedCause ////////
  it = newInterpretationTable("OSD.motorStartFailedCause", "Other");
  it->add(0, "None");
  it->add(1, "CompassError");
  it->add(2, "AssistantProtected");
  it->add(3, "DeviceLocked");
  it->add(4, "DistanceLimit");
  it->add(5, "IMUNeedCalibration");
  it->add(6, "IMUSNError");
  it->add(7, "IMUWarning");
  it->add(8, "CompassCalibrating");
  it->add(9, "AttiError");
  it->add(10, "NoviceProtected");
  it->add(11, "BatteryCellError");
  it->add(12, "BatteryCommuniteError");
  it->add(13, "SeriousLowVoltage");
  it->add(14, "SeriousLowPower");
  it->add(15, "LowVoltage");
  it->add(16, "TempureVolLow");
  it->add(17, "SmartLowToLand");
  it->add(18, "BatteryNotReady");
  it->add(19, "SimulatorMode");
  it->add(20, "PackMode");
  it->add(21, "AttitudeAbnormal");
  it->add(22, "UnActive");
  it->add(23, "FlyForbiddenError");
  it->add(24, "BiasError");
  it->add(25, "EscError");
  it->add(26, "ImuInitError");
  it->add(27, "SystemUpgrade");
  it->add(28, "SimulatorStarted");
  it->add(29, "ImuingError");
  it->add(30, "AttiAngleOver");
  it->add(31, "GyroscopeError");
  it->add(32, "AcceleratorError");
  it->add(33, "CompassFailed");
  it->add(34, "BarometerError");
  it->add(35, "BarometerNegative");
  it->add(36, "CompassBig");
  it->add(37, "GyroscopeBiasBig");
  it->add(38, "AcceleratorBiasBig");
  it->add(39, "CompassNoiseBig");
  it->add(40, "BarometerNoiseBig");
  it->add(41, "InvalidSn");
  it->add(44, "FlashOperating");
  it->add(45, "GPSdisconnect");
  it->add(47, "SDCardException");
  it->add(61, "IMUNoconnection");
  it->add(62, "RCCalibration");
  it->add(63, "RCCalibrationException");
  it->add(64, "RCCalibrationUnfinished");
  it->add(65, "RCCalibrationException2");
  it->add(66, "RCCalibrationException3");
  it->add(67, "AircraftTypeMismatch");
  it->add(68, "FoundUnfinishedModule");
  it->add(70, "CyroAbnormal");
  it->add(71, "BaroAbnormal");
  it->add(72, "CompassAbnormal");
  it->add(73, "GPS_Abnormal");
  it->add(74, "NS_Abnormal");
  it->add(75, "TopologyAbnormal");
  it->add(76, "RC_NeedCali");
  it->add(77, "InvalidFloat");
  it->add(78, "M600_BAT_TOO_LITTLE");
  it->add(79, "M600_BAT_AUTH_ERR");
  it->add(80, "M600_BAT_COMM_ERR");
  it->add(81, "M600_BAT_DIF_VOLT_LARGE_1");
  it->add(82, "M600_BAT_DIF_VOLT_LARGE_2");
  it->add(83, "InvalidVersion");
  it->add(84, "GimbalGyroAbnormal");
  it->add(85, "GimbalESC_PitchNonData");
  it->add(86, "GimbalESC_RollNonData");
  it->add(87, "GimbalESC_YawNonData");
  it->add(88, "GimbalFirmwIsUpdating");
  it->add(89, "GimbalDisorder");
  it->add(90, "GimbalPitchShock");
  it->add(91, "GimbalRollShock");
  it->add(92, "GimbalYawShock");
  it->add(93, "IMUcCalibrationFinished");
  it->add(101, "BattVersionError");
  it->add(102, "RTK_BadSignal");
  it->add(103, "RTK_DeviationError");
  it->add(112, "ESC_Calibrating");
  it->add(113, "GPS_SignInvalid");
  it->add(114, "GimbalIsCalibrating");
  it->add(115, "LockByApp");
  it->add(116, "StartFlyHeightError");
  it->add(117, "ESC_VersionNotMatch");
  it->add(118, "IMU_ORI_NotMatch");
  it->add(119, "StopByApp");
  it->add(120, "CompassIMU_ORI_NotMatch");
  it->add(122, "CompassIMU_ORI_NotMatch");
  it->add(123, "BatteryOverTemperature");
  it->add(124, "BatteryInstallError");
  it->add(125, "BeImpact");

  ////////// OSD.nonGPSCause ////////
  it = newInterpretationTable("OSD.nonGPSCause", "Unknown");
  it->add(0, "Already");
  it->add(1, "Forbid");
  it->add(2, "GpsNumNonEnough");
  it->add(3, "GpsHdopLarge");
  it->add(4, "GpsPositionNonMatch");
  it->add(5, "SpeedErrorLarge");
  it->add(6, "YawErrorLarge");
  it->add(7, "CompassErrorLarge");

  ////////// OSD.droneType ////////
  it = newInterpretationTable("OSD.droneType", "Unknown");
  it->add(1, "Inspire 1");
  it->add(2, "P3 Advanced");
  it->add(3, "P3 Professional");
  it->add(4, "P3 Standard");
  it->add(5, "OpenFrame");
  it->add(6, "AceOne");
  it->add(7, "WKM");
  it->add(8, "Naza");
  it->add(9, "A2");
  it->add(10, "A3");
  it->add(11, "P4");
  it->add(14, "Matrice 600");
  it->add(15, "P3 4K");
  it->add(16, "Mavic");
  it->add(17, "Inspire 2");
  it->add(18, "P4 Professional");
  it->add(20, "N3");
  it->add(21, "Spark");
  it->add(23, "Matrice 600 Pro");
  it->add(24, "Mavic Air");
  it->add(25, "Matrice 200");
  it->add(27, "P4 Advanced");
  it->add(28, "Matrice 210");
  it->add(29, "P3SE");
  it->add(30, "Matrice 210MTK");

  ////////// OSD.imuInitFailReason ////////
  it = newInterpretationTable("OSD.imuInitFailReason", "None");
  it->add(0, "MonitorError");
  it->add(1, "CollectingData");
  it->add(3, "AcceDead");
  it->add(4, "CompassDead");
  it->add(5, "BarometerDead");
  it->add(6, "BarometerNegative");
  it->add(7, "CompassModTooLarge");
  it->add(8, "GyroBiasTooLarge");
  it->add(9, "AcceBiasTooLarge");
  it->add(10, "CompassNoiseTooLarge");
  it->add(11, "BarometerNoiseTooLarge");
  it->add(12, "WaitingMcStationary");
  it->add(13, "AcceMoveTooLarge");
  it->add(14, "McHeaderMoved");
  it->add(15, "McVibrated");

  ////////// OSD.motorFailReason ////////
  it = newInterpretationTable("OSD.motorFailReason", "");
  it->add(94, "TakeoffException");
  it->add(95, "ESC_StallNearGround");
  it->add(96, "ESC_UnbalanceOnGround");
  it->add(97, "ESC_PART_EMPTYOnGround");
  it->add(98, "EngineStartFailed");
  it->add(99, "AutoTakeoffLaunchFailed");
  it->add(100, "RollOverOnGround");

  ////////// OSD.ctrlDevice ////////
  it = newInterpretationTable("OSD.ctrlDevice", "Other");
  it->add(0, "RC");
  it->add(1, "App");
  it->add(2, "OnboardDevice");
  it->add(3, "Camera");

  ////////// GIMBAL.mode ////////
  it = newInterpretationTable("GIMBAL.mode", "Other");
  it->add(0, "YawNoFollow");
  it->add(1, "FPV");
  it->add(2, "YawFollow");

  ////////// SMART_BATTERY.status ////////
  it = newInterpretationTable("SMART_BATTERY.status", "???");
  it->add(0, "None");

  ////////// SMART_BATTERY.goHomeStatus ////////
  it = newInterpretationTable("SMART_BATTERY.goHomeStatus", "Unknown");
  it->add(0, "NonGoHome");
  it->add(1, "GoHome");
  it->add(2, "GoHomeAlready");

  ////////// DEFORM.deformStatus ////////
  it = newInterpretationTable("DEFORM.deformStatus", "Unknown");
  it->add(1, "FoldComplete");
  it->add(2, "Folding");
  it->add(3, "StretchComplete");
  it->add(4, "Stretching");
  it->add(5, "StopDeformation");

  ////////// DEFORM.deformMode ////////
  it = newInterpretationTable("DEFORM.deformMode", "Other");
  it->add(0, "Pack");
  it->add(1, "Protect");
  it->add(2, "Normal");

  ////////// HOME.iocMode ////////
  it = newInterpretationTable("HOME.iocMode", "Other");
  it->add(1, "CourseLock");
  it->add(2, "HomeLock");
  it->add(3, "HotspotSurround");

  ////////// RECOVER.droneType ////////
  it = newInterpretationTable("RECOVER.droneType", "Unknown");
  it->add(1, "Inspire 1");
  it->add(2, "P3 Standard");
  it->add(3, "P3 Advanced");
  it->add(4, "P3 Professional");
  it->add(5, "OSMO");
  it->add(6, "Matrice 100");
  it->add(7, "P4");
  it->add(8, "LB2");
  it->add(9, "Inspire 1 Pro");
  it->add(10, "A3");
  it->add(11, "Matrice 600");
  it->add(12, "P3 4K");
  it->add(13, "Mavic Pro");
  it->add(14, "Zenmuse XT");
  it->add(15, "Inspire 1 RAW");
  it->add(16, "A2");
  it->add(17, "Inspire 2");
  it->add(18, "OSMO Pro");
  it->add(19, "OSMO Raw");
  it->add(20, "SMO+");
  it->add(21, "Mavic");
  it->add(22, "OSMO Mobile");
  it->add(23, "OrangeCV600");
  it->add(24, "P4 Professional");
  it->add(25, "N3 FC");
  it->add(26, "Spark");
  it->add(27, "Matrice 600 Pro");
  it->add(28, "P4 Advanced");
  it->add(30, "AG405");
  it->add(31, "Matrice 200");
  it->add(33, "Matrice 210");
  it->add(34, "Matrice 210RTK");
  it->add(38, "Mavic Air");

  ////////// RECOVER.appType ////////

  it = newInterpretationTable("RECOVER.appType", "Unknown");
  it->add(1, "iOS");
  it->add(2, "Android");


  ////////// DETAILS.appType ////////

  it = newInterpretationTable("DETAILS.appType", "Unknown");
  it->add(1, "iOS");
  it->add(2, "Android");
}

#if 0

#endif
