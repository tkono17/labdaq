/*
  pddaq.cxx
*/
#define LINUX
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include "CAENVMElib.h"
#include "CaenVme/PDDaqConfig.hxx"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"


const unsigned int VMEADC_BASE = 0x000700;

using namespace std;

int main(int argc, char* argv[]) {
  int retcode;
  int32_t handle(-999);

  // char sw_release[100] = "";
  // char fw_release[100] = "";

  PDDaqConfig config;
  if ( (retcode = config.parse_args(argc, argv) ) != 0) {
    return retcode;
  }
  config.print();

  cout << "Start DAQ" << endl;
  CAENVME_API status;

  status = CAENVME_Init(cvV2718, 0, 0, &handle);
  if (handle < 0) {
    cout << "Error while initializing VME, VMEstatus=" << status 
	 << ", handle=" << handle << endl;
    return -2;
  }

  bool ready;
  unsigned int data=0x0;
  unsigned int reset_data=0x1;
  int nevents = 0;
  int ch;
  char bname1[100];
  char bname2[100];
  int adc[8];

  TFile* fout = TFile::Open(config.outputFileName().c_str(), "RECREATE");
  TTree* tree = new TTree("t", "TTree from Photo-Detector DAQ");
  for (ch=0; ch<8; ++ch) {
    if (config.channelStatus(ch)) {
      sprintf(bname1, "adc%d", ch);
      sprintf(bname2, "adc%d/i", ch);
      tree->Branch(bname1, &adc[ch], bname2);
    }
  }
  while (true) {
    if (nevents >= config.maxEvents()) {
      break;
    }
    status = CAENVME_WriteCycle(handle, VMEADC_BASE+0x0, &reset_data, 
				cvA24_U_DATA, cvD16);
    ready = false;
    do {
      ready = true;
      for (ch=0; ch<8; ++ch) {
	status = CAENVME_ReadCycle(handle, VMEADC_BASE+0x0, &data, 
				   cvA24_U_DATA, cvD16);
	//	cout << "Status ch " << ch << ": " << status << ", data 0x" 
	//	     << std::hex << data << std::dec << endl;
	if ( ( (data>>15) & 0x1) == 0x0) {
	  ready = false;
	  break;
	}
	adc[ch] = data & 0x3fff;
      }
      //cout << "Ready ok? " << ready << endl;
      if (ready) {
	tree->Fill();
	nevents ++;
      } else {
	gSystem->Sleep(1);
      }
    } while (!ready);
  }

  tree->Write();
  fout->Write();

  return 0;
}
