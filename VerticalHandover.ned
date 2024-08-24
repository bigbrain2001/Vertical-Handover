//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

import Source;
import Server;
import Destination;
import Network;
import MADM_Algorithm;
import FLC;

network VerticalHandover
{
    parameters:
        int networkSize;
        int UMTS_capacity;
        int LTE_capacity;
        int selection;
        int file_Size_min;
        int file_Size_max;
        int net_load_select;
        int MADM_selection;
        @display("bgb=484,364");
    submodules:
        Source: Source {
            @display("p=37,164;i=block/source");
        }
        Server: Server {
            selection = parent.selection;
            networkSize = parent.networkSize;
            @display("p=127,164;i=block/buffer");
        }
        dest: Destination {
            selection = parent.selection;
            net_load_select = parent.net_load_select;
            networkSize = parent.networkSize;
            MADM_selection = parent.MADM_selection;
            @display("p=436,164;i=block/sink");
        }
        UMTS1: Network { //the good UMTS network
            UMTS_capacity = parent.UMTS_capacity; //1024Mb
            net_load_select = parent.net_load_select;
            @display("p=280,31;i=block/network");
        }
        UMTS2: Network { //the bad UMTS network
            UMTS_capacity = parent.UMTS_capacity; //1024Mb
            net_load_select = parent.net_load_select;
            @display("p=280,106;i=block/network");
        }
        LTE: Network { //The 4G network
            UMTS_capacity = parent.LTE_capacity; //10240Mb
            net_load_select = parent.net_load_select;
            @display("p=280,255;i=block/network2");
        }
        MADM_ALG: MADM_Algorithm {
            networkSize = parent.networkSize;
            MADM_selection = parent.MADM_selection;
            @display("p=127,294;i=block/classifier");
        }
        fuzzySelAlg: FLC {
            networkSize = parent.networkSize;
            @display("p=127,50;i=block/classifier2");
        }
    connections:

        Source.txPackets --> Server.rxPackets; //Generating Data to the server and storing it in a buffer
        fuzzySelAlg.txServer --> Server.rxFuzzyAlg; //receives the selection from fuzzy alg
        dest.txSourceEOF --> Server.rxDestEOF; //send a message to the generator that the whole file(all packets) has been recieved and to start sending a new one
        Server.txSourceEOF --> Source.rxEOF; //restarts the file transfer
        MADM_ALG.txSelection --> Server.rxAlg;	//receives the selection from the MADM algorithms
		//Network conenctions
		//Sends the IP packets through the network first
        Server.txNetwork[0] --> UMTS1.rxDataServer;
        Server.txNetwork[1] --> UMTS2.rxDataServer;
        Server.txNetwork[2] --> LTE.rxDataServer;
		//Updates the rates to the server, so it can be sent to the selection algorithms
        UMTS1.txCondServer --> Server.rxNetwork[0];
        UMTS2.txCondServer --> Server.rxNetwork[1];
        LTE.txCondServer --> Server.rxNetwork[2];
        //Sends the data sent to network to it's destination
        UMTS1.txDestination --> dest.rxData[0];
        UMTS2.txDestination --> dest.rxData[1];
        LTE.txDestination --> dest.rxData[2];
        //Algorithm Connections
        for i=0..networkSize - 1 {
            Server.txFuzzy[i] --> fuzzySelAlg.rxServer[i];
            Server.txAlgorithm[i] --> MADM_ALG.rxTransferRates[i];
        }
}
