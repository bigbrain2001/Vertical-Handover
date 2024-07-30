#include "headers/MADM_SAW_Algorithm.h"

Define_Module(MADM_SAW_Algorithm);
#define n_colums 2

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
        networkLoad[i] = 0;
    }
}

double MADM_SAW_Algorithm::scaleValues(double orig_min, double orig_max, double new_min, double new_max, double value){
    double scaled;
     if(value<=new_min)
         scaled = new_min;
     else if (value>=new_max){
         scaled = new_max;
     }
     else
         scaled = ((value-orig_min)/(orig_max-orig_min)) * (new_max-new_min) + new_min;
    return scaled;
}

double MADM_SAW_Algorithm::calculateScoreSAW(double criteria, double weight){
    double score=0.0;
        score+= weight * criteria;
    return score;
}

void MADM_SAW_Algorithm::doSAW(){
    double maxRate = 0;
    double maxValue[n_colums];
    double SAW_Scores[n_colums];
    int best_network = -1;

    for(int i=0;i<n_colums;i++){
        maxValue[i] = 0.0;
        SAW_Scores[i] = 0.0;
    }

        /*for(int i = 0; i < nrNetworks;i++){
            if(networkTransferRates[i] > maxRate){
                maxRate = networkTransferRates[i];
                //best_network = i;
            }
        }*/

    for(int i = 0; i < nrNetworks; i++){
        for(int j = 0; j < n_colums; j++){
            if(CriteriaForSAW[i][j] > maxValue[j]){
                maxValue[j] = CriteriaForSAW[i][j];
            }
        }

    }
    for(int i=0;i< nrNetworks;i++){
        for(int j=0;j<n_colums;j++){
            CriteriaForSAW[i][j] /= maxValue[j]; //scale with rij = xij/xj(max)
        }
    }

    double max_Score=-1;
    for(int i=0;i<nrNetworks;i++){
        for(int j=0; j < n_colums; j++){
            SAW_Scores[i] += calculateScoreSAW(CriteriaForSAW[i][j], networkWeights[j]); //calculate SAW scores for all networks
        }

        if(max_Score < SAW_Scores[i]){
            best_network = i; //choose the best network based on the chosen score
        }
    }

    cMessage *selection = new cMessage("selection");
    selection->addPar("select_network");
    selection->par("select_network") = best_network;
    selection->addPar("Transfer_Rate");
    selection->par("Transfer_Rate") = networkTransferRates[best_network];
    send(selection, "txSelection");

}

void MADM_SAW_Algorithm::handleMessage(cMessage *msg){
    for(int i=0;i < nrNetworks; i++){
        if(msg->arrivedOn("rxTransferRates"),i){
            double transferRate = (double)msg->par("transferRate");
            double netLoad = (double)msg->par("netLoad");
            networkTransferRates[i] = transferRate;
            CriteriaForSAW[i][0] = transferRate;
            CriteriaForSAW[i][1] = netLoad;
            //networkLoad[i] = netLoad;
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


