#include "headers/extCondAnalyzer.h"

#include <random>

#include <omnetpp/csimplemodule.h>
Define_Module(extCondAnalyzer);

#define LAST_LOAD_NUMBER 5
#define SMOOTH_NETLOAD_FACTOR 0.4

double extCondAnalyzer::minimum(double cell, double radio){
    if(cell < radio){
        return cell;
    }
    else{
        return radio;
    }
}

double extCondAnalyzer::maximum(double cell, double radio){
    if(cell < radio){
        return radio;
    }
    else{
        return cell;
    }
}

void extCondAnalyzer::initialize()
{
    //sendMessageEvent = new cMessage("start");
    //condL = par("init_cell_tc;").doubleValue()*1000.0;
    //condR = par("init_radio_tc").doubleValue()*1000.0;
    capacity = par("UMTS_capacity").intValue();
    net_load_select = par("net_load_select").intValue();
    battery_consumption = par("battery_consumption").doubleValue();
    for(int i = 0;i < LAST_LOAD_NUMBER;i++){
        netLoad[i] = 0;
    }
    //cMessage *condition = new cMessage("Condition");
    //condition->addPar("transferRate");
    //condition->par("transferRate") = minimum(condL,condR);
    //send(condition,"txServer");
    //scheduleAt(simTime(), sendMessageEvent);

}

double extCondAnalyzer::calculate_cell_load(){
    double sum=0;
    for(int i=0;i<LAST_LOAD_NUMBER;i++){
        sum+= netLoad[i];
    }
    double average_cell = sum/LAST_LOAD_NUMBER;

    double res = SMOOTH_NETLOAD_FACTOR * average_cell + (1-SMOOTH_NETLOAD_FACTOR)*current_cap;
    return res;
}

void extCondAnalyzer::sendTR(double cell, double radio){
    cMessage *condition = new cMessage("Condition");
    condition->addPar("netLoad");
    if(net_load_select == 0){
        condition->par("netLoad") = (condL/(double)capacity)*100.0; //network capacity in the 0 - 100 %
    }
    else{
        condition->par("netLoad") = calculate_cell_load()*100.0; //network capacity in the 0 - 100%
    }

    condition->addPar("transferRate");
    condition->par("transferRate") = minimum(capacity,maximum(8000, minimum(condL,condR))); //also checks for a minimum of 8kbps transfer rate and a maximum of the cell capacity
    condition->addPar("battery_consumption");
    condition->par("battery_consumption") = battery_consumption;
    send(condition,"txServer");
}


void extCondAnalyzer::handleMessage(cMessage *msg)
{

    if(msg->arrivedOn("rxRadio")){
        double dataR = (double)msg->par("PossibleTransferRate");
        condR = dataR;
        delete msg;
        if(init_delay < 1){
            init_delay++;
        }
        else{
            sendTR(condL,condR);
        }



    }
    else if(msg->arrivedOn("rxLoad")){
        double dataL = (double)msg->par("CurrentCellLoad");
        condL = dataL; //transferRate
        if(condL > (double)capacity){
            condL = (double)capacity; //checks to not send more than maximum capacity
        }
        double loads = (condL/(double)capacity);
        netLoad[pointer] = loads; //a minimum of 8kbps transfer rate
        current_cap = netLoad[pointer];
        pointer++;
        if(pointer >= LAST_LOAD_NUMBER){
            pointer=0;
        }
        delete msg;
        if(init_delay < 1){
            init_delay++;
        }
        else{
            sendTR(condL,condR);
        }

    }
    //sends the IP packet to it's destination
    else if (msg->arrivedOn("rxServer")){
            send(msg,"txDestination");

    }

}


