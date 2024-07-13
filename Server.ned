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

simple Server{
    parameters:
        int networkSize;
        double simCycle @unit(ms) = default(10 ms);
    
    gates:
        input rxPackets; //file splits recieved from Source
        //input rxFuzzyAlg; //data from LFA
        //input rxcsAlg;		//data from Consumer Selection Alg
        input rxDestEOF;	//EOF message from the destination
        input rxNetwork[networkSize]; //to recieve transfer rate from each network
        output txSourceEOF;  //EOF message to generator to restart the file generator
        //output txFuzzy; //communication to fuzzy
        //output txCustomerSelectAlgorithm; //communication to cs algorithm
        output txNetwork[networkSize]; //data files sent to the network

        
}