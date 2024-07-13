#include "radioCondGen.h"
#include <random>

#include <omnetpp/csimplemodule.h>
Define_Module(radioCondGen);



void radioCondGen::initialize()
{
    sendMessageEvent = new cMessage("start");
    cellLoadTime = par("radioTimeModifier").doubleValue();
    meanNormal = par("meanForRadioNormaldistriburion").doubleValue();
    deviationNormal = par("radioDeviationNormalValue").doubleValue();

    initRadioTransferCapacity = par("init_radio_transfer_capacity").doubleValue()*1000.0; //to measure in kilo
    cMessage *dataR = new cMessage("dataR");
    dataR->addPar("PossibleTransferRate");
    dataR->par("PossibleTransferRate") = initRadioTransferCapacity;
    send(dataR,"txRCondGen");





    //set the random number generator under normal distribution
    std::random_device rd; //can be used for full random for each simulation
    unsigned int seed = 10;
    //generator.seed(rd());
    generator.seed(seed);
    normal_dist = std::normal_distribution<double>(meanNormal, deviationNormal);
    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);

}

void radioCondGen::handleMessage(cMessage *msg)
{

    double randomness = normal_dist(generator);

    // Ensure the randomness is within the range [32, 384] kBps
    randomness = std::max(32.0, std::min(384.0, randomness));
    //double randomness = 256.0; //temp
    cMessage *dataR = new cMessage("dataR");
    dataR->addPar("PossibleTransferRate");
    dataR->par("PossibleTransferRate") = randomness*1000;  //to correctly measure the transfer rate in kilo
    send(dataR,"txRCondGen");

    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);


}
