#include "headers/destination.h"
#include <string>
#include <omnetpp/csimplemodule.h>
#include <omnetpp.h>
#include <fstream>
#include <iostream>
#include <iomanip>
Define_Module(Destination);



void Destination::initialize()
{

    packetDelay.setName("packetDelay");
    file_delay.setName("fileDelay");



    netSize = par("networkSize").intValue();
    algorithm_selected = par("selection").intValue();
    net_load_selected = par("net_load_select").intValue();
    MADM_selection = par("MADM_selection").intValue();
    packets_per_network0.setName("packets_per_network0");
    packets_per_network1.setName("packets_per_network1");
    packets_per_network2.setName("packets_per_network2");


}

void Destination::handleMessage(cMessage *msg)
{

    simtime_t lifetime = simTime() - msg->getCreationTime();
    int endOfFile = (int)msg->par("EOF");
    for(int i=0;i < netSize; i++){
        if(msg->arrivedOn("rxData",i)){
            EV << "Message arrived on gate: " << i << endl;
            int packetSize = (int)msg->par("packetSize");
            nrPackets[i]++;
            delete msg;
            filesize += packetSize;
            lifetimeStats[i].collect(lifetime);
            packetDelay.record(lifetime);


        }
    }


    EV << "Received " << endOfFile << endl;
    if(endOfFile == 1){
        lifetimeStats_per_file[currentFile].collect(lifetime); //collect mean_delay for the file
        file_delay.record(lifetime);
        fileSize[currentFile] = filesize;
        currentFile++;
        filesize=0;
        if(currentFile >= FILE_SIZE_-1){ //we need to count latest FILE_SIZE_ files
            std::cout << "Too many files sent to gather data, expand the memory buffer" << endl;
            exit(1);
        }
        cMessage *startFileTransfer = new cMessage("file_recieved");
        send(startFileTransfer,"txSourceEOF");
    }







}

void Destination::write_statistics(const std::string& file_name_net,const std::string& file_name_files){
    std::ofstream name(file_name_net, std::ios::app); //(name, append)
    for (int i = 0; i < netSize; i++) {
        if(lifetimeStats[i].getCount() == 0){
            name << i << ", "
                    << std::fixed << std::setprecision(4) << 0 << ", " ///fixed -> writes the data normally, without scientific number
                    << std::fixed << std::setprecision(2) << 0 << ", " //precision is similar to %.2f and such
                    << std::fixed << std::setprecision(2) << 0 << ", "
                    << lifetimeStats[i].getCount() << std::endl;
        }
        else{
        name << i << ", "
                << std::fixed << std::setprecision(4) << lifetimeStats[i].getMean() << ", " ///fixed -> writes the data normally, without scientific number
                << std::fixed << std::setprecision(2) << lifetimeStats[i].getMin() << ", " //precision is similar to %.2f and such
                << std::fixed << std::setprecision(2) << lifetimeStats[i].getMax() << ", "
                << lifetimeStats[i].getCount() << std::endl;
        }
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
    name.close();
    name2.close();
}


void Destination::finish(){
    for(int i=0;i<netSize;i++){
        switch(i){
        case 0:{
            packets_per_network0.record(nrPackets[0]);
            break;
        }
        case 1:{
            packets_per_network1.record(nrPackets[1]);
            break;
        }
        case 2:{
            packets_per_network2.record(nrPackets[2]);
            break;
        }
        default:{
            std::cout << "Add more vectors to gather more data for networks" << endl;
            exit(1);
        }
        }
    }
    if(algorithm_selected == 0){ //MADM
        switch(MADM_selection){
        case 0:{
            switch(net_load_selected){
            case 0:{ //Last netLoad selection
                std::string network = "results_txt/SAW_results_networks_LAST.txt";
                std::string file = "results_txt/SAW_results_files_LAST.txt";
                write_statistics(network, file);
                break;
            }
            case 1:{ //average network load selection
                std::string network = "results_txt/SAW_results_networks_AVG.txt";
                std::string file = "results_txt/SAW_results_files_AVG.txt";
                write_statistics(network, file);
                break;
            }

            default:
                std::cout << "ERROR WRITING SELECTING NETLOAD ALGORITHM" << endl;
                exit(1);
            }
            break;
        }

        case 1:{
            switch(net_load_selected){
            case 0:{ //Last netLoad selection
                std::string network = "results_txt/TOPSIS_results_networks_LAST.txt";
                std::string file = "results_txt/TOPSIS_results_files_LAST.txt";
                write_statistics(network, file);
                break;
            }
            case 1:{ //average network load selection
                std::string network = "results_txt/TOPSIS_results_networks_AVG.txt";
                std::string file = "results_txt/TOPSIS_results_files_AVG.txt";
                write_statistics(network, file);
                break;
            }

            default:
                std::cout << "ERROR WRITING SELECTING NETLOAD ALGORITHM" << endl;
                exit(1);
            }
            break;
        }
        case 2:{
            switch(net_load_selected){
            case 0:{ //Last netLoad selection
                std::string network = "results_txt/MAXMIN_results_networks_LAST.txt";
                std::string file = "results_txt/MAXMIN_results_files_LAST.txt";
                write_statistics(network, file);
                break;
            }
            case 1:{ //average network load selection
                std::string network = "results_txt/MAXMIN_results_networks_AVG.txt";
                std::string file = "results_txt/MAXMIN_results_files_AVG.txt";
                write_statistics(network, file);
                break;
            }

            default:{
                std::cout << "ERROR WRITING SELECTING NETLOAD ALGORITHM" << endl;
                exit(1);
            }
            }
            break;
        }
        case 3:{
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

            default:{
                std::cout << "ERROR WRITING SELECTING NETLOAD ALGORITHM" << endl;
                exit(1);
            }
            }
            break;
        }
        default:{
            std::cout << "ERROR WRITING SELECTING MADM ALGORITHM" << endl;
            exit(1);
        }
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

        default:{
            std::cout << "ERROR WRITING SELECTING NETLOAD ALGORITHM" << endl;
            exit(1);
        }
        }
    }
}

