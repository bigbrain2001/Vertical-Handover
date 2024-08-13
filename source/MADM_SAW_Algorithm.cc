#include "headers/MADM_SAW_Algorithm.h"
#include <cmath>
#include <limits.h>
#include <float.h>
#include <algorithm> //for std::sort

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
    MADM_selection = par("MADM_selection").intValue();
    for(int i=0;i<nrNetworks; i++){
        networkTransferRates[i] = 0;
    }
}

void MADM_SAW_Algorithm::sendSelection(int network, double transferRate){
    cMessage *selection = new cMessage("selection");
    selection->addPar("select_network");
    selection->par("select_network") = network;
    selection->addPar("Transfer_Rate");
    selection->par("Transfer_Rate") = transferRate;
    send(selection, "txSelection");
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

int MADM_SAW_Algorithm::doSAW(){
    double maxRate = 0;
    double maxValue[n_colums];
    double CriteriaForSAW[nrNetworks][n_colums];
    double SAW_Scores[nrNetworks];

    for(int i=0;i<nrNetworks;i++){
        for(int j=0;j<n_colums;j++){
            CriteriaForSAW[i][j] = Criteria[i][j];
        }
    }

    for(int i=0; i< nrNetworks;i++){
        SAW_Scores[i] = 0.0;
    }
    int best_network = -1;

    for(int i=0;i<n_colums;i++){
        maxValue[i] = 0.0;

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
    return best_network;
    //sendSelection(best_network, networkTransferRates[best_network]);
    /*
    cMessage *selection = new cMessage("selection");
    selection->addPar("select_network");
    selection->par("select_network") = best_network;
    selection->addPar("Transfer_Rate");
    selection->par("Transfer_Rate") = networkTransferRates[best_network];
    send(selection, "txSelection");
    */

}


int MADM_SAW_Algorithm::doMaxMin(){
    double CriteriaForMinMax[nrNetworks][n_colums];
    double bestValue = -1;
    int bestNetwork = -1;
    for(int i=0;i<nrNetworks;i++){
        double minimumVal = DBL_MAX;
        for(int j=0;j<n_colums;j++){
            CriteriaForMinMax[i][j] = pow(Criteria[i][j],networkWeights[j]*10); // Aij = a[i][j]^weight[j]
            if(minimumVal > CriteriaForMinMax[i][j]){
                minimumVal = CriteriaForMinMax[i][j];
            }
        }

        if(minimumVal > bestValue){
            bestValue = minimumVal;
            bestNetwork = i;
        }
    }
    return bestNetwork;
    //sendSelection(bestNetwork, networkTransferRates[bestNetwork]);


}

double MADM_SAW_Algorithm::doSquaresColums(int column){
    double sum=0.0;
    for(int i=0;i<nrNetworks;i++){
            sum+= pow(CriteriaForTOPSIS[i][column],2);
    }
    return sqrt(sum);
}

double MADM_SAW_Algorithm::minimum(double a, double b){
    if(a < b){
        return a;
    }
    else return b;
}

double MADM_SAW_Algorithm::maximum(double a, double b){
    if(a > b){
        return a;
    }
    else return b;
}

void MADM_SAW_Algorithm::doTOPSIS(){

    double bestValue = -1;
    int bestNetwork = -1;

    for(int i=0;i<nrNetworks;i++){
        for(int j=0;j<n_colums;j++){
            CriteriaForTOPSIS[i][j] = Criteria[i][j];
        }
    }
    for(int i=0;i<n_colums;i++){
        double normalFactor = doSquaresColums(i);
        for(int j=0;j<nrNetworks;j++){
            CriteriaForTOPSIS[j][i] /= normalFactor; //normalize the matrix
            CriteriaForTOPSIS[j][i] *= networkWeights[j]; //apply weights
        }
    }

    //find the ideal and negative ideal solutions
    double idealSolutions[n_colums];
    double negativeIdealSolutions[n_colums];
    for(int j=0;j<n_colums;j++){
        double maxVal = -DBL_MAX;
        double minVal = DBL_MAX;
        for(int i=0;i<nrNetworks;i++){
            maxVal = maximum(maxVal,CriteriaForTOPSIS[i][j]);
            minVal = minimum(minVal,CriteriaForTOPSIS[i][j]);
        }
        idealSolutions[j] = maxVal;
        negativeIdealSolutions[j] = minVal;
    }
    //calculate the separation from ideal and non-ideal solutions
    double separationFromIdeal[nrNetworks];
    double separationFromNonIdeal[nrNetworks];
    for(int i=0;i<nrNetworks;i++){
        double distanceToIdeal=0.0;
        double distanceToNonIdeal=0.0;

        for(int j=0;j<n_colums;j++){
            distanceToIdeal+= pow(CriteriaForTOPSIS[i][j]-idealSolutions[j],2);
            distanceToNonIdeal+= pow(CriteriaForTOPSIS[i][j]-negativeIdealSolutions[j],2);
        }
        separationFromIdeal[i] = sqrt(distanceToIdeal);
        separationFromNonIdeal[i] = sqrt(distanceToNonIdeal);
    }

    double closeness[nrNetworks];
    for(int i=0;i<nrNetworks;i++){
        closeness[i] = separationFromNonIdeal[i] / (separationFromNonIdeal[i] + separationFromIdeal[i]);
    }
   //i will need a vector to save all the data, and use pair
    ranks.resize(nrNetworks);
    std::vector<std::pair<double, int>> closenessWithIndex(nrNetworks);

    for (int i = 0; i < nrNetworks; i++) {
        closenessWithIndex[i] = std::make_pair(closeness[i], i);
    }

    std::sort(closenessWithIndex.begin(), closenessWithIndex.end(), std::greater<>());

    for (int i = 0; i < ranks.size(); i++) {
        ranks[i] = closenessWithIndex[i].second;
    }
}

void MADM_SAW_Algorithm::handleMessage(cMessage *msg){
    for(int i=0;i < nrNetworks; i++){
        if(msg->arrivedOn("rxTransferRates",i)){
            double transferRate = (double)msg->par("transferRate");
            double netLoad = (double)msg->par("netLoad");
            double battery_consumption = (double)msg->par("battery_consumption");
            Criteria[i][0] = networkTransferRates[i] = transferRate;
            Criteria[i][1] = netLoad;
            Criteria[i][2] = battery_consumption;
            delete msg;
            if(init_contor < nrNetworks){
                init_contor++;
            }
            else{
                switch(MADM_selection){
                case 0:{
                    int best = doSAW();
                    sendSelection(best, networkTransferRates[best]);
                    break;
                }
                case 1:{
                    doTOPSIS();
                    int best = ranks[0];
                    sendSelection(best, networkTransferRates[best]);
                    break;
                }
                case 2:{
                    int best = doMaxMin();
                    sendSelection(best, networkTransferRates[best]);
                    break;
                }
                case 3:{
                    int best_SAW = doSAW();
                    doTOPSIS();
                    int best_TOPSIS = ranks[0];
                    int best_MaxMin = doMaxMin();
                    int best_count[3];
                    for(int i=0;i < 3;i++){
                        best_count[i]=0;
                    }
                    for(int i=0; i < 3; i++){
                        if(i == best_SAW){
                           best_count[i]++;
                        }
                        else if(i == best_TOPSIS){
                            best_count[i]++;
                        }
                        else if(i == best_MaxMin){
                            best_count[i]++;
                        }
                    }
                    int best=0;
                    for(int i=0; i < 3; i++){
                        if(best < best_count[i]){
                            best = i;
                        }
                    }
                    sendSelection(best, networkTransferRates[best]);
                    break;
                }
                default:{
                    std::cout << "Error selecting MADM algorithm" <<endl;
                    exit(1);
                }
                }
            }
        }
    }
}


