
#ifndef HEADERS_EXTCONDANALYZER_H_
#define HEADERS_EXTCONDANALYZER_H_

#include <omnetpp.h>


using namespace omnetpp;

class extCondAnalyzer : public cSimpleModule
{

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    double minimum(double cell, double radio);
    void sendTR(double cell,double radio);
    cMessage *sendMessageEvent;
    double condR=0;
    double condL=0;
    int capacity;
    double timeL;
    double timeR;
    unsigned short int init_delay=0;
};



#endif /* HEADERS_EXTCONDANALYZER_H_ */
