#include "headers/destination.h"

#include <omnetpp/csimplemodule.h>
Define_Module(Destination);



void Destination::initialize()
{
    lifetimeSignal = registerSignal("lifetime");
    lifetimeSignal_per_file = registerSignal("lifetimeSignal_per_file");
    netSize = par("networkSize").intValue();
    first_pack = 0;
}

void Destination::handleMessage(cMessage *msg)
{

    simtime_t lifetime = simTime() - msg->getCreationTime();

    for(int i=0;i < netSize; i++){
        if(msg->arrivedOn("rxData",i)){
            EV << "Message arrived on gate: " << i << endl;
            filesize += (int)msg->par("packetSize");
            lifetimeStats[i].collect(lifetime);
        }
    }

    emit(lifetimeSignal, lifetime);
        int endOfFile = (int)msg->par("EOF");
            EV << "Received " << endOfFile << endl;
            if(endOfFile == 1){
                lifetimeStats_per_file[currentFile].collect(lifetime); //collect mean_delay for the file
                fileSize[currentFile] = filesize;
                currentFile++;
                filesize=0;
                emit(lifetimeSignal_per_file,lifetime);
                if(currentFile >= 10){ //we need to count latest 10 files
                    endSimulation();
                }
                cMessage *startFileTransfer = new cMessage("file_recieved");
                send(startFileTransfer,"txSourceEOF");

            }

    delete msg;

    //used to collect data of all IP packets for all networks

    //int size = (int)msg->par("packetSize");




}


void Destination::finish(){
    FILE *networkR;
    networkR = fopen("results_networks.txt", "a");
    for(int i = 0; i < netSize; i++){
        fprintf(networkR, "%d, %.4f, %.2f, %.2f, %d\n",  i, lifetimeStats[i].getMean(), lifetimeStats[i].getMin(), lifetimeStats[i].getMax(), lifetimeStats[i].getCount() ); ///import csv in excel si se baga automan in excel
        //char buf[40];
        //sprintf(buf, "my lifetime user%d", i);
        //lifetimeStats[i].recordAs(buf);
    }
    //fprintf(fp, "%.2f, %.3f\n", NetworkLoad, sendingTime);
    fclose(networkR);

    FILE *fileR;
    fileR = fopen("results_files.txt", "a");
    for(int i = 0; i < 10; i++){
        fprintf(fileR, "%d, %.4f, %.2f, %.2f, %.2f, %d\n",  i, lifetimeStats_per_file[i].getMean(), lifetimeStats_per_file[i].getMin(), lifetimeStats_per_file[i].getMax(), (double)lifetimeStats_per_file[i].getMean()/(double)fileSize[i] ,fileSize[i] ); ///import csv in excel si se baga automan in excel
    }
    fclose(fileR);
}
