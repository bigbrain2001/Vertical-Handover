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

simple Source{
    parameters:
        double MAX_SIM_TIME @unit(ms) = default(100000 ms);
        double simCycle @unit(ms) = default(0.1 ms);
        //MaximumTransmissionUnit - 1,500 bytes is the maximum MTU size. Expected 1400 - 1500;
        int fileSplitSize; //bytes
        int fileSize; //bytes
        int file_Size_min;
        int file_Size_max;
    
    
    gates:
        output txPackets; //packets to server
        input rxEOF; // EOF message from server
}