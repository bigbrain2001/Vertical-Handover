#include "headers/extCondAnalyzer.h"

#include <random>

#include <omnetpp/csimplemodule.h>
Define_Module(extCondAnalyzer);

double extCondAnalyzer::minimum(double cell, double radio){
    if(cell < radio){
        return cell;
    }
    else{
        return radio;
    }
}

void extCondAnalyzer::initialize()
{
    //sendMessageEvent = new cMessage("start");
    //condL = par("init_cell_tc;").doubleValue()*1000.0;
    //condR = par("init_radio_tc").doubleValue()*1000.0;
    capacity = par("UMTS_capacity").intValue();
    //cMessage *condition = new cMessage("Condition");
    //condition->addPar("transferRate");
    //condition->par("transferRate") = minimum(condL,condR);
    //send(condition,"txServer");
    //scheduleAt(simTime(), sendMessageEvent);

}

void extCondAnalyzer::sendTR(double cell, double radio){
    cMessage *condition = new cMessage("Condition");
    condition->addPar("netLoad");
    condition->par("netLoad") = ((minimum(cell,radio)*100.0) / (double)capacity); //network capacity in the meaning of %
    condition->addPar("transferRate");
    condition->par("transferRate") = std::max(8000.0,minimum(cell,radio)); //a minimum of 8kbps transfer rate
    send(condition,"txServer");
}


void extCondAnalyzer::handleMessage(cMessage *msg)
{

    if(msg->arrivedOn("rxRadio")){
        int dataR = (int)msg->par("PossibleTransferRate");
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
        int dataL = (int)msg->par("CurrentCellLoad");
        condL = dataL;
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
