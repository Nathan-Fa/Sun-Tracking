#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "main.h"
#include "twislave.h"


ISR (TIMER0_OVF_vect)
{
	switch(i2cdata[1]){	
		case CMD_SET_ANGLE:	
			vertSteps = i2cdata[3] * VERTICAL_FACTOR;
			motorControl(VERTICAL,NORMAL_MODE);
			break;
		case CMD_GOTO_REFERENCE:	
			motorControl(VERTICAL,GOTO_REFERENCE); 
			break;
		case CMD_RESET:
			funcptr(); // Jump to Reset vector 0x0000
			break;
		case CMD_STOP:
			VERT_OUT_PORT &= ~(1<<VERT_OUT);
			i2cdata[1] = CMD_TERMINATED;
			break;
		default: break;
	}

	switch(i2cdata[2]){
		case CMD_SET_ANGLE:	
			horzSteps = i2cdata[4] * HORIZONTAL_FACTOR;
			motorControl(HORIZONTAL,NORMAL_MODE); 
			break;	
		case CMD_GOTO_REFERENCE:	
			motorControl(HORIZONTAL,GOTO_REFERENCE); 
			break;
		case CMD_RESET:
			funcptr(); // Jump to Reset vector 0x0000
			break;
		case CMD_STOP:
			HORZ_OUT_PORT &= ~(1<<HORZ_OUT);
			i2cdata[2] = CMD_TERMINATED;
			break;
		default: break;
	}
}


ISR(INT0_vect)
{ 
	if(vertDirection == BACKWARD){
		hall_cnt_vert_1--; 
	}else{
		hall_cnt_vert_1++;
	}
}


ISR(INT1_vect)
{ 
	if(horzDirection == BACKWARD){
		hall_cnt_horz_1--; 
	}else{
		hall_cnt_horz_1++;
	}
}


/* just for safty reasons and never used in normal operation */
ISR(PCINT2_vect)
{
	if((!(VERT_END_PIN & (1<<VERT_END))) && (vertDirection == BACKWARD) && (i2cdata[1] != CMD_GOTO_REFERENCE)){
		VERT_D_PORT &= ~(1<<VERT_D2);
		i2cdata[1] = ERR_ILLEGAL_END;
	}
} 


ISR(PCINT0_vect)
{
	if((!(HORZ_END_PIN & (1<<HORZ_END))) && (horzDirection == BACKWARD) && (i2cdata[2] != CMD_GOTO_REFERENCE)){
		HORZ_D_PORT &= ~(1<<HORZ_D2);
		i2cdata[2] = ERR_ILLEGAL_END;
	}
}


int main( void )
{
	/* init functions */
	init_io_ports();
	init_hall_cnt();
	init_twi_slave(I2C_SLAVE_ADRESSE);
	init_timer();

	sei();	

	ENABLE_MOTOR_CONTROLLERS;

	/* goto reference position on reset */	
	i2cdata[1] = CMD_GOTO_REFERENCE;
	i2cdata[2] = CMD_GOTO_REFERENCE;

//	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	while(1)
	{
		i2cdata[5] = hall_cnt_horz_1;
		i2cdata[6] = (hall_cnt_horz_1 >> 7);
		i2cdata[7] = (hall_cnt_horz_1 >> 14);

		/* disable motors reaching upper step limit */
		if(hall_cnt_horz_1 > MAX_NUMBER_OF_STEPS){
			HORZ_D_PORT &= ~(1<<HORZ_D2);
			i2cdata[2] = ERR_STEPCOUNTER_OVERFLOW;
		}

		if(hall_cnt_vert_1 > MAX_NUMBER_OF_STEPS){
			VERT_D_PORT &= ~(1<<VERT_D2);
			i2cdata[1] = ERR_STEPCOUNTER_OVERFLOW;
		}

		/* processing staus flag */
		if(!(VERT_SF_PORT |= (1<<VERT_SF))){
			/* D2 needs to be toogled to clear status flag */	
			for (unsigned char i=0x00; i<0x05; i++){
				VERT_D_PORT ^= (1<<VERT_D2); 
				_delay_ms(1);
			}
			/* if status flag is stil set, entering error state */
			if(!(VERT_SF_PORT |= (1<<VERT_SF))){
				i2cdata[1] = ERR_STAUS_FLAG;
			}
		}

		if(!(HORZ_SF_PORT |= (1<<HORZ_SF))){
			/* D2 needs to be toogled to clear status flag */	
			for (unsigned char i=0x00; i<0x05; i++){
				HORZ_D_PORT ^= (1<<HORZ_D2); 
				_delay_ms(1);
			}
			/* if status flag is stil set, entering error state */
			if(!(HORZ_SF_PORT |= (1<<HORZ_SF))){
				i2cdata[2] = ERR_STAUS_FLAG;
			}
		}

		_delay_ms(2000);

		if(i2cdata[1] == CMD_TERMINATED && i2cdata[2] == CMD_TERMINATED){
//			FIXME
//			DISABLE_MOTOR_CONTROLLERS;
//			sleep_mode();                   // goto sleep mode
//			ENABLE_MOTOR_CONTROLLERS;
//			_delay_ms(1);
		}
	}
	
	return 0; 
}


unsigned char motorControl(unsigned char motor, unsigned char mode){
	if(mode == GOTO_REFERENCE){
		if(motor == VERTICAL){
			VERT_DIR_PORT &= ~(1<<VERT_DIR);
			VERT_OUT_PORT |= (1<<VERT_OUT);
			VERT_D_PORT |= (1<<VERT_D2);
			VERT_D_PORT &= ~(1<<VERT_D1);
			if(!(VERT_END_PIN & (1<<VERT_END))){  		// if end-switch is low
				VERT_OUT_PORT &= ~(1<<VERT_OUT);	// disable motor
				hall_cnt_vert_1 = 0x00;			// reset counter
				i2cdata[1] = CMD_TERMINATED;
			}
		}
		else if (motor == HORIZONTAL){
			HORZ_DIR_PORT &= ~(1<<HORZ_DIR);
			HORZ_OUT_PORT |= (1<<HORZ_OUT);
			HORZ_D_PORT |= (1<<HORZ_D2);
			HORZ_D_PORT &= ~(1<<HORZ_D1);
			if(!(HORZ_END_PIN & (1<<HORZ_END))){  		// if end-switch is low
				HORZ_OUT_PORT &= ~(1<<HORZ_OUT);	// disable motor
				hall_cnt_horz_1 = 0x00;			// reset counter
				i2cdata[2] = CMD_TERMINATED;
			}
		}
	}else if(mode == NORMAL_MODE){
		if(motor == VERTICAL){
			VERT_D_PORT |= (1<<VERT_D2);
			VERT_D_PORT &= ~(1<<VERT_D1);
			//steps = 48000;
			if(vertSteps > hall_cnt_vert_1){
				vertDirection = FORWARD;
				VERT_DIR_PORT |= (1<<VERT_DIR);
				VERT_OUT_PORT |= (1<<VERT_OUT);
			}else if(vertSteps < hall_cnt_vert_1){
				vertDirection = BACKWARD;
				VERT_OUT_PORT |= (1<<VERT_OUT);
				VERT_DIR_PORT &= ~(1<<VERT_DIR);
			}else{
				VERT_OUT_PORT &= ~(1<<VERT_OUT);
				i2cdata[1] = CMD_TERMINATED;
			}
		}else if(motor == HORIZONTAL){
			HORZ_D_PORT |= (1<<HORZ_D2);
			HORZ_D_PORT &= ~(1<<HORZ_D1);
			if(horzSteps > hall_cnt_horz_1){
				horzDirection = FORWARD;
				HORZ_DIR_PORT |= (1<<HORZ_DIR);
				HORZ_OUT_PORT |= (1<<HORZ_OUT);
			}else if(horzSteps < hall_cnt_horz_1){
				horzDirection = BACKWARD;
				HORZ_OUT_PORT |= (1<<HORZ_OUT);
				HORZ_DIR_PORT &= ~(1<<HORZ_DIR);
			}else{
				HORZ_OUT_PORT &= ~(1<<HORZ_OUT);
				i2cdata[2] = CMD_TERMINATED;
			}
		}
	}
	return 0;
}


void init_hall_cnt(void){
	/* Enable ext. interrupt on rising edges */
	EICRA |= (1<<ISC00) | (1<<ISC01) | (1<<ISC10) | (1<<ISC11);	
	EIMSK |= (1<<INT0) | (1<<INT1);	
}


void init_timer(void){
	/* timer overflow every 1ms */
	TCCR0B |= (1<<CS01) | (1<<CS00); 	// Prescaler 64
	TCNT0 = 131;				// Preload 131
	TIMSK0 |= (1<<TOIE0);			// Overflow Interrupt 
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
	VERT_SF_PORT |= (1<<VERT_SF);
	HORZ_SF_PORT |= (1<<HORZ_SF);

	/* M1D1 | M2D1 | M1nD2 | M2nD2 */
	VERT_D_DDR |= (1<<VERT_D1)|(1<<VERT_D2);
	HORZ_D_DDR |= (1<<HORZ_D1)|(1<<HORZ_D2);

	/* M1_End_Swich | M2_End_Swich */
	VERT_END_DDR &= ~(1<<VERT_END);
	HORZ_END_DDR &= ~(1<<HORZ_END); 

	/* M1_HALL1 | M2_HALL1 */
	HALL_1_DDR &= ~((1<<VERT_HALL_1)|(1<<HORZ_HALL_1));
	HALL_1_PORT |= (1<<VERT_HALL_1)|(1<<HORZ_HALL_1);

	/* Pin change interrupt on M1_End_Swich and M2_End_Swich*/
	PCMSK0 |= (1 << PCINT6);
	PCMSK2 |= (1 << PCINT16);
	PCICR |= (1 << PCIE0)|(1 << PCIE2);
}

