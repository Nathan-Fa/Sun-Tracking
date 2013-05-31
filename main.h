/**** directions ****/
#define BACKWARD 			0x01
#define FORWARD				0x02
#define	HORIZONTAL 			0x11
#define	VERTICAL 			0x12

/**** motor modes ****/
#define	NORMAL_MODE 			0x20
#define FAST_MODE			0x21
#define GOTO_REFERENCE			0x22

/**** pin definitions ****/
#define VERTICAL_FACTOR			495
#define HORIZONTAL_FACTOR 		550

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

#define HORZ_END_DDR			DDRB
#define HORZ_END_PIN			PINB

#define VERT_END_DDR			DDRD
#define VERT_END_PIN			PIND

/**** macros ****/
#define DISABLE_MOTOR_CONTROLLERS 	PORTC &= ~(1<<PC0);
#define ENABLE_MOTOR_CONTROLLERS 	PORTC |= (1<<PC0);

/**** I2C commands ****/
#define	SET_ELEVATION 			0x02
#define	SET_AZIMUTH 			0x03
#define	GOTO_VERTICAL_REFERENCE 	0x04
#define	GOTO_HORIZONTAL_REFERENCE	0x05
#define CMD_TERMINATED			0x33

#define I2C_SLAVE_ADRESSE 		0x13

/**** function prototypes ****/
unsigned char motorControl(unsigned char angle, unsigned char motor, unsigned char mode);
void init_hall_cnt(void);
void init_io_ports(void);

/**** global variables ****/
uint32_t hall_cnt_vert_1, hall_cnt_vert_2, hall_cnt_horz_1, hall_cnt_horz_2;
unsigned char azimuth, elevation;
unsigned char vertDirection, horzDirection;

