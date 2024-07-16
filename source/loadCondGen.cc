#include "headers/loadCondGen.h"

#include <random>

#include <omnetpp/csimplemodule.h>
Define_Module(loadCondGen);



void loadCondGen::initialize()
{
    sendMessageEvent = new cMessage("start");

    cellLoadTime = par("cellLoadTimeModifier").doubleValue();

    cMessage *dataL = new cMessage("dataL");
    init_cell_transfer_capacity = par("init_cell_transfer_capacity").doubleValue()*1000.0; //to measure in kilo
    dataL->addPar("CurrentCellLoad");
    dataL->par("CurrentCellLoad") = init_cell_transfer_capacity;
    send(dataL,"txCondGen");

    mean = par("meanForCellNormaldistriburion").doubleValue();
    deviation = par("deviationCellNormalDistribution").doubleValue();

    std::random_device rd; //can be used for full random for each simulation
    // Initialize random number generator
    //unsigned int seed = 10;
    generator.seed(rd());
    //generator.seed(seed);
    normal_dist = std::normal_distribution<double>(mean, deviation);

    scheduleAt(simTime(), sendMessageEvent);

}

void loadCondGen::handleMessage(cMessage *msg)
{

    double randomness = normal_dist(generator);

    // Make sure the randomness is within the range [32, 384] kbps
    randomness = std::max(32.0, std::min(384.0, randomness));
    randomness = randomness*1000;

    //double randomness = 256.0; //temp
    cMessage *dataL = new cMessage("dataL");
    dataL->addPar("CurrentCellLoad");
    dataL->par("CurrentCellLoad") = 1024000000 - randomness; //to correctly measure in kilo
    send(dataL,"txCondGen");

    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);


}
