/*
  UsbDigitizerConfig.cxx
*/
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "CaenUsbReadout/UsbDigitizerConfig.hxx"

using namespace std;

UsbDigitizerConfig::UsbDigitizerConfig() {
  mOutputFileName = "usbdaq.root";
  for (int i=0; i<4; ++i) {
    mChannelStatus[i] = 0;
  }
  //  mNumberOfSamples = 2500;
  mNumberOfSamples = 25000;
  mMaxEvents = 1000;
}

UsbDigitizerConfig::~UsbDigitizerConfig() {
}

void UsbDigitizerConfig::setChannelStatus(int ch, int status) {
  if (ch >= 0 && ch < 4) {
    mChannelStatus[ch] = status;
  }
}

int UsbDigitizerConfig::channelStatus(int ch) const {
  if (ch >= 0 && ch < 4) {
    return mChannelStatus[ch];
  } else {
    return 0;
  }
}

void UsbDigitizerConfig::getMask(unsigned int& mask) const {
  mask = 0x0;
  for (int i=0; i<4; ++i) {
    if (mChannelStatus[i] == 1) {
      mask += (1 << i);
    }
  }
}

bool UsbDigitizerConfig::isChannelActive(int ch) const {
  if (ch >= 0 && ch < 4) {
    return mChannelStatus[ch];
  }
  return 0;
}

int UsbDigitizerConfig::parse_args(int argc, char* argv[]) {
  int iarg=1;
  std::string k="";
  std::string v;
  bool show_help=false;
  bool err = false;
  int badarg=-1;

  while (iarg < argc) {
    k = argv[iarg];
    if (k == "--output-file" || k == "-o") {
      iarg ++;
      if (iarg < argc) {
	mOutputFileName = argv[iarg];
      }
    } else if (k == "--channels" || k == "-c") {
      iarg ++;
      if (iarg < argc) {
	for (unsigned int j=0; j<4; ++j) {
	  if (j < std::strlen(argv[iarg])) {
	    v = argv[iarg][j];
	    int ch = std::atoi(v.c_str());
	    mChannelStatus[ch] = 1;
	  }
	}
      }
    } else if (k == "--samples" || k == "-s") {
      iarg ++;
      if (iarg < argc) {
	mNumberOfSamples = std::atoi(argv[iarg]);
      }
    } else if (k == "--max-events" || k == "-n") {
      iarg ++;
      if (iarg < argc) {
	mMaxEvents = std::atoi(argv[iarg]);
      }
    } else if (k == "-h" || k == "--help") {
      show_help = true;
      break;
    } else {
      show_help = true;
      badarg = iarg;
      err = true;
      break;
    }
    iarg ++;
  }

  if (err && badarg >= 0) {
    cout << "Bad argument at argv[" << badarg << "]" << endl;
  }
  if (show_help) {
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << "  Options: -n|--max-events <n>" << endl;
    cout << "           -s|--samples <n>" << endl;
    cout << "           -c|--channels 0123" << endl;
    cout << "           -o|--output-file <filename.root>" << endl;
  }
  if (err) {
    return -1;
  } else if (show_help) {
    return 1;
  }
  return 0;
}
