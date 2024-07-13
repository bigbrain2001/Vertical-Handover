#ifndef SOURCE_H_
#define SOURCE_H_

#include <omnetpp.h>
using namespace omnetpp;

class Source : public cSimpleModule
{
private:
    cMessage *sendMessageEvent;
    double MAX_SIM;
    int file_Size;
    int file_split_size;
    int currentfileSize;


  public:
    Source();
    virtual ~Source();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};


#endif /* SOURCE_H_ */
