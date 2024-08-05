#include "headers/destination.h"
#include <string>
#include <omnetpp/csimplemodule.h>
#include <fstream>
#include <iostream>
#include <iomanip>
Define_Module(Destination);



void Destination::initialize()
{
    lifetimeSignal = registerSignal("lifetime");
    lifetimeSignal_per_file = registerSignal("lifetimeSignal_per_file");
    netSize = par("networkSize").intValue();
    algorithm_selected = par("selection").intValue();
    net_load_selected = par("net_load_select").intValue();
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
        if(currentFile >= FILE_SIZE_-1){ //we need to count latest FILE_SIZE_ files
            std::cout << "Too many files sent to gather data, expand the memory buffer" << endl;
            exit(1);//delete msg;
            currentFile = 0;
        }
        cMessage *startFileTransfer = new cMessage("file_recieved");
        send(startFileTransfer,"txSourceEOF");

    }

    delete msg;

    //used to collect data of all IP packets for all networks

    //int size = (int)msg->par("packetSize");




}

void Destination::write_statistics(const std::string& file_name_net,const std::string& file_name_files){
    std::ofstream name(file_name_net, std::ios::app); //(name, append)
    for (int i = 0; i < netSize; i++) {
        name << i << ", "
                << std::fixed << std::setprecision(4) << lifetimeStats[i].getMean() << ", " ///fixed -> writes the data normally, without scientific number
                << std::fixed << std::setprecision(2) << lifetimeStats[i].getMin() << ", " //precision is similar to %.2f and such
                << std::fixed << std::setprecision(2) << lifetimeStats[i].getMax() << ", "
                << lifetimeStats[i].getCount() << std::endl;
    }

    std::ofstream name2(file_name_files, std::ios::app);
    for(int i = 0; i < FILE_SIZE_; i++){
        if(lifetimeStats_per_file[i].getCount() == 0){
            break;
        }
        else{
            name2 << i << ", "
                    << std::fixed << std::setprecision(4) << lifetimeStats_per_file[i].getMean() << ", " ///fixed -> writes the data normally, without scientific number
                    << std::fixed << std::setprecision(2) << lifetimeStats_per_file[i].getMin() << ", " //precision is similar to %.2f and such
                    << std::fixed << std::setprecision(2) << lifetimeStats_per_file[i].getMax() << ", "
                    << std::fixed << std::setprecision(2) << (((double)lifetimeStats_per_file[i].getMean()/(double)fileSize[i])*1000000.0) << ", "
                    << fileSize[i] << std::endl;

        }
    }
}


void Destination::finish(){
    if(algorithm_selected == 0){ //MADM
        switch(net_load_selected){
        case 0:{ //Last netLoad selection
            std::string network = "results_txt/MADM_results_networks_LAST.txt";
            std::string file = "results_txt/MADM_results_files_LAST.txt";
            write_statistics(network, file);
            break;
        }
        case 1:{ //average network load selection
            std::string network = "results_txt/MADM_results_networks_AVG.txt";
            std::string file = "results_txt/MADM_results_files_AVG.txt";
            write_statistics(network, file);
            break;
        }

        default:
            std::cout << "ERROR WRITING SELECTING NETLOAD ALGORITHM" << endl;
            exit(1);
        }




    } else{ //FUZZY
        switch(net_load_selected){
        case 0:{ //Last netLoad selection
            std::string network = "results_txt/FUZZY_results_networks_LAST.txt";
            std::string file = "results_txt/FUZZY_results_files_LAST.txt";
            write_statistics(network, file);
            break;
        }
        case 1:{ //average network load selection
            std::string network = "results_txt/FUZZY_results_networks_AVG.txt";
            std::string file = "results_txt/FUZZY_results_files_AVG.txt";
            write_statistics(network, file);
            break;
        }

        default:
            std::cout << "ERROR WRITING SELECTING NETLOAD ALGORITHM" << endl;
            exit(1);
        }
    }
}

