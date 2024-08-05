#include "headers/loadCondGen.h"

#include <random>

#include <omnetpp/csimplemodule.h>
Define_Module(loadCondGen);



void loadCondGen::initialize()
{
    sendMessageEvent = new cMessage("start");
    capacity = par("UMTS_capacity").intValue();

    cellLoadTime = par("cellLoadTimeModifier").doubleValue();

    cMessage *dataL = new cMessage("dataL");
    init_cell_transfer_capacity = par("init_cell_transfer_capacity").doubleValue()*1000.0; //to measure in kilo
    dataL->addPar("CurrentCellLoad");
    double load;
    if(init_cell_transfer_capacity < 8){
        load = 8;
    }
    else{
        load = init_cell_transfer_capacity;
    }

    dataL->par("CurrentCellLoad") = load*1000;
    send(dataL,"txCondGen");

    mean = par("meanForCellNormaldistriburion").doubleValue();
    deviation = par("deviationCellNormalDistribution").doubleValue();

    std::random_device rd; //can be used for full random for each simulation
    // Initialize random number generator
    //unsigned int seed = 10;
    generator.seed(rd());
    //generator.seed(seed);
    normal_dist = std::normal_distribution<double>(mean, deviation);

    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);

}

void loadCondGen::handleMessage(cMessage *msg)
{

    double randomness = normal_dist(generator);

    // Make sure the randomness is within the range [32, 384] kbps
    randomness = std::max(32.0, std::min(384.0, randomness));

    //double randomness = 256.0; //temp
    cMessage *dataL = new cMessage("dataL");
    dataL->addPar("CurrentCellLoad");
    dataL->par("CurrentCellLoad") = (double)capacity - randomness*1000; //to correctly measure in kilo
    send(dataL,"txCondGen");

    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);


}
