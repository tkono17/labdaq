/*
  caen_vme.cxx
*/
#define LINUX
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include "CAENVMElib.h"
#include "TSystem.h"

//const unsigned int VMEADC_BASE = 0xffff00;
const unsigned int VMEADC_BASE = 0x000700;

using namespace std;

int main(int argc, char* argv[]) {
  int32_t handle(-999);

  char sw_release[100] = "";
  char fw_release[100] = "";

  cout << "Start" << endl;
  CAENVME_API status;

  status = CAENVME_SWRelease(sw_release);
  cout << "SW release : " << sw_release << " status=" << status << endl;

  status = CAENVME_Init(cvV2718, 0, 0, &handle);
  cout << "VME Init handle: " << handle << endl;

  if (handle < 0) {
    cout << "Error while initializing VME handle=" << handle << endl;
    return -2;
  }
  status = CAENVME_BoardFWRelease(handle, fw_release);
  cout << "FW release: " << fw_release << endl;

  bool ready;
  unsigned int data=0x0;
  unsigned int reset_data=0x1;

  while (true) {
    status = CAENVME_WriteCycle(handle, VMEADC_BASE+0x0, &reset_data, 
				cvA24_U_DATA, cvD16);
    cout << "Reset status : " << status << endl;

    ready = false;
    do {
      status = CAENVME_ReadCycle(handle, VMEADC_BASE+0x0, &data, 
				 cvA24_U_DATA, cvD16);
      if ( ( (data>>15) & 0x1) == 0x1) {
	ready = true;
	cout << "Data = " << std::hex << data << std::dec << endl;
      }
      if (ready) {
      } else {
	gSystem->Sleep(1);
      }
    } while (!ready);
  }

  return 0;
}
