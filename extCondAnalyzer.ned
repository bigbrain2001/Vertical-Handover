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



simple extCondAnalyzer{
 
 parameters:
 	//double init_cell_tc;
 	//double init_radio_tc;
 	int UMTS_capacity;
 gates:
     input rxLoad;
     input rxRadio;
     input rxServer;
     output txDestination; //sends the packets from the server through the netowrk to it's destination
	 output txServer; //sends the transfer rate to the server
     
     
}