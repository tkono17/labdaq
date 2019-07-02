/*
  caen_usb1.cxx
  ==========
  Test communication with the CAEN 250 MHz digitizer via USB.
*/

#include <iostream>
#include "CAENDigitizer.h"
#include "CaenUsbReadout/CaenHelper.hxx"
#include "CaenUsbReadout/TreeHelper.hxx"
#include "CaenUsbReadout/UsbDigitizerConfig.hxx"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"

using namespace std;

int main(int argc, char* argv[]) {
  cout << "Test connection to CAEN digitizer via USB" << endl;

  int handle(-1);
  CAEN_DGTZ_ErrorCode retcode;
  CAEN_DGTZ_BoardInfo_t board_info;
  CAEN_DGTZ_TriggerMode_t trigger_mode=CAEN_DGTZ_TRGMODE_ACQ_ONLY;
  CAEN_DGTZ_IOLevel_t io_level=CAEN_DGTZ_IOLevel_NIM;
  uint32_t mask(0x1);
  //  uint32_t length(2500);
  uint32_t length(1000);
  uint32_t post_trigger_size(0);
  CAEN_DGTZ_AcqMode_t daq_mode;
  int ch;

  UsbDigitizerConfig config;
  int status=0;
  if ( (status = config.parse_args(argc, argv) ) != 0) {
    return status;
  }

  config.getMask(mask);
  length = config.numberOfSamples();

  Int_t event_number;
  UInt_t timestamp;
  UInt_t Nadc = config.numberOfSamples();
  //  UInt_t adc[4][10000];
  UInt_t adc[4][25000];
  char bname1[100];
  char bname2[100];
  TFile* fout = TFile::Open(config.outputFileName().c_str(), "RECREATE");
  TTree* tree = new TTree("t", "Data from USB Digitizer");
  tree->Branch("event_number", &event_number, "event_number/I");
  tree->Branch("timestamp", &timestamp, "timestamp/i");
  tree->Branch("nadc", &Nadc, "Nadc/i");
  for (int i=0; i<4; ++i) {
    if ( ( (mask >> i) & 0x1) == 0x1) {
      sprintf(bname1, "adc%d", i);
      sprintf(bname2, "adc%d[Nadc]/i", i);
      cout << "bname2 : " << bname2 << endl;
      tree->Branch(bname1, adc[i], bname2);
    }
  }

  // Open connection to the device
  retcode = CAEN_DGTZ_OpenDigitizer(CAEN_DGTZ_USB, 0, 0, 0, &handle);
  if (retcode != 0) {
    cout << "Failed to open the digitizer" << endl;
    cout << "retcode: " << retcode << endl;
    cout << "Handle : " << handle << endl;
    return retcode;
  }
  // Configuration
  retcode = CAEN_DGTZ_GetInfo(handle, &board_info);
  printBoardInfo(board_info);

  retcode = CAEN_DGTZ_SetSWTriggerMode(handle, CAEN_DGTZ_TRGMODE_DISABLED);
  retcode = CAEN_DGTZ_SetExtTriggerInputMode(handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);
retcode = CAEN_DGTZ_GetExtTriggerInputMode(handle, &trigger_mode);
  // retcode = CAEN_DGTZ_SetSWTriggerMode(handle, CAEN_DGTZ_TRGMODE_ACQ_ONLY);
  // retcode = CAEN_DGTZ_SetExtTriggerInputMode(handle,CAEN_DGTZ_TRGMODE_DISABLED);
  retcode = CAEN_DGTZ_GetExtTriggerInputMode(handle, &trigger_mode);
  cout << "Trigger mode: " << trigger_mode << endl;
  retcode = CAEN_DGTZ_GetIOLevel(handle, &io_level);

  cout << "I/O level: " << io_level << endl;

  retcode = CAEN_DGTZ_SetChannelEnableMask(handle, mask);
  retcode = CAEN_DGTZ_GetChannelEnableMask(handle, &mask);
  cout << "Channel mask set: " << mask << endl;

  retcode = CAEN_DGTZ_SetRecordLength(handle, length);
  retcode = CAEN_DGTZ_GetRecordLength(handle, &length);
  cout << "Record length: " << length << endl;

  retcode = CAEN_DGTZ_SetPostTriggerSize(handle, post_trigger_size);
  retcode = CAEN_DGTZ_GetPostTriggerSize(handle, &post_trigger_size);
  cout << "Post trigger size: " << post_trigger_size << " (%)" << endl;

  retcode = CAEN_DGTZ_GetAcquisitionMode(handle, &daq_mode);
  cout << "Acquisition mode: " << daq_mode << endl;

  for (ch=0; ch<4; ++ch) {
    if (config.isChannelActive(ch)) {
      retcode = CAEN_DGTZ_SetChannelDCOffset(handle, ch, 0xa000);
      //retcode = CAEN_DGTZ_SetChannelDCOffset(handle, ch, 0xffff);
    }
  }

  // Data acquisition
  retcode = CAEN_DGTZ_SWStartAcquisition(handle);

  char* buffer=0;
  uint32_t bufsize;
  int max_events_transfer=10;
  void** event=0;

  retcode = CAEN_DGTZ_SetMaxNumEventsBLT(handle, max_events_transfer);
  retcode = CAEN_DGTZ_MallocReadoutBuffer(handle, &buffer, &bufsize);
  uint32_t bufsize0 = bufsize;

  cout << "bufsize : " << bufsize << endl;

  uint32_t nevents(0);
  int32_t ievent;
  CAEN_DGTZ_EventInfo_t event_info;
  char* event_ptr(0);

  while (true) {
    // for (int j=0; j<i; ++j) {
    //   retcode = CAEN_DGTZ_SendSWtrigger(handle);
    // }
    if (event_number >= config.maxEvents()) {
      cout << "Finished processing " << event_number << " events / "
	   << config.maxEvents() << " (max specified)" << endl;
      break;
    }
    // cout << "event_number so far: " << event_number << endl;

    retcode = CAEN_DGTZ_ReadData(handle, CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT, 
				 buffer, &bufsize);
    retcode = CAEN_DGTZ_GetNumEvents(handle, buffer, bufsize, &nevents);
    
    // cout << "Buf/N events : " << bufsize << "/" << nevents << endl;
    for (ievent=0; ievent<static_cast<int>(nevents); ++ievent) {
      retcode = CAEN_DGTZ_GetEventInfo(handle, buffer, bufsize, 
				       ievent, &event_info, &event_ptr);
      //cout << "  ievent : " << ievent << endl;
      printEventInfo(event_info);

      unsigned int* words = reinterpret_cast<unsigned int*>(event_ptr);
      event_number = words[2] & 0xffffff;
      timestamp = words[3];
      int offset = 4;
      for (ch=0; ch<4; ++ch) {
	if ( ( (mask>>ch) & 0x1 ) != 0x1) continue;

	for (int isample=0; isample<static_cast<int>(length); ++isample) {
	  int shift = 0;
	  if ( (isample % 2) == 1) shift = 16;
	  int iword = offset + (isample/2);
	  unsigned int data = words[iword];
	  adc[ch][isample] = (data >> shift) & 0xfff;
	}
	offset += length/2;
      }
      // retcode = CAEN_DGTZ_DecodeEvent(handle, event_ptr, &event);
      // retcode = CAEN_DGTZ_FreeEvent(handle, &event);
      tree->Fill();
    }

  }

  tree->Write();
  fout->Write();

  retcode = CAEN_DGTZ_SWStopAcquisition(handle);

  retcode = CAEN_DGTZ_FreeReadoutBuffer(&buffer);

  // Close connection to the device
  retcode = CAEN_DGTZ_CloseDigitizer(handle);
  cout << "retcode on close: " << retcode << endl;

  return 0;
}

