#include "headers/radioCondGen.h"

#include <random>

#include <omnetpp/csimplemodule.h>
Define_Module(radioCondGen);

radioCondGen::radioCondGen()
{
    sendMessageEvent = nullptr;
}

radioCondGen::~radioCondGen()
{
    cancelAndDelete(sendMessageEvent);
}




void radioCondGen::initialize()
{
    sendMessageEvent = new cMessage("start");
    cellLoadTime = par("radioTimeModifier").doubleValue();
    meanNormal = par("meanForRadioNormaldistriburion").doubleValue();
    deviationNormal = par("radioDeviationNormalValue").doubleValue();
    seed = par("seed").intValue();

    radio_min = par("TransferRate_Min").doubleValue();
    radio_max = par("TransferRate_Max").doubleValue();

    initRadioTransferCapacity = par("init_radio_transfer_capacity").doubleValue()*1000.0; //to measure in kilo
    if(initRadioTransferCapacity < 8000){
        initRadioTransferCapacity = 8000; //for a minimum of 8kbps
    }
    cMessage *dataR = new cMessage("dataR");
    dataR->addPar("PossibleTransferRate");
    dataR->par("PossibleTransferRate") = initRadioTransferCapacity;
    send(dataR,"txRCondGen");





    //set the random number generator under normal distribution
    //std::random_device rd; //can be used for full random for each simulation
    //unsigned int seed = 42;
    generator.seed(seed);
    //generator.seed(seed);
    normal_dist = std::normal_distribution<double>(meanNormal, deviationNormal);
    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);

}

void radioCondGen::handleMessage(cMessage *msg)
{
    double randomness = normal_dist(generator);
    // Ensure the randomness is within the range [radio_min, radio_max] kbps
    randomness = std::max(radio_min, std::min(radio_max, randomness));
    cMessage *dataR = new cMessage("dataR");
    dataR->addPar("PossibleTransferRate");
    double data = randomness*1000; //kilo
    if(data < 8000){ //set a minimum of 8kbps transfer Rate
        data = 8000;
    }
    dataR->par("PossibleTransferRate") = data;
    send(dataR,"txRCondGen");
    scheduleAt(simTime()+cellLoadTime, sendMessageEvent);
}
