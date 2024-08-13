
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
    double maximum(double cell, double radio);
    void sendTR(double cell,double radio);
    double calculate_cell_load();
    //cMessage *sendMessageEvent;
    double condR=0;
    double condL=0;
    double current_cap=0;;
    int capacity;
    int net_load_select;
    double battery_consumption;
    double netLoad[20];
    unsigned short int pointer=0;
    double timeL;
    double timeR;
    unsigned short int init_delay=0;
};



#endif /* HEADERS_EXTCONDANALYZER_H_ */
