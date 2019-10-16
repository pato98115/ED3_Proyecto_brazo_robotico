/*
===============================================================================
 Name        : RoboticArmUARTControl.c
 Author      : Patricio Viccini,Santana Ignacio
 Version     :1.0
===============================================================================
*/

#include "LPC17xx.h"

#include "lpc17xx_uart.h"

int main(void) {


	Motor motor_0;
	motor_0.number = 0;
	motor_0.dir_portnum = 2;
	motor_0.step_portnum = 2;
	motor_0.dir_pinnum = 0;
	motor_0.step_pinnum = 1;
	motor_0.m_us_portnum = 2;
	motor_0.m1_us_pinnum = 2;
	motor_0.m2_us_pinnum = 3;
	motor_0.m3_us_pinnum = 4;
	motor_0.timer = LPC_TIM0;

	Motor motor_1;
	motor_1.number = 1;
	motor_1.dir_portnum = 2;
	motor_1.step_portnum = 2;
	motor_1.dir_pinnum = 5;
	motor_1.step_pinnum = 6;
	motor_1.m_us_portnum = 2;
	motor_1.m1_us_pinnum = 7;
	motor_1.m2_us_pinnum = 8;
	motor_1.m3_us_pinnum = 10;
	motor_1.timer = LPC_TIM1;

	motor_config(&motor_0);
	motor_config(&motor_1);
	motor_init(&motor_0, STD_PERIOD);
	motor_init(&motor_1, STD_PERIOD);
	micro_stepping_cfg(&motor_0, 1, 1, 1);
	micro_stepping_cfg(&motor_1, 1, 1, 1);
	start_motor(&motor_0);
	start_motor(&motor_1);
	while(1){
    }
    return 0 ;
}

void confUart(void){
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	//configuración por defecto:
	UARTConfigStruct.Baud_rate = 19200;
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

void UART0_IRQnHandler(void){
	uint32_t intscr,tmp;
	static uint8_t info[3];
	intscr = UART_GetIntId(LPC_UART0);
	tmp = intscr & UART_IIR_INTID_MASK;
	if(tmp == UART_IIR_INTID_RDA|| tmp == UART_IIR_INTID_CTI){
		UART_Recieve(LPC_UART0,info,sizeof(info),NONE_BLOCKING);
		*(info+1) = "\n\r";
		UART_Send(LPC_UART0,info,sizeof(info),NONE_BLOCKING);
		switch(info[0]){
		case 0 :
			start_steps(&motor0,HORARIA);
		case 1:
			start_steps(&motor0,ANTI_HORARIA);
		case 2 :
			stop_steps(&motor0);
		case 3 :
			start_steps(&motor1,HORARIA);
		case 4:
			start_steps(&motor1,ANTI_HORARIA);
		case 2 :
			stop_steps(&motor1);
		}
	}
	return;
}