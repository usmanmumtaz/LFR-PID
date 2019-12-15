

// Timer 1 base address
#define TM_BASE 							0x40031000

// Peripheral clock enabling for timer and GPIO
#define RCGC_TIMER_R 				  *( volatile unsigned long *) 0x400FE604
#define RCGC2_GPIO_R 				  *( volatile unsigned long *) 0x400FE108
#define CLOCK_GPIO_AF 				  0x00000021 // Port A,F clock control
#define SYS_CLOCK_FREQUENCY   16000000


// General purpose timer register definitions
#define GPTM_CONFIG_R 				*( volatile long *) ( TM_BASE + 0x000 )
#define GPTM_CONTROL_R 				*( volatile long *) ( TM_BASE + 0x00C )

#define GPTM_TA_MODE_R 				*( volatile long *) ( TM_BASE + 0x008 )
#define GPTM_TA_IL_R 					*( volatile long *) ( TM_BASE + 0x02C )
#define GPTM_TA_MATCH_R 			*( volatile long *) ( TM_BASE + 0x034 )

#define GPTM_TA_MODE_RB 			*( volatile long *) ( TM_BASE + 0x004 )
#define GPTM_TA_IL_RB 				*( volatile long *) ( TM_BASE + 0x028 )
#define GPTM_TA_MATCH_RB 			*( volatile long *) ( TM_BASE + 0x030 )


// GPIO PA01234 configuration
#define GPIO_PORTA_DATA_R 				*((volatile unsigned long*)0x4000407C)
#define GPIO_PORTA_DIR_R 					*((volatile unsigned long*)0x40004400)
#define GPIO_PORTA_DEN_R 					*((volatile unsigned long*)0x4000451C)
#define PORTA_PIN									0x1F


// GPIO PF2 alternate function configuration
#define GPIO_PORTF_AFSEL_R 		*(( volatile unsigned long *) 0x40025420 )
#define GPIO_PORTF_PCTL_R 		*(( volatile unsigned long *) 0x4002552C )
#define GPIO_PORTF_DEN_R 			*(( volatile unsigned long *) 0x4002551C )


// Timer config and mode bit field definitions
#define TIM_16_BIT_CONFIG 		0x00000004 // 16- bit timer
#define TIM_PERIODIC_MODE 		0x00000002 // Periodic timer mode
#define TIM_A_ENABLE 					0x00000101 // Timer A enable control
#define TIM_PWM_MODE 					0x0000000A // Timer in PWM mode
#define TIM_CAPTURE_MODE 			0x00000004 // Timer capture mode


// Timer1 A reload value for 1 kHz PWM frequency
#define TIM_A_INTERVAL 				16000 // Timer reload value for
// 1 kHz PWM frequency at
// clock frequency of 16 MHz
// 16 ,000 ,000/16000
// = 1 kHz
#define TIM_A_MATCH 					12000 // Timer match value for 75%
// duty cycle
// Timer and GPIO intialization and configuration


#define set 4
#define Kp  700
#define Kd  1000
#define Ki  500


//TIMER 1-A: GREEN
//TIMER 1-B: BLUE

void Timer1_Init ( void )
{
// Enable the clock for Timer1
RCGC_TIMER_R |= 0x02 ;
	
	
// Configure PortF pin 2,3 as Timer1 A,B output respectively
GPIO_PORTF_AFSEL_R |= 0x0000000C ;
GPIO_PORTF_PCTL_R |= 0x00007700 ; // Configure as timer CCP pin
GPIO_PORTF_DEN_R |= 0x0000000C ;
	
	
// Enable the clock for Timer 1
GPTM_CONTROL_R &= ~( TIM_A_ENABLE ); // disable timer 1 A
	
	
// Timer1 A configured as 16- bit timer
GPTM_CONFIG_R |= TIM_16_BIT_CONFIG ;
// Timer1 A in periodic timer , edge count and PWM mode
GPTM_TA_MODE_R |= TIM_PWM_MODE ;
GPTM_TA_MODE_R &= ~( TIM_CAPTURE_MODE );
// Make PWM frequency 1 kHz using reload value of 16000
GPTM_TA_IL_R = TIM_A_INTERVAL ;
//GPTM_TA_MATCH_R = TIM_A_MATCH;



// Timer1 B in periodic timer , edge count and PWM mode
GPTM_TA_MODE_RB |= TIM_PWM_MODE ;
GPTM_TA_MODE_RB &= ~( TIM_CAPTURE_MODE );
// Make PWM frequency 1 kHz using reload value of 16000
GPTM_TA_IL_RB = TIM_A_INTERVAL ;
//GPTM_TA_MATCH_RB = TIM_A_MATCH;
	
	
GPTM_CONTROL_R |= TIM_A_ENABLE ;
}



void Speed_Control (int motor_speed)
{
	
	
	
	
	// Configure PWM duty cycle value ( should be less than 16000)
  if (TIM_A_MATCH + motor_speed >= TIM_A_INTERVAL) {GPTM_TA_MATCH_R = TIM_A_INTERVAL-1;}
	else if (TIM_A_MATCH + motor_speed <= 0) {GPTM_TA_MATCH_R = 1;}
	else {GPTM_TA_MATCH_R = TIM_A_MATCH + motor_speed ;}

	// Configure PWM duty cycle value ( should be less than 16000)
  if (TIM_A_MATCH - motor_speed >= TIM_A_INTERVAL) {GPTM_TA_MATCH_RB = TIM_A_INTERVAL-1;}
	else if (TIM_A_MATCH - motor_speed <= 0) {GPTM_TA_MATCH_RB = 1;}
	else {GPTM_TA_MATCH_RB = TIM_A_MATCH - motor_speed ;}
	
  // Enable timer1 
  
}




// Application main function
int main ( void ){
	unsigned int LUT[9] = {0x1F, 0x0F, 0x7, 0x03, 0x11, 0x18, 0x1C, 0x1E, 0x1F};
	int actual, error=0, pre_error=0, diff_error=0, cum_error=0, motor_speed;
	
	
	// Enable the clock for port F
	RCGC2_GPIO_R |= CLOCK_GPIO_AF ;
	
	GPIO_PORTA_DEN_R |= PORTA_PIN;
	GPIO_PORTA_DIR_R &= ~(PORTA_PIN);

	Timer1_Init (); // Initialize the timer
	


	while (1)
{
if( GPIO_PORTA_DATA_R != 0x11)
{
	
	for (actual=0; actual<=8; actual++)
	{
		
		if (GPIO_PORTA_DATA_R == LUT[actual])
		{
			error = set - actual;
			break;
		}
		if (actual == 8) {actual = 0;}
	}
}
else {error = 0;}

if (error == 4)
{
	if (pre_error < 0) {error = -4;}
}

diff_error = error - pre_error;
cum_error = error + cum_error;
motor_speed = ((Kp*error) + (Kd*diff_error) + (Ki*cum_error));
Speed_Control (motor_speed);
pre_error = error;
}
}
//Connections:
//Green: Right Motor.
//Blue: Left Motor..

	

