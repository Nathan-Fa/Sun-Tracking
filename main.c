#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
#include "main.h"
#include "twislave.h"   


ISR(INT0_vect)
{ 
	if(vertDirection == FORWARD){
		hall_cnt_vert_1++; 
	}else if(vertDirection == BACKWARD){
		hall_cnt_vert_1--;
	}
}


ISR(INT1_vect)
{ 
	if(horzDirection == FORWARD){
		hall_cnt_horz_1++; 
	}else if(horzDirection == BACKWARD){
		hall_cnt_horz_1--;
	}
}


void init_hall_cnt(void)
{
	/* reset hall counters */
	hall_cnt_vert_1 = 0x00;
	hall_cnt_horz_1 = 0x00;

	/* Enable ext. interrupt on rising edges */
	EICRA |= (1<<ISC00) | (0<<ISC01) | (0<<ISC10) | (0<<ISC11);	
	EIMSK |= (1<<INT0) | (1<<INT1);	
	sei();	
}


void init_io_ports(void){
	/* EN */
	EN_DDR |= (1<<EN);

	/* M1_DIR | M2_DIR */
	VERT_DIR_DDR |= (1<<VERT_DIR);
	HORZ_DIR_DDR |= (1<<HORZ_DIR);

	/* M1_PWM | M2_PWM */
	VERT_OUT_DDR |= (1<<VERT_OUT);
	HORZ_OUT_DDR |= (1<<HORZ_OUT);

	/* M1nSF | M2nSF */
	VERT_SF_DDR &= ~(1<<VERT_SF);
	HORZ_SF_DDR &= ~(1<<HORZ_SF);

	/* M1D1 | M2D1 | M1nD2 | M2nD2 */
	VERT_D_DDR |= (1<<VERT_D1)|(1<<VERT_D2);
	HORZ_D_DDR |= (1<<HORZ_D1)|(1<<HORZ_D2);

	/* M1_End_Swich | M2_End_Swich */
	VERT_END_DDR &= ~(1<<VERT_END);
	HORZ_END_DDR &= ~(1<<HORZ_END); 

	/* M1_HALL1 | M2_HALL1 */
	HALL_1_DDR &= ~((1<<VERT_HALL_1)|(1<<HORZ_HALL_1));
	HALL_1_PORT |= (1<<VERT_HALL_1)|(1<<HORZ_HALL_1);
}


int main( void )
{
	/* init functions */
	init_io_ports();
	init_hall_cnt();
	init_twi_slave(I2C_SLAVE_ADRESSE);

/* i2c DEBUG messages */
	i2cdata[3] = 0xDE;
	i2cdata[4] = 0xAD;
	i2cdata[5] = 0xBE;
	i2cdata[6] = 0xEF;
/* i2c DEBUG  messages */

	/* goto reference position on reset */
	motorControl(0x00,VERTICAL,GOTO_REFERENCE);

	while(1)
	{
		switch(i2cdata[1]){
		case SET_AZIMUTH:	
			azimuth = i2cdata[2]; motorControl(azimuth,HORIZONTAL,NORMAL_MODE); break;
		case SET_ELEVATION:	
			elevation = i2cdata[2]; motorControl(elevation,VERTICAL,NORMAL_MODE);break;
		case GOTO_VERTICAL_REFERENCE:	
			motorControl(0x00,VERTICAL,GOTO_REFERENCE); break;
		case GOTO_HORIZONTAL_REFERENCE:	
			motorControl(0x00,HORIZONTAL,GOTO_REFERENCE); break;
		default: break;
		}
	}
	
	return 0; 
}


unsigned char motorControl(unsigned char angle, unsigned char motor, unsigned char mode){

	unsigned char dir, out, end, dirPort, outPort, dPort, endPin, d1, d2;

	ENABLE_MOTOR_CONTROLLERS;
	
	if(mode == GOTO_REFERENCE){
		if(motor == VERTICAL){
			end = VERT_END; 
			endPin = VERT_END_PIN;
			dir = VERT_DIR; 
			out = VERT_OUT; 
			dirPort = VERT_DIR_PORT; 
			outPort = VERT_OUT_PORT;
			dPort = VERT_D_PORT;
			d1 = VERT_D1; 
			d2 = VERT_D2;
		}
		else if (motor == HORIZONTAL){
			end = HORZ_END;
			endPin = HORZ_END_PIN;
			dir = HORZ_DIR; 
			out = HORZ_OUT; 
			dirPort = HORZ_DIR_PORT; 
			outPort = HORZ_OUT_PORT;
			dPort = HORZ_D_PORT;
			d1 = HORZ_D1; 
			d2 = HORZ_D2;
		}
		dPort |= (1<<d2); dPort &= ~(1<<d1);
		dirPort &= ~(1<<dir);
		outPort |= (1<<out);
		while(endPin & (1<<end)){ asm("nop"); }
	}else{
		if(motor == VERTICAL){
			unsigned int steps = angle * VERTICAL_FACTOR;
			if(steps > hall_cnt_vert_1){
				EICRA |= (1<<ISC00) | (1<<ISC01);	// cnt rising edges
				vertDirection = FORWARD;
				while(hall_cnt_vert_1 < steps)
				{		
					VERT_OUT_PORT |= (1<<VERT_OUT);
					VERT_DIR_PORT |= (1<<VERT_DIR);
				}
			}else{
				EICRA |= (0<<ISC00) | (1<<ISC01);  	// cnt falling edges
				vertDirection = BACKWARD;
				while(hall_cnt_vert_1 > steps)
				{		
					VERT_OUT_PORT |= (1<<VERT_OUT);
					VERT_DIR_PORT &= ~(1<<VERT_DIR);
				}
			}
		}else if(motor == HORIZONTAL){
			unsigned int steps = angle * HORIZONTAL_FACTOR;
			if(steps > hall_cnt_horz_1){
				EICRA |= (1<<ISC10) | (1<<ISC11);	// cnt rising edges
				horzDirection = FORWARD;
				while(hall_cnt_horz_1 > steps)
				{		
					HORZ_OUT_PORT |= (1<<HORZ_OUT);
					HORZ_DIR_PORT |= (1<<HORZ_DIR);
				}
			}else{
				EICRA |= (0<<ISC10) | (1<<ISC11);  	// cnt falling edges
				horzDirection = BACKWARD;
				while(hall_cnt_horz_1 < steps)
				{		
					HORZ_OUT_PORT |= (1<<HORZ_OUT);
					HORZ_DIR_PORT &= ~(1<<HORZ_DIR);
				}
			}
		}
	}
	outPort &= ~(1<<out);
	DISABLE_MOTOR_CONTROLLERS;

	i2cdata[1] = CMD_TERMINATED;
	return 0;
}
