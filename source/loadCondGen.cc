#include "headers/loadCondGen.h"

#include <random>

#include <omnetpp/csimplemodule.h>
Define_Module(loadCondGen);

loadCondGen::loadCondGen()
{
    sendMessageEvent = nullptr;
}

loadCondGen::~loadCondGen()
{
    cancelAndDelete(sendMessageEvent);
}


void loadCondGen::initialize()
{
    sendMessageEvent = new cMessage("start");
    capacity = par("UMTS_capacity").intValue();
    cell_min = par("TransferRate_Min").doubleValue();
    cell_max = par("TransferRate_Max").doubleValue();
    cellLoadTime = par("cellLoadTimeModifier").doubleValue();
    seed = par("seed").intValue();

    cMessage *dataL = new cMessage("dataL");
    init_cell_transfer_capacity = par("init_cell_transfer_capacity").doubleValue()*1000.0; //to measure in kilo
    dataL->addPar("CurrentCellLoad");
    double load;
    if(init_cell_transfer_capacity < 8000){
        load = 8000;
    }
    else{
        load = init_cell_transfer_capacity;
    }

    dataL->par("CurrentCellLoad") = load;
    send(dataL,"txCondGen");

    mean = par("meanForCellNormaldistriburion").doubleValue();
    deviation = par("deviationCellNormalDistribution").doubleValue();

    //std::random_device rd; //can be used for full random for each simulation
    // Initialize random number generator
    //unsigned int seed = 10;
    generator.seed(seed);
    //generator.seed(seed);
    normal_dist = std::normal_distribution<double>(mean, deviation);

    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);

}

void loadCondGen::handleMessage(cMessage *msg)
{

    double randomness = normal_dist(generator);

    // Make sure the randomness is within the range [32, 384] kbps
    randomness = std::max(cell_min, std::min(cell_max, randomness));

    //double randomness = 256.0; //temp
    cMessage *dataL = new cMessage("dataL");
    dataL->addPar("CurrentCellLoad");
    double load = (double)capacity - randomness*1000; //kilo
    if(load < 8){ //wants to avoid as much as possilble 100% network load
        load = 8;
    }
    if(load > (double)capacity){ //checks that there is no more allowed load than capacity
        load = (double)capacity;
    }
    dataL->par("CurrentCellLoad") = load*1000; //to correctly measure in kilo
    send(dataL,"txCondGen");

    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);


}
