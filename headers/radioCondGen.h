#ifndef HEADERS_RADIOCONDGEN_H_
#define HEADERS_RADIOCONDGEN_H_

#include <omnetpp.h>
#include <random>


using namespace omnetpp;

class radioCondGen : public cSimpleModule
{

public:
  radioCondGen();
  virtual ~radioCondGen();
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
    int seed;
    double radio_min;
    double radio_max;
};



#endif /* HEADERS_RADIOCONDGEN_H_ */
