
#ifndef DESTINATION_H_
#define DESTINATION_H_

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
    int currentFile=0;;
    int netSize;
    int filesize=0;
    int first_pack;
    int total_IP_size=0;
};





#endif /* DESTINATION_H_ */
