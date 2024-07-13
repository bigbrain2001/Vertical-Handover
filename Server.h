#ifndef SERVER_H_
#define SERVER_H_

#include <omnetpp.h>
#include <float.h>
using namespace omnetpp;

class Server : public cSimpleModule
{
private:
    cMessage *sendMessageEvent;
    double MAX_SIM;
    int file_Size;
    int file_split_size;


  public:
    Server();
    virtual ~Server();
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void processQueues();
    cQueue queue;
    cQueue queue_EOF;
    int nrNetworks;
    double networkTransferRates[10];
    double time_updates;
    double time_min;
    int init_contor=0;
    //double fastest_transfer_rate_change_time = DBL_MAX;
    //double changeTime[20];
};



#endif /* SERVER_H_ */
