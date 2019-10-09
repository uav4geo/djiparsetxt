INCLUDES =
##### Change the following for your environment:
COMPILE_OPTS =          -O $(INCLUDES) -I. 
CPP =                   cpp
CPLUSPLUS_COMPILER =    c++
CPLUSPLUS_FLAGS =       $(COMPILE_OPTS) -Wall
OBJ =                   o
LINK =                  c++ -o 
LINK_OPTS =
EXE =
##### End of variables to change

ALL = djiparsetxt
all:	$(ALL)

DJIPARSETXT_OBJS = djiparsetxt.$(OBJ) DJITxtParser.$(OBJ) RecordAndDetailsParser.$(OBJ) \
	parseDetails.$(OBJ) \
	parseRecord.$(OBJ) \
	parseRecord_OSD.$(OBJ) \
	parseRecord_HOME.$(OBJ) \
	parseRecord_GIMBAL.$(OBJ) \
	parseRecord_RC.$(OBJ) \
	parseRecord_CUSTOM.$(OBJ) \
	parseRecord_DEFORM.$(OBJ) \
	parseRecord_CENTER_BATTERY.$(OBJ) \
	parseRecord_SMART_BATTERY.$(OBJ) \
	parseRecord_APP_TIP.$(OBJ) \
	parseRecord_APP_WARN.$(OBJ) \
	parseRecord_RECOVER.$(OBJ) \
	parseRecord_APP_GPS.$(OBJ) \
	parseRecord_FIRMWARE.$(OBJ) \
	parseRecord_JPEG.$(OBJ) \
	parseRecordUnknownFormat.$(OBJ) \
	scrambleTable.$(OBJ) \
	parseFieldWithinRecord.$(OBJ) \
	FieldDatabase.$(OBJ) \
	InterpretationTable.$(OBJ) \
	interpretationTables.$(OBJ) \
	rowOutput.$(OBJ) \
	fieldOutput.$(OBJ)
djiparsetxt: $(DJIPARSETXT_OBJS)
	$(LINK)$@ $(DJIPARSETXT_OBJS)

djiparsetxt.$(CPP):				DJITxtParser.hh
DJITxtParser.$(CPP): 	   			DJITxtParser.hh
RecordAndDetailsParser.$(CPP):			RecordAndDetailsParser.hh
RecordAndDetailsParser.hh:			DJITxtParser.hh FieldDatabase.hh
parseDetails.$(CPP):				RecordAndDetailsParser.hh
parseRecord.$(CPP):				RecordAndDetailsParser.hh
parseRecord_OSD.$(CPP):				RecordAndDetailsParser.hh
parseRecord_HOME.$(CPP):			RecordAndDetailsParser.hh
parseRecord_GIMBAL.$(CPP):			RecordAndDetailsParser.hh
parseRecord_RC.$(CPP):				RecordAndDetailsParser.hh
parseRecord_CUSTOM.$(CPP):			RecordAndDetailsParser.hh
parseRecord_DEFORM.$(CPP):			RecordAndDetailsParser.hh
parseRecord_CENTER_BATTERY.$(CPP):		RecordAndDetailsParser.hh
parseRecord_SMART_BATTERY.$(CPP):		RecordAndDetailsParser.hh
parseRecord_APP_TIP.$(CPP):			RecordAndDetailsParser.hh
parseRecord_APP_WARN.$(CPP):			RecordAndDetailsParser.hh
parseRecord_RECOVER.$(CPP):			RecordAndDetailsParser.hh
parseRecord_APP_GPS.$(CPP):			RecordAndDetailsParser.hh
parseRecord_FIRMWARE.$(CPP):			RecordAndDetailsParser.hh
parseRecord_JPEG.$(CPP):			RecordAndDetailsParser.hh
parseRecordUnknownFormat.$(CPP):		RecordAndDetailsParser.hh
parseFieldWithinRecord.$(CPP):			RecordAndDetailsParser.hh
FieldDatabase.$(CPP):				FieldDatabase.hh
InterpretationTable.$(CPP):			InterpretationTable.hh
interpretationTables.$(CPP):			FieldDatabase.hh
FieldDatabase.hh:				InterpretationTable.hh
rowOutput.$(CPP):				RecordAndDetailsParser.hh
fieldOutput.$(CPP):				FieldDatabase.hh

.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

clean:
	-rm -rf *.$(OBJ) $(ALL) core *.core *~













