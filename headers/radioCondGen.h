#ifndef HEADERS_RADIOCONDGEN_H_
#define HEADERS_RADIOCONDGEN_H_

#include <omnetpp.h>
#include <random>


using namespace omnetpp;

class radioCondGen : public cSimpleModule
{

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    cMessage *sendMessageEvent;
    double cellLoadTime;
    std::default_random_engine generator;
    std::normal_distribution<double> normal_dist;
    double meanNormal;
    double deviationNormal;
    double initRadioTransferCapacity;
};



#endif /* HEADERS_RADIOCONDGEN_H_ */
