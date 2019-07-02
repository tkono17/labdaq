/*
  PDDaqConfig.cxx
*/
#include "CaenVme/PDDaqConfig.hxx"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

PDDaqConfig::PDDaqConfig() {
  mOutputFileName = "pddaq.root";
  mMaxEvents = 1;
  for (int i=0; i<8; ++i) {
    mChannelStatus[i] = 0;
  }
}

PDDaqConfig::~PDDaqConfig() {
}

void PDDaqConfig::setChannelStatus(int ch, int status) {
  if (ch >= 0 && ch < 8) {
    mChannelStatus[ch] = status;
  }
}

int PDDaqConfig::channelStatus(int ch) const {
  if (ch >= 0 && ch < 8) {
    return mChannelStatus[ch];
  }
  return 0;
}

int PDDaqConfig::parse_args(int argc, char* argv[]) {
  std::string k, v;
  bool err=false;
  bool show_help=false;
  int iarg=0;
  int ch;
  int status=0;

  for (ch=0; ch<8; ++ch) {
    setChannelStatus(ch, 0);
  }
  for (iarg=1; iarg<argc; ++iarg) {
    k = argv[iarg];
    cout << "for " << iarg << ", " << k << endl;
    if (k == "-o" || k == "--output-file") {
      iarg ++;
      mOutputFileName = argv[iarg];
    } else if (k == "-n" || k == "--max-events") {
      iarg ++;
      mMaxEvents = std::atoi(argv[iarg]);
    } else if (k == "-c" || k == "--channels") {
      iarg ++;
      for (unsigned int i=0; i<std::strlen(argv[iarg]); ++i) {
	v = argv[iarg][i];
	ch = std::atoi(v.c_str());
	setChannelStatus(ch, 1);
      }
    } else if (k == "-h" || k == "--help") {
      show_help = true;
      break;
    } else {
      err = true;
      show_help = true;
      break;
    }
    cout << "k = " << k << " i " << iarg << endl;
  }
  if (err) {
    cout << "Unknown option in iarg=" << iarg << ", " << argv[iarg] << endl;
    status = -1;
  }
  if (show_help) {
    cout << "Usage: " << argv[0] << " [Options]" << endl;
    cout << "  Options: -n|--max-events <n>" << endl;
    cout << "           -c|--channels <channels>" << endl;
    cout << "           -h|--help" << endl;
    cout << "  Examples: " << argv[0] << " -n 100 -c 0178" << endl;
    if (!err) status = 1;
  }
  return status;
}

void PDDaqConfig::print() const {
  cout << "PDDaqConfig: " << endl;
  cout << "  OutputFileName: " << mOutputFileName << endl;
  cout << "  MaxEvents: " << mMaxEvents << endl;
  cout << "  ActiveChannels: ";
  for (int ch=0; ch<8; ++ch) {
    if (mChannelStatus[ch] != 0) cout << " " << ch << ", ";
  }
  cout << endl;
}
