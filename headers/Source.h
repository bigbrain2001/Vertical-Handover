#ifndef HEADERS_SOURCE_H_
#define HEADERS_SOURCE_H_

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


#endif /* HEADERS_SOURCE_H_ */
