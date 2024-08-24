#include "headers/Source.h"

#include <omnetpp/csimplemodule.h>
Define_Module(Source);

Source::Source()
{
    sendMessageEvent = nullptr;
}

Source::~Source()
{
    cancelAndDelete(sendMessageEvent);
}


void Source::initialize()
{
    file_Size_min = par("file_Size_min").intValue();
    file_Size_max = par("file_Size_max").intValue();
    file_Size = (int)uniform(std::max(0, file_Size_min), std::max(1, file_Size_max+1));
    currentfileSize = file_Size*8; //converts to bits
    file_split_size = par("fileSplitSize").intValue(); //converts to bits
    file_split_size = std::max(0, std::min(1500, file_split_size))*8;
    MAX_SIM = par("MAX_SIM_TIME").doubleValue();
    sendMessageEvent = new cMessage("start");
    scheduleAt(simTime(), sendMessageEvent);
}

void Source::handleMessage(cMessage *msg)
{

    //stops the app at a predetermined simulation time (100k seconds for example)
    double simulationTime  = simTime().dbl();
    if(simulationTime >= MAX_SIM){
        delete msg;
        endSimulation();
    }
    //Splits the file into multiple IP packets over the course of self cycles and sends them to the storage element in the server
    else if(msg->isSelfMessage()){
        while(currentfileSize > 0){
            cMessage *file_part = new cMessage("file_part");
            //0 - not EOF, 1 - EOF and will result in getting a restart message of the generator
            file_part->addPar("EOF");
            //It splits the file into multiple parts and sends them to the storage element in the server
            file_part->addPar("packetSize");
            //if there is less or equal data than the total size split, sends the rest of the data in one IP packet and sets the EOF
            if(currentfileSize <= file_split_size){
                file_part->par("packetSize") = currentfileSize;
                file_part->par("EOF") = 1;
                currentfileSize = 0;
                send(file_part, "txPackets");
                cancelEvent(sendMessageEvent); //stop data transmission until further notice
                break;

            }
            //if not, send the IP packets as usual with no EOF and full IP Packet Size
            else{
                file_part->par("EOF") = 0;
                file_part->par("packetSize") = file_split_size;
                currentfileSize -= file_split_size;
                send(file_part, "txPackets");
            }


        }
    }
    else if(msg->arrivedOn("rxEOF")){
        file_Size = (int)uniform(std::max(0, file_Size_min), std::max(1, file_Size_max+1));
        currentfileSize = file_Size*8; //resets file size
        delete msg;
        scheduleAt(simTime(),sendMessageEvent); //restarts data transfer immediately
    }




}
