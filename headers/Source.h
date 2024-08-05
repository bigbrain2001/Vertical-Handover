#ifndef HEADERS_SOURCE_H_
#define HEADERS_SOURCE_H_

#include <omnetpp.h>
#include <random>
using namespace omnetpp;

class Source : public cSimpleModule
{
private:
    cMessage *sendMessageEvent;
    double MAX_SIM;
    int file_Size;
    int file_split_size;
    int currentfileSize;
    int file_Size_min;
    int file_Size_max;
    int fileSize;


  public:
    Source();
    virtual ~Source();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};


#endif /* HEADERS_SOURCE_H_ */
