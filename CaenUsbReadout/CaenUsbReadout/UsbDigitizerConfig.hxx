#ifndef __UsbDigitizerConfig_hxx__
#define __UsbDigitizerConfig_hxx__
/*
  UsbDigitizerConfig.hxx
*/
#include <string>

class UsbDigitizerConfig {
public:
  UsbDigitizerConfig();
  ~UsbDigitizerConfig();

  void setOutputFileName(const std::string& x) { mOutputFileName = x; }
  void setChannelStatus(int ch, int status);
  void setNumberOfSamples(int n) { mNumberOfSamples = n; }
  void setMaxEvents(int n) { mMaxEvents = n; }

  const std::string& outputFileName() const { return mOutputFileName; }
  const int* channelStatus() const { return mChannelStatus; }
  int channelStatus(int ch) const;
  int numberOfSamples() const { return mNumberOfSamples; }
  int maxEvents() const { return mMaxEvents; }

  bool isChannelActive(int ch) const;
  void getMask(unsigned int& mask) const;

  int parse_args(int argc, char* argv[]);

protected:
  std::string mOutputFileName;
  int mChannelStatus[4];
  int mNumberOfSamples;
  int mMaxEvents;

};

#endif // __UsbDigitizerConfig_hxx__
