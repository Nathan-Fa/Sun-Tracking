Sun_Tracking_Control
===============

Sun_Tracking_Control converts sun angles to motor steps and drives two linear motors for sun-tracking

#######################################################################

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

contact: gottwald@htw-dresden.de

#######################################################################

features:
- calculating step count from given sun angles 
- evaluating motor-step-count from hall-sensors 
- drives linear motors to given angles
- Safety functions 

#######################################################################

Used device: ATMega88

Documentation and schematics can be found at: 

http://wiki.labor-elektrische-mobilitaet.de/index.php/LEV_charge:Sonnenstandsnachf%C3%BChrungseinrichtung

http://wiki.labor-elektrische-mobilitaet.de/index.php/Motorsteuerung
