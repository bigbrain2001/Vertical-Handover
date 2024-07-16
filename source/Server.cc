#include "headers/Server.h"

#include <omnetpp/csimplemodule.h>
Define_Module(Server);

Server::Server()
{
    sendMessageEvent = nullptr;
}

Server::~Server()
{
    cancelAndDelete(sendMessageEvent);
}


void Server::initialize()
{
    sendMessageEvent = new cMessage("start");
    queue.setName("queue");
    nrNetworks = par("networkSize").intValue();
    for(int i=0;i<nrNetworks;i++){
        networkTransferRates[i] = 0;
    }
    //scheduleAt(simTime() + fastest_transfer_rate_change_time, sendMessageEvent);
    //scheduleAt(simTime(), sendMessageEvent);

}

void Server::processQueues(double transfer, int retea)
{

    // If a fastest network is found, send packets
    if (retea != -1) {
        // Send packets from the queue with a transfer rate of packetSize/transferRate
        if (!queue.isEmpty()) {
            cMessage *file_part = (cMessage *)queue.pop();
            int packetSizee = (int)file_part->par("packetSize");
            double transfer_rate = (double)packetSizee/transfer;
            send(file_part,"txNetwork",retea);
            scheduleAt(simTime() + transfer_rate,sendMessageEvent);
        }

    }
}


void Server::handleMessage(cMessage *msg)
{
    if(msg->arrivedOn("rxPackets")){
            queue.insert(msg);
        if(init_contor >= nrNetworks-1){
            if(!(sendMessageEvent->isScheduled())){
                scheduleAt(simTime(),sendMessageEvent);
            }
        }

    }
    else if(msg->arrivedOn("rxNetwork")){
        for(int i=0;i < nrNetworks;i++){
            if(msg->arrivedOn("rxNetwork",i)){
                send(msg,"txAlgorithm",i);
                //double transfer = (double)msg->par("transferRate");
                //networkTransferRates[i] = transfer;
                //EV << "Transfer rate network " << i << " " << networkTransferRates[i] <<endl;
                //delete msg;
                //if(init_contor < nrNetworks-1){
                    //init_contor++;
                //}
                //else{
                    //if(!(sendMessageEvent->isScheduled())){
                        //scheduleAt(simTime(),sendMessageEvent);
                    //}
                //}

            }
        }
    }
    else if(msg->arrivedOn("rxDestEOF")){
        send(msg,"txSourceEOF");//restarts the generator
    }
    else if(msg->arrivedOn("rxAlg")){
        waiting_for_Alg = false;
        transfer_rate = (double)msg->par("Transfer_Rate");
        chosenNet = (int)msg->par("select_network");
        delete msg;
        if(!(sendMessageEvent->isScheduled())){
            scheduleAt(simTime(),sendMessageEvent);
        }
    }
    if(msg->isSelfMessage()){
        processQueues(transfer_rate, chosenNet);
    }

}

