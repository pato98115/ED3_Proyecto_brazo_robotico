/*
===============================================================================
 Name        : RoboticArmUARTControl.c
 Author      : Patricio Viccini,Santana Ignacio
 Version     :1.0
===============================================================================
*/

#include "LPC17xx.h"

#include "lpc17xx_uart.h"
#include "stepper_motors.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_pwm.h"
#include "servo_motors.h"

void confUart(void);
void confPin(void);
void confPWM(void);

#define STD_PERIOD 300

uint8_t inte = 0;

int main(void) {
	Servo_Motor servo_motor;
	servo_motor.match_ch = 1;
	servo_motor.tic_period = 100;
	servo_motor.duty_cycle = 13;
	servo_motor.cycle = 200;
	servo_motor.inf_limit = 13;
	servo_motor.sup_limit = 35;
	servo_init(&servo_motor);
	servo_pin_start(&servo_motor);

	confPin();
	confUart();
	Motor motor_0;
	motor_0.number = 0;
	motor_0.dir_portnum = 2;
	motor_0.step_portnum = 2;
	motor_0.dir_pinnum = 1;
	motor_0.step_pinnum = 2;
	motor_0.m_us_portnum = 2;
	motor_0.m1_us_pinnum = 3;
	motor_0.m2_us_pinnum = 4;
	motor_0.m3_us_pinnum = 5;
	motor_0.timer = LPC_TIM0;
	motor_0.enable_portnum = 0;
	motor_0.enable_pinnum = 11;


	Motor motor_1;
	motor_1.number = 1;
	motor_1.dir_portnum = 2;
	motor_1.step_portnum = 2;
	motor_1.dir_pinnum = 6;
	motor_1.step_pinnum = 7;
	motor_1.m_us_portnum = 2;
	motor_1.m1_us_pinnum = 8;
	motor_1.m2_us_pinnum = 10;
	motor_1.m3_us_pinnum = 11;
	motor_1.enable_portnum = 2;
	motor_1.enable_pinnum = 12;
	motor_1.timer = LPC_TIM0;

	motor_config(&motor_0);
	motor_config(&motor_1);
	motor_timer_init(&motor_0, STD_PERIOD);
	micro_stepping_cfg(&motor_0, 1, 1, 1);
	micro_stepping_cfg(&motor_1, 1, 1, 1);
	start_motor_timer(&motor_0);

	confPWM();

	while(1){
		uint32_t aux = servo_motor.duty_cycle;
		if(inte)
			servo_motor.duty_cycle = 35;
		else
			servo_motor.duty_cycle =13;
		if(servo_motor.duty_cycle !=aux)
			servo_update_duty_cycle(&servo_motor);
	}

	return 0 ;
}

void TIMER0_IRQHandler(void){
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
	for(uint8_t i = 0; i < 4; i++){
		if(get_motor_flag(i) == 1){
			Motor motor = get_motor(i);
			do_step(&motor);
		}
	}
	return;
}

void TIMER1_IRQHandler(void){
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
	for(uint8_t i = 0; i < 4; i++){
		if(get_motor_flag(i) == 1){
			Motor motor = get_motor(i);
			do_step(&motor);
		}
	}
	return;
}

void confPWM(void){

}

void confUart(void){
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	//configuración por defecto:
	UARTConfigStruct.Baud_rate = 9600;
	UARTConfigStruct.Databits = UART_DATABIT_8;
	UARTConfigStruct.Parity = UART_PARITY_NONE;
	UARTConfigStruct.Stopbits = UART_STOPBIT_1;
	UART_Init(LPC_UART0, &UARTConfigStruct);

	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);//Inicializa FIFO
	UART_FIFOConfig(LPC_UART0, &UARTFIFOConfigStruct);

	// Habilita transmisión
	UART_TxCmd(LPC_UART0, ENABLE);
	UART_IntConfig(LPC_UART0,UART_INTCFG_THRE,ENABLE);

	UART_IntConfig(LPC_UART0,UART_INTCFG_RBR,ENABLE);
	UART_IntConfig(LPC_UART0,UART_INTCFG_RLS,ENABLE);
	NVIC_EnableIRQ(UART0_IRQn);
	return;
}

void confPin(void){
	PINSEL_CFG_Type PinCfg;
	//configuración pin de Tx y Rx
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Portnum = 2;
	PinCfg.Pinnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	return;
}

void UART0_IRQHandler(void){
	uint32_t intscr,tmp;
	static uint8_t info[3];
	intscr = UART_GetIntId(LPC_UART0);
	tmp = intscr & UART_IIR_INTID_MASK;
	if(tmp == UART_IIR_INTID_RDA|| tmp == UART_IIR_INTID_CTI){
		UART_Receive(LPC_UART0,info,sizeof(info),NONE_BLOCKING);
		*(info+1) = (uint8_t)'\r';
		*(info+2) = (uint8_t)'\n';
		UART_Send(LPC_UART0,info,sizeof(info),NONE_BLOCKING);
		Motor motor0 = get_motor(0);
		Motor motor1 = get_motor(1);
		switch(info[0]){
		case '0':
			inte = (inte)? 0:1;
			break;
		case '1' :
			start_steps(&(motor0),HORARIA);
			break;
		case '2':
			start_steps(&motor0,ANTI_HORARIA);
			break;
		case '3' :
			stop_steps(&motor0);
			break;
		case '4' :
			start_steps(&motor1,HORARIA);
			break;
		case '5':
			start_steps(&motor1,ANTI_HORARIA);
			break;
		case '6' :
			stop_steps(&motor1);
			break;
		}
	}
	return;
}
