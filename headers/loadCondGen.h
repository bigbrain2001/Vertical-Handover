#ifndef HEADERS_LOADCONDGEN_H_
#define HEADERS_LOADCONDGEN_H_

#include <omnetpp.h>
#include <random>


using namespace omnetpp;

class loadCondGen : public cSimpleModule
{

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    double init_cell_transfer_capacity; //kBps
    cMessage *sendMessageEvent;
    double cellLoadTime;
    double mean;
    double deviation;
    int capacity;
    std::default_random_engine generator;
    std::normal_distribution<double> normal_dist;
};



#endif /* HEADERS_LOADCONDGEN_H_ */
