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

module NetworkUMTS{
    parameters:
     //double radioTimeModifier @unit(s); //time for reschedule of the module
     //double cellLoadTimeModifier @unit(s);
     
     //double meanForCellNormaldistriburion;
     //double meanForRadioNormaldistriburion;
     
     //double radioDeviationNormalValue;
     //double deviationCellNormalDistribution;
     
 	 //double init_radio_transfer_capacity;
 	 //double init_cell_transfer_capacity;
 	 
 	 int UMTS_capacity;
 	 
 		
 	gates:
 	    //input rxCondServer; //for updating loadCondGen
 	    input rxDataServer; //to recieve the data on the network
 	    output txCondServer; //to give the server the transfer rate for the selection
 	    output txDestination;   //to send the data further to the destination
 	submodules:
 	    loadCondition: loadCondGen{
 	        //cellLoadTimeModifier = parent.cellLoadTimeModifier;
 	        //meanForCellNormaldistriburion = parent.meanForCellNormaldistriburion;
 	        //deviationCellNormalDistribution = parent.deviationCellNormalDistribution;
 	        UMTS_capacity = parent.UMTS_capacity;
 	        
 	    }    
 	    radioCondition: radioCondGen{
 	        //radioTimeModifier = parent.radioTimeModifier;
 	        //radioDeviationNormalValue = parent.radioDeviationNormalValue;
 	        //meanForRadioNormaldistriburion = parent.meanForRadioNormaldistriburion;
 	        //init_radio_transfer_capacity = parent.init_radio_transfer_capacity;
 	    }
 	    extConditionAnalyzer: extCondAnalyzer{
 	        //init_cell_tc = parent.init_cell_transfer_capacity;
 	        //init_radio_tc = parent.init_radio_transfer_capacity;
 	        UMTS_capacity = parent.UMTS_capacity;
 	    }
 	    
    connections:
        //rxCondServer --> loadCondition.rxCondGen; //if chosen, will increase/decrease the load due to users
        loadCondition.txCondGen --> extConditionAnalyzer.rxLoad; //sends the cell load condition to the analyzer
        radioCondition.txRCondGen --> extConditionAnalyzer.rxRadio; //sends the radio condition to the analyzer
        extConditionAnalyzer.txDestination --> txDestination; //sends the data to destination
        rxDataServer --> extConditionAnalyzer.rxServer; //recieves the data to transfer to destination
        extConditionAnalyzer.txServer --> txCondServer; //sends the analyzed transfer rate to the server
        
}