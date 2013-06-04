Sun_Tracking_Control
===============

Converts sun angles and drives two linear motors for sun-tracking.

Copyright (C) 2013  Felix Gottwald, HTW Dresden

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>

contact:
info@labor-elektrische-mobilitaet.de

#### main features:
- calculating step count from given sun angles 
- evaluates hall-sensor-controlled step counter 
- drives half-bridges for linear motors to given angles
- Safety functions 

#### Communication protocol: 

* i²c slave adresse 			`0x13`
* ERROR CODES: 
    * _ERR\_STAUS\_FLAG_		`0x50`
    * _ERR\_ILLEGAL\_END_		`0x51`
    * _ERR\_STEPCOUNTER\_OVERFLOW_	`0x52`
* COMMANDS:
    * _CMD\_SET\_ANGLE_	 		`0x02`
    * _CMD\_GOTO\_REFERENCE_ 		`0x04`
    * _CMD\_STOP_			`0x06`
    * _CMD\_RESET_			`0x08`
    * _CMD\_TERMINATED_			`0x33`
* I²C Register Definition:


register | meaning
1 | vertical cmd/err
2 | horizontal cmd/err
3 | vertical data
4 | horizontal data


#### Used devices: 
* ATMega88(Atmel) 
* 33926(freescale) 
* SM4S520M1(SatControl)

#### Documentation and Schematics:
* http://wiki.labor-elektrische-mobilitaet.de/index.php/LEV_charge:Sonnenstandsnachf%C3%BChrungseinrichtung
* http://wiki.labor-elektrische-mobilitaet.de/index.php/Motorsteuerung

