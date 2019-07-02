#ifndef __PDDaqConfig_hxx__
#define __PDDaqConfig_hxx__
/*
  PDDaqConfig.hxx
*/
#include <string>

class PDDaqConfig {
public:
  PDDaqConfig();
  ~PDDaqConfig();

  void setOutputFileName(const std::string& x) { mOutputFileName=x; }
  void setMaxEvents(int n) { mMaxEvents = n; }
  void setChannelStatus(int ch, int status);

  const std::string& outputFileName() const { return mOutputFileName; }
  int maxEvents() const { return mMaxEvents; }
  const int* channelStatus() const { return mChannelStatus; }
  int channelStatus(int ch) const;

  int parse_args(int argc, char* argv[]);

  void print() const;

protected:
  std::string mOutputFileName;
  int mMaxEvents;
  int mChannelStatus[8];
};

#endif // __PDDaqConfig_hxx__
