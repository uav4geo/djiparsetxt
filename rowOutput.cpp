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

    Outputting a row of data from the table.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"
#include <stdio.h>

#define separator ","
#define o_(label,nFrac) if (outputColumnLabels) printf(label); else fFieldDatabase->outputField(label, nFrac)
#define oInterpreted_(label,interpretedLabel) if (outputColumnLabels) printf(interpretedLabel); else fFieldDatabase->outputFieldInterpreted(label, interpretedLabel)
#define oBoolean_(label) if (outputColumnLabels) printf(label); else fFieldDatabase->outputFieldAsBoolean(label)
#define oFirst(label) o_(label,0)
#define oFrac(label,nFrac) do { printf(separator); o_(label,nFrac); } while (0)
#define o(label) oFrac(label,0)
#define oInterpreted(label,interpretedLabel) do { printf(separator); oInterpreted_(label,interpretedLabel); } while (0)
#define oBoolean(label) do { printf(separator); oBoolean_(label); } while (0)

void RecordAndDetailsParser::outputOneRow(int outputColumnLabels) {
  oFirst("CUSTOM.updateTime");
  oFrac("CUSTOM.hSpeed", 2);
  oFrac("CUSTOM.distance", 2);
  oFrac("OSD.latitude", 6);
  oFrac("OSD.longitude", 6);
  oFrac("OSD.height", 1);
  oFrac("OSD.xSpeed", 1);
  oFrac("OSD.ySpeed", 1);
  oFrac("OSD.zSpeed", 1);
  oFrac("OSD.pitch", 1);
  oFrac("OSD.roll", 1);
  oFrac("OSD.yaw", 1);
  oInterpreted("OSD.flycState.RAW", "OSD.flycState");
  oInterpreted("OSD.flycCommand.RAW", "OSD.flycCommand");
  oBoolean("OSD.canIOCWork");
  oInterpreted("OSD.groundOrSky.RAW", "OSD.groundOrSky");
  oBoolean("OSD.isMotorUp");
  oBoolean("OSD.isSwaveWork");
  oInterpreted("OSD.goHomeStatus.RAW", "OSD.goHomeStatus");
  oBoolean("OSD.isImuPreheated");
  oBoolean("OSD.isVisionUsed");
  o("OSD.voltageWarning");
  o("OSD.modeChannel");
  oBoolean("OSD.compassError");
  oBoolean("OSD.waveError");
  o("OSD.gpsLevel");
  oInterpreted("OSD.batteryType.RAW", "OSD.batteryType");
  oBoolean("OSD.isAcceletorOverRange");
  oBoolean("OSD.isVibrating");
  oBoolean("OSD.isBarometerDeadInAir");
  oBoolean("OSD.isMotorBlocked");
  oBoolean("OSD.isNotEnoughForce");
  oBoolean("OSD.isPropellerCatapult");
  oBoolean("OSD.isGoHomeHeightModified");
  oBoolean("OSD.isOutOfLimit");
  o("OSD.gpsNum");
  o("OSD.flightAction");
  oInterpreted("OSD.flightAction.RAW", "OSD.flightAction");
  oInterpreted("OSD.motorStartFailedCause.RAW", "OSD.motorStartFailedCause");
  oInterpreted("OSD.nonGPSCause.RAW", "OSD.nonGPSCause");
  oBoolean("OSD.isQuickSpin");
  o("OSD.battery");
  oFrac("OSD.sWaveHeight", 1);
  oFrac("OSD.flyTime", 1);
  o("OSD.motorRevolution");
  o("OSD.flycVersion");
  oInterpreted("OSD.droneType.RAW", "OSD.droneType");
  oInterpreted("OSD.imuInitFailReason.RAW", "OSD.imuInitFailReason");
  oInterpreted("OSD.motorFailReason.RAW", "OSD.motorFailReason");
  oInterpreted("OSD.ctrlDevice.RAW", "OSD.ctrlDevice");
  oFrac("GIMBAL.pitch", 1);
  oFrac("GIMBAL.roll", 1);
  oFrac("GIMBAL.yaw", 1);
  oInterpreted("GIMBAL.mode.RAW", "GIMBAL.mode");
  oFrac("GIMBAL.rollAdjust", 1);
  oFrac("GIMBAL.yawAngle", 1);
  oBoolean("GIMBAL.isAutoCalibration");
  o("GIMBAL.autoCalibrationResult");
  oBoolean("GIMBAL.isPitchInLimit");
  oBoolean("GIMBAL.isRollInLimit");
  oBoolean("GIMBAL.isYawInLimit");
  oBoolean("GIMBAL.isStuck");
  o("GIMBAL.version");
  oBoolean("GIMBAL.isSingleClick");
  oBoolean("GIMBAL.isDoubleClick");
  oBoolean("GIMBAL.isTripleClick");
  o("RC.aileron");
  o("RC.elevator");
  o("RC.throttle");
  o("RC.rudder");
  o("RC.gimbal");
  o("RC.goHome");
  o("RC.mode");
  o("RC.wheelOffset");
  o("RC.record");
  o("RC.shutter");
  o("RC.playback");
  o("RC.custom1");
  o("RC.custom2");
  o("CENTER_BATTERY.relativeCapacity");
  o("CENTER_BATTERY.currentPV");
  o("CENTER_BATTERY.currentCapacity");
  o("CENTER_BATTERY.fullCapacity");
  o("CENTER_BATTERY.life");
  o("CENTER_BATTERY.loopNum");
  o("CENTER_BATTERY.errorType");
  o("CENTER_BATTERY.current");
  o("CENTER_BATTERY.voltageCell1");
  o("CENTER_BATTERY.voltageCell2");
  o("CENTER_BATTERY.voltageCell3");
  o("CENTER_BATTERY.voltageCell4");
  o("CENTER_BATTERY.voltageCell5");
  o("CENTER_BATTERY.voltageCell6");
  o("CENTER_BATTERY.serialNo");
  o("CENTER_BATTERY.productDate");
  o("CENTER_BATTERY.temperature");
  o("CENTER_BATTERY.connStatus");
  o("CENTER_BATTERY.totalStudyCycle");
  o("CENTER_BATTERY.lastStudyCycle");
  o("CENTER_BATTERY.isNeedStudy");
  o("CENTER_BATTERY.isBatteryOnCharge");
  o("SMART_BATTERY.usefulTime");
  o("SMART_BATTERY.goHomeTime");
  o("SMART_BATTERY.landTime");
  o("SMART_BATTERY.goHomeBattery");
  o("SMART_BATTERY.landBattery");
  o("SMART_BATTERY.safeFlyRadius");
  o("SMART_BATTERY.volumeConsume");
  oInterpreted("SMART_BATTERY.status.RAW", "SMART_BATTERY.status");
  oInterpreted("SMART_BATTERY.goHomeStatus.RAW", "SMART_BATTERY.goHomeStatus");
  o("SMART_BATTERY.goHomeCountdown");
  o("SMART_BATTERY.voltage");
  o("SMART_BATTERY.battery");
  o("SMART_BATTERY.lowWarning");
  o("SMART_BATTERY.lowWarningGoHome");
  o("SMART_BATTERY.seriousLowWarning");
  o("SMART_BATTERY.seriousLowWarningLanding");
  o("SMART_BATTERY.voltagePercent");
  o("DEFORM.isDeformProtected");
  oInterpreted("DEFORM.deformStatus.RAW", "DEFORM.deformStatus");
  oInterpreted("DEFORM.deformMode.RAW","DEFORM.deformMode");
  oFrac("HOME.latitude", 6);
  oFrac("HOME.longitude", 6);
  oFrac("HOME.height", 2);
  oBoolean("HOME.isHomeRecord");
  o("HOME.goHomeMode");
  o("HOME.aircraftHeadDirection");
  oBoolean("HOME.isDynamicHomePointEnabled");
  o("HOME.goHomeStatus");
  oBoolean("HOME.hasGoHome");
  o("HOME.compassCeleStatus");
  oBoolean("HOME.isCompassCeleing");
  oBoolean("HOME.isBeginnerMode");
  oBoolean("HOME.isIOCEnabled");
  oInterpreted("HOME.iocMode.RAW", "HOME.iocMode");
  o("HOME.goHomeHeight");
  oFrac("HOME.courseLockAngle", 1);
  o("HOME.dataRecorderStatus");
  o("HOME.dataRecorderRemainCapacity");
  o("HOME.dataRecorderRemainTime");
  o("HOME.dataRecorderFileIndex");
  oInterpreted("RECOVER.droneType.RAW", "RECOVER.droneType");
  oInterpreted("RECOVER.appType.RAW", "RECOVER.appType");
  o("RECOVER.appVersion");
  o("RECOVER.aircraftSn");
  o("RECOVER.aircraftName");
  o("RECOVER.activeTimestamp");
  o("RECOVER.cameraSn");
  o("RECOVER.rcSn");
  o("RECOVER.batterySn");
  o("FIRMWARE.version");
  o("DETAILS.street");
  o("DETAILS.citypart");
  o("DETAILS.city");
  o("DETAILS.area");
  o("DETAILS.isFavorite");
  o("DETAILS.isNew");
  o("DETAILS.needUpload");
  o("DETAILS.recordLineCount");
  o("DETAILS.timestamp");
  oFrac("DETAILS.latitude", 6);
  oFrac("DETAILS.longitude", 6);
  oFrac("DETAILS.totalDistance", 2);
  oFrac("DETAILS.totalTime", 1);
  oFrac("DETAILS.maxHeight", 1);
  oFrac("DETAILS.maxHorizontalSpeed", 2);
  oFrac("DETAILS.maxVerticalSpeed", 1);
  o("DETAILS.photoNum");
  o("DETAILS.videoTime");
  o("DETAILS.activeTimestamp");
  o("DETAILS.aircraftName");
  o("DETAILS.aircraftSn");
  o("DETAILS.cameraSn");
  o("DETAILS.rcSn");
  o("DETAILS.batterySn");
  oInterpreted("DETAILS.appType.RAW", "DETAILS.appType");
  o("DETAILS.appVersion");
  oFrac("APP_GPS.latitude", 6);
  oFrac("APP_GPS.longitude", 6);
  o("APP_GPS.accuracy");
  o("APP_TIP.tip");
  o("APP_WARN.warn");
  printf("\n");
}
