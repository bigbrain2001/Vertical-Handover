
#ifndef HEADERS_DESTINATION_H_
#define HEADERS_DESTINATION_H_
#define FILE_SIZE_ 2000000

#include <omnetpp.h>
#include <string>


using namespace omnetpp;

class Destination : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void write_statistics(const std::string& file_name_net,const std::string& file_name_files);
    void finish();
    //cMessage *sendMessageEvent;
    cHistogram lifetimeStats[20];
    cHistogram lifetimeStats_per_file[FILE_SIZE_];
    cOutVector file_delay;
    cOutVector packetDelay;
    cOutVector packets_per_network0;
    cOutVector packets_per_network1;
    cOutVector packets_per_network2;
    int fileSize[FILE_SIZE_];
    double nrPackets[3] = {0,0,0};
    int currentFile=0;
    int algorithm_selected;
    int net_load_selected;
    int MADM_selection;
    int netSize;
    int filesize=0;
    int total_IP_size=0;
};





#endif /* HEADERS_DESTINATION_H_ */
