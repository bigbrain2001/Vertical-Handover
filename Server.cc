#include "Server.h"

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
    queue_EOF.setName("queue_EOF");
    nrNetworks = par("networkSize").intValue();
    time_updates = par("simCycle").doubleValue();
    for(int i=0;i<nrNetworks;i++){
        networkTransferRates[i] = 0;
    }
    //scheduleAt(simTime() + fastest_transfer_rate_change_time, sendMessageEvent);
    //scheduleAt(simTime(), sendMessageEvent);

}

void Server::processQueues()
{
    // Find the fastest network
    int fastestNetwork = -1;
    double maxRate = 0;
    for (int i = 0; i < nrNetworks; i++) {
        if (networkTransferRates[i] > maxRate) {
            maxRate = networkTransferRates[i];
            fastestNetwork = i;
        }
    }

    // If a fastest network is found, send packets
    if (fastestNetwork != -1) {
        // Send packets from the main queue first
        if (!queue.isEmpty()) {
            cMessage *file_part = (cMessage *)queue.pop();
            int packetSizee = (int)file_part->par("packetSize");
            double transfer_rate = (double)packetSizee/maxRate;
            if(file_part->hasPar("packetSize")){
                EV << "COADA HAS IT" << endl;
            }
            send(file_part,"txNetwork",fastestNetwork);
            scheduleAt(simTime() + transfer_rate,sendMessageEvent);
        }
        else{
            // Send the last file element stored in queue_EOF
            if(!queue_EOF.isEmpty()){
                cMessage *file_part = (cMessage *)queue_EOF.pop();
                int packetSizee = (int)file_part->par("packetSize");
                double transfer_rate = (double)packetSizee/maxRate;
                EV << "Sent EOF" << endl;
                if(file_part->hasPar("packetSize")){
                    EV << "EOF HAS IT" << endl;
                }
                send(file_part,"txNetwork",fastestNetwork);
                scheduleAt(simTime() + transfer_rate,sendMessageEvent);
            }
        }



    }
}


void Server::handleMessage(cMessage *msg)
{
    if(msg->arrivedOn("rxPackets")){
        int endfile = (int)msg->par("EOF");
        if(endfile == 0){
            queue.insert(msg);
        }
        else {
            queue_EOF.insert(msg);
        }
        if(init_contor >= nrNetworks-1){
            if(!(sendMessageEvent->isScheduled())){
                scheduleAt(simTime(),sendMessageEvent);
            }
        }

    }
    else if(msg->arrivedOn("rxNetwork")){
        for(int i=0;i < nrNetworks;i++){
            if(msg->arrivedOn("rxNetwork",i)){
                double transfer = (double)msg->par("transferRate");
                networkTransferRates[i] = transfer;
                EV << "Transfer rate network " << i << " " << networkTransferRates[i] <<endl;
                delete msg;
                if(init_contor < nrNetworks-1){
                    init_contor++;
                }
                else{
                    if(!(sendMessageEvent->isScheduled())){
                        scheduleAt(simTime(),sendMessageEvent);
                    }
                }

            }
        }
    }
    else if(msg->arrivedOn("rxDestEOF")){
        send(msg,"txSourceEOF");//restarts the generator
    }
    if(msg->isSelfMessage()){
        processQueues();
    }

}

