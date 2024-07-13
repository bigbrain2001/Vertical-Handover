#include "Source.h"

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
    sendMessageEvent = new cMessage("start");
    file_Size = par("fileSize").intValue();
    currentfileSize = file_Size;
    file_split_size = 1500*8;
    //file_split_size = par("fileSize").intValue();
    //fileSize = uniform(1000000, 3000000) //random file size of 1MB - 3 MB
    //file_split_size = uniform(1400, 1500) //split each file into size 1400 - 1450 byte splits
    MAX_SIM = par("MAX_SIM_TIME").doubleValue();
    scheduleAt(simTime(), sendMessageEvent);

}

void Source::handleMessage(cMessage *msg)
{

    //stops the app at a predetermined simulation time (100k seconds for example)
    double simulationTime  = simTime().dbl();
    if(simulationTime >= MAX_SIM){
        endSimulation();
    }

    //Splits the file into multiple IP packets over the course of self cycles and sends them to the storage element in the server
    if(msg->isSelfMessage()){
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
                //cancelEvent(sendMessageEvent);
                //scheduleAt(simTime(),sendMessageEvent); //transmits data at a certain delay
                //break;

            }


        }
    }
    else if(msg->arrivedOn("rxEOF")){
        currentfileSize = file_Size; //resets file size
        delete msg;
        //cancelAndDelete(sendMessageEvent);
        //endSimulation();
        scheduleAt(simTime(),sendMessageEvent); //restarts data transfer immediately

    }




}
