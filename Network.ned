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

import loadCondGen;
import radioCondGen;
import extCondAnalyzer;

module Network
{
    parameters:
        int net_load_select;
        int UMTS_capacity;
        double battery_consumption;
        double radio_min;
        double radio_max;
        double cell_min;
        double cell_max;
        double init_radio;
        double init_cell;
        double radio_mean;
        double radio_deviation;
        double cell_mean;
        double cell_deviation;
        double radio_cycle @unit(s) = default(1s);
        double cell_cycle @unit(s) = default(1s);

        @display("bgb=300.9,275.3333");
    gates:
        input rxDataServer; //to recieve the data on the network
        output txCondServer; //to give the server the transfer rate for the selection
        output txDestination;   //to send the data further to the destination
    submodules:
        loadCondition: loadCondGen {
            UMTS_capacity = parent.UMTS_capacity;
            TransferRate_Min = parent.cell_min;
            TransferRate_Max = parent.cell_max;
            init_cell_transfer_capacity = parent.init_cell;
            cellLoadTimeModifier = parent.cell_cycle;
            meanForCellNormaldistriburion = parent.cell_mean;
            deviationCellNormalDistribution = parent.cell_deviation;
            @display("p=71.78333,75.71667;i=block/cogwheel");
        }
        radioCondition: radioCondGen {
            TransferRate_Min = parent.radio_min;
            TransferRate_Max = parent.radio_max;
            init_radio_transfer_capacity = parent.init_radio;
            meanForRadioNormaldistriburion = parent.radio_mean;
            radioDeviationNormalValue = parent.radio_deviation;
            radioTimeModifier = parent.radio_cycle;
            @display("p=220.26666,75.71667;i=block/cogwheel");
        }
        extConditionAnalyzer: extCondAnalyzer {
            net_load_select = parent.net_load_select;
            UMTS_capacity = parent.UMTS_capacity;
            battery_consumption = parent.battery_consumption;
            @display("p=145.53333,153.4;i=block/join");
        }

    connections:
        loadCondition.txCondGen --> extConditionAnalyzer.rxLoad; //sends the cell load condition to the analyzer
        radioCondition.txRCondGen --> extConditionAnalyzer.rxRadio; //sends the radio condition to the analyzer
        extConditionAnalyzer.txDestination --> txDestination; //sends the data to destination
        rxDataServer --> extConditionAnalyzer.rxServer; //recieves the data to transfer to destination
        extConditionAnalyzer.txServer --> txCondServer; //sends the analyzed transfer rate to the server

}
