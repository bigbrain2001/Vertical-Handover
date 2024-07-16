/*
 * MADM_SAW_Algorithm.h
 *
 *  Created on: 13 Jul 2024
 *      Author: Rosca Augustin
 */

#ifndef HEADERS_MADM_SAW_ALGORITHM_H_
#define HEADERS_MADM_SAW_ALGORITHM_H_

#include <omnetpp.h>
using namespace omnetpp;

class MADM_SAW_Algorithm : public cSimpleModule
{
private:
    //cMessage *sendMessageEvent;



public:
    MADM_SAW_Algorithm();
    virtual ~MADM_SAW_Algorithm();
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    int nrNetworks;
    double networkTransferRates[10];
    double networkWeights[10];
    int init_contor=0;
    void doSAW();
    void sendMessage();
};

#endif /* HEADERS_MADM_SAW_ALGORITHM_H_ */
