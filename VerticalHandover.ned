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
import NetworkUMTS;
import MADM_SAW_Algorithm;
import FLC;

network VerticalHandover
{
    parameters:
        int networkSize;
        int UMTS_capacity;
        int selection;

        @display("bgb=337,284");
    submodules:
        Source: Source {

            @display("p=28,138");
        }
        Server: Server {
            selection = parent.selection;
            networkSize = parent.networkSize;
            @display("p=83,138");
        }

        dest: Destination {
            networkSize = parent.networkSize;
            @display("p=270,138");
        }

        networkUMTS[networkSize]: NetworkUMTS {
            UMTS_capacity = parent.UMTS_capacity; //1024Mb
            //@display("p=167,138");
        }

        SAW_ALG: MADM_SAW_Algorithm {
            networkSize = parent.networkSize;
            @display("p=83,212");
        }
        fuzzySelAlg: FLC {
			networkSize = parent.networkSize;
            @display("p=83,72");
        }


    connections:

        Source.txPackets --> Server.rxPackets; //Generating Data to the server and storing it in a buffer
        fuzzySelAlg.txServer --> Server.rxFuzzyAlg;



        dest.txSourceEOF --> Server.rxDestEOF; //send a message to the generator that the whole file(all packets) has been recieved and to start sending a new one
        Server.txSourceEOF --> Source.rxEOF;
        SAW_ALG.txSelection --> Server.rxAlg;


        for i=0..networkSize - 1 {
            Server.txFuzzy[i] --> fuzzySelAlg.rxServer[i];
                    
            Server.txNetwork[i] --> networkUMTS[i].rxDataServer;
            Server.txAlgorithm[i] --> SAW_ALG.rxTransferRates[i];
            networkUMTS[i].txCondServer --> Server.rxNetwork[i];
            networkUMTS[i].txDestination --> dest.rxData[i];
        }


}
