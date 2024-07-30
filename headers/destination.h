
#ifndef HEADERS_DESTINATION_H_
#define HEADERS_DESTINATION_H_

#include <omnetpp.h>


using namespace omnetpp;

class Destination : public cSimpleModule
{
private:
    simsignal_t lifetimeSignal;
    simsignal_t lifetimeSignal_per_file;
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void finish();
    cMessage *sendMessageEvent;
    cHistogram lifetimeStats[20];
    cHistogram lifetimeStats_per_file[20];
    int fileSize[20];
    int currentFile=0;
    int algorithm_selected;
    int netSize;
    int filesize=0;
    int total_IP_size=0;
};





#endif /* HEADERS_DESTINATION_H_ */
