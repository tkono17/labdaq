/*
  CaenHelper.hxx
*/
#include "CaenUsbReadout/CaenHelper.hxx"
#include "CAENDigitizerType.h"
#include "CAENDigitizer.h"
#include <iostream>

using namespace std;

void printBoardInfo(const CAEN_DGTZ_BoardInfo_t& info) {
  cout << "*** BoardInfo ***" << endl;
  cout << "  ModelName:               " << info.ModelName << endl;
  cout << "  Model:                   " << info.Model << endl;
  cout << "  Channels:                " << info.Channels << endl;
  cout << "  FormFactor:              " << info.FormFactor << endl;
  cout << "  ROC_FirmwareRel:         " << info.ROC_FirmwareRel << endl;
  cout << "  ROC_FirmwareRel:         " << info.ROC_FirmwareRel << endl;
  cout << "  SerialNumber:            " << info.SerialNumber << endl;
  cout << "  PCB_Revision:            " << info.PCB_Revision << endl;
  cout << "  ADC_NBits:               " << info.ADC_NBits << endl;
  cout << "  SAMCorrectionDataLoaded: " << info.SAMCorrectionDataLoaded << endl;
  cout << "  CommHandle:              " << info.CommHandle << endl;
  cout << "  License:                 " << info.License << endl;
}

void printEventInfo(const CAEN_DGTZ_EventInfo_t& info) {
  cout << "*** EventInfo ***" << endl;
  cout << "  EventSize:      " << info.EventSize << endl;
  cout << "  BoardId:        " << info.BoardId << endl;
  cout << "  Pattern:        " << info.Pattern << endl;
  cout << "  ChannelMask:    " << info.ChannelMask << endl;
  cout << "  EventCounter:   " << info.EventCounter << endl;
  cout << "  TriggerTimeTag: " << info.TriggerTimeTag << endl;
}

