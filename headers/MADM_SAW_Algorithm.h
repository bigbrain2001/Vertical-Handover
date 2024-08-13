/*
 * MADM_SAW_Algorithm.h
 *
 *  Created on: 13 Jul 2024
 *      Author: Rosca Augustin
 */

#ifndef HEADERS_MADM_SAW_ALGORITHM_H_
#define HEADERS_MADM_SAW_ALGORITHM_H_

#define n_colums 3

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
    int MADM_selection;
    double networkTransferRates[10];
    double Criteria[10][n_colums];
    double CriteriaForTOPSIS[10][n_colums];

    double networkWeights[3] = {0.3, 0.5, 0.2};
    int init_contor=0;
    std::vector<int> ranks;

    double minimum(double a, double b);
    double maximum(double a, double b);
    int doSAW();
    int doMaxMin();
    void sendSelection(int network, double transferRate);
    double scaleValues(double orig_min, double orig_max, double new_min, double new_max, double value);
    double calculateScoreSAW(double criteria, double weight);
    void doTOPSIS();
    double doSquaresColums(int column);
    //void sendMessage();
};

#endif /* HEADERS_MADM_SAW_ALGORITHM_H_ */
