#ifndef HEADERS_SERVER_H_
#define HEADERS_SERVER_H_

#include <omnetpp.h>
#include <float.h>
using namespace omnetpp;

class Server : public cSimpleModule
{
private:
    cMessage *sendMessageEvent;
    //double MAX_SIM;
    int file_Size;
    int file_split_size;


  public:
    Server();
    virtual ~Server();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void processQueues(double transfer, int retea);
    void finish();
    cQueue queue;
    int nrNetworks;
    double networkTransferRates[20];
    bool waiting_for_Alg = true;
    double transfer_rate;
    int chosenNet;
    int alg_select;
    //double fastest_transfer_rate_change_time = DBL_MAX;
    //double changeTime[20];
};



#endif /* HEADERS_SERVER_H_ */
