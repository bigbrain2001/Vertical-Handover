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

simple radioCondGen{
 parameters:
     double radioTimeModifier @unit(s); //time for reschedule of the module
     double meanForRadioNormaldistriburion;
     double radioDeviationNormalValue;
 	 double init_radio_transfer_capacity;
 gates:
 output txRCondGen;   
    
    
}