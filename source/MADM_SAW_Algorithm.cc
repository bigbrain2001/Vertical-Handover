/*
 * MADM_SAW_Algorithm.cc
 *
 *  Created on: 13 Jul 2024
 *      Author: Rosca Augustin
 */

#include "headers/MADM_SAW_Algorithm.h"

Define_Module(MADM_SAW_Algorithm);

MADM_SAW_Algorithm::MADM_SAW_Algorithm()
{
    //sendMessageEvent = nullptr;
}

MADM_SAW_Algorithm::~MADM_SAW_Algorithm()
{
    //cancelAndDelete(sendMessageEvent);
}


void MADM_SAW_Algorithm::initialize(){
    nrNetworks = par("networkSize").intValue();
    for(int i=0;i<nrNetworks; i++){
        networkTransferRates[i] = 0;
    }
}

void MADM_SAW_Algorithm::doSAW(){
    double maxRate=0;
    int best_network = -1;
    for(int i = 0; i < nrNetworks;i++){
        if(networkTransferRates[i] > maxRate){
            maxRate = networkTransferRates[i];
            best_network = i;
        }
    }
    cMessage *selection = new cMessage("selection");
    selection->addPar("select_network");
    selection->par("select_network") = best_network;
    selection->addPar("Transfer_Rate");
    selection->par("Transfer_Rate") = maxRate;
    send(selection, "txSelection");

}

void MADM_SAW_Algorithm::handleMessage(cMessage *msg){
    for(int i=0;i < nrNetworks; i++){
        if(msg->arrivedOn("rxTransferRates"),i){
            double transferRate = (double)msg->par("transferRate");
            networkTransferRates[i] = transferRate;
            delete msg;
            if(init_contor < nrNetworks){
                init_contor++;
            }
            else{
                doSAW();
            }
            break;
        }
    }
}


