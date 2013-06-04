/*********************************************************************
Sun_Tracking_Control/main.h

Converts sun angles and drives two linear motors for horizontal 
and vertical sun-tracking.

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

Contact: gottwald@htw-dresden.de
***********************************************************************/


/**** directions ****/
#define BACKWARD 			0x01
#define FORWARD				0x02
#define	HORIZONTAL 			0x11
#define	VERTICAL 			0x12

/**** motor modes ****/
#define	NORMAL_MODE 			0x20
#define SLOW_MODE			0x21
#define GOTO_REFERENCE			0x22

/**** step calculating factors ****/
#define VERTICAL_FACTOR			0x1E0
#define HORIZONTAL_FACTOR 		0x215

#define MAX_NUMBER_OF_STEPS		0xBD74

/**** pin definitions ****/
#define VERT_D1				PB0
#define HORZ_D1				PC1
#define VERT_D2				PB2
#define HORZ_D2				PC3
#define EN				PC0
#define VERT_HALL_1			PD2
#define HORZ_HALL_1			PD3
#define VERT_HALL_2			PD1
#define HORZ_HALL_2			PD4

#define VERT_SF				PB1
#define HORZ_SF				PC2

#define VERT_OUT			PD6
#define VERT_DIR			PD7
#define HORZ_DIR			PB7
#define HORZ_OUT			PD5
#define VERT_END			PD0
#define HORZ_END			PB6
#define VERT_SF				PB1
#define HORZ_SF				PC2

/**** port definitions ****/
#define VERT_OUT_PORT			PORTD
#define HORZ_OUT_PORT			PORTD
#define VERT_DIR_PORT			PORTD
#define HORZ_DIR_PORT			PORTB
#define VERT_D_PORT			PORTB
#define HORZ_D_PORT			PORTC

#define HORZ_DIR_DDR			DDRB
#define VERT_DIR_DDR			DDRD
#define HORZ_OUT_DDR			DDRD
#define VERT_OUT_DDR			DDRD
#define VERT_D_DDR			DDRB
#define HORZ_D_DDR			DDRC
#define EN_DDR				DDRC

#define HALL_1_PIN 			PIND
#define HALL_1_PORT 			PORTD
#define HALL_1_DDR 			DDRD

#define VERT_SF_DDR			DDRB
#define HORZ_SF_DDR			DDRC

#define VERT_SF_PORT			PORTB
#define HORZ_SF_PORT			PORTC

#define HORZ_END_DDR			DDRB
#define HORZ_END_PIN			PINB

#define VERT_END_DDR			DDRD
#define VERT_END_PIN			PIND

/**** I2C commands ****/
#define	CMD_SET_ANGLE	 		0x02
#define	CMD_GOTO_REFERENCE 		0x04
#define CMD_STOP			0x06
#define CMD_RESET			0x08
#define CMD_TERMINATED			0x33

#define ERR_STAUS_FLAG			0x50
#define ERR_ILLEGAL_END			0x51
#define ERR_STEPCOUNTER_OVERFLOW	0x52

/**** macros ****/
#define DISABLE_MOTOR_CONTROLLERS 	PORTC &= ~(1<<PC0);
#define ENABLE_MOTOR_CONTROLLERS 	PORTC |= (1<<PC0);

#define I2C_SLAVE_ADRESSE 		0x13

/**** function prototypes ****/
unsigned char motorControl(unsigned char motor, unsigned char mode);
void init_hall_cnt(void);
void init_timer(void);
void init_io_ports(void);

/**** global variables ****/
unsigned long int hall_cnt_vert_1, hall_cnt_vert_2, hall_cnt_horz_1, hall_cnt_horz_2;
unsigned int vertSteps, horzSteps;
unsigned char vertDirection, horzDirection;

/**** Set up function pointer to RESET vector ****/
void (*funcptr)( void ) = 0x0000; //

