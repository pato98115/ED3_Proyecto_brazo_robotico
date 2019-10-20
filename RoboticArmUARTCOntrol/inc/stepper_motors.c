/*
===============================================================================
 Name        : stepper_motors.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "stepper_motors.h"

void delay(uint32_t tics);

/*
 * Configura los pines a partir del motor
 */
void motor_config(Motor *motor){
	if(((motor->dir_pinnum == motor->step_pinnum) && (motor->dir_portnum == motor->step_portnum))
	||((motor->enable_pinnum == motor->step_pinnum) && (motor->enable_portnum == motor->step_portnum))
	||((motor->enable_pinnum == motor->dir_pinnum) && (motor->enable_portnum == motor->dir_portnum)))
	{
		while(1){} //ERROR coincidencia de pines
	}
	PINSEL_CFG_Type pin_cfg;
	//config dir pin
	pin_cfg.Funcnum = PINSEL_FUNC_0;
	pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;
	pin_cfg.Portnum = motor->dir_portnum;
	pin_cfg.Pinnum = motor->dir_pinnum;
	PINSEL_ConfigPin(&pin_cfg);
	//config step pin
	pin_cfg.Portnum = motor->step_portnum;
	pin_cfg.Pinnum = motor->step_pinnum;
	PINSEL_ConfigPin(&pin_cfg);
	//config enable pin
	pin_cfg.Portnum = motor->enable_portnum;
	pin_cfg.Pinnum = motor->enable_pinnum;
	PINSEL_ConfigPin(&pin_cfg);
	//config microstepping
	pin_cfg.Portnum = motor->m_us_portnum;
	pin_cfg.Pinnum = motor->m1_us_pinnum;
	PINSEL_ConfigPin(&pin_cfg);
	pin_cfg.Pinnum = motor->m2_us_pinnum;
	PINSEL_ConfigPin(&pin_cfg);
	pin_cfg.Pinnum = motor->m3_us_pinnum;
	PINSEL_ConfigPin(&pin_cfg);
	//configura gpio outputs
	GPIO_SetDir(motor->dir_portnum, (1 << motor->dir_pinnum), 1);
	GPIO_SetDir(motor->step_portnum, (1 << motor->step_pinnum), 1);
	GPIO_SetDir(motor->enable_portnum, (1 << motor->enable_pinnum), 1);
	GPIO_SetDir(motor->m_us_portnum, (1 << motor->m1_us_pinnum), 1);
	GPIO_SetDir(motor->m_us_portnum, (1 << motor->m2_us_pinnum), 1);
	GPIO_SetDir(motor->m_us_portnum, (1 << motor->m3_us_pinnum), 1);
	GPIO_SetValue(motor->enable_portnum,(1<<motor->enable_pinnum));

	put_motor(motor);
	return;
}
/*
 * Inicializa el timer que corresponde al motor y setea el match
 * para que se corresponda con la velocidad
 * velocity = periodo de un paso en micro segundos
 */
void motor_timer_init(Motor *motor, uint32_t period){
	config_timer_useg(1, motor->timer);
	config_match(period, motor->timer);
	NVIC_EnableIRQ(get_IRQn(motor->timer));
	return;
}
/*
 * Activa el timer asociado al motor
 */
void start_motor_timer(Motor *motor){
	TIM_Cmd(motor->timer, ENABLE);
	return;
}
/*
 * Desactiva el timer asociado al motor
 */
void stop_motor_timer(Motor *motor){
	TIM_Cmd(motor->timer, DISABLE);
	return;
}

void micro_stepping_cfg(Motor *motor, uint8_t m1, uint8_t m2, uint8_t m3){//configura el microstepping
	if(m1==1) GPIO_SetValue(motor->m_us_portnum, (1 << motor->m1_us_pinnum));
	else GPIO_ClearValue(motor->m_us_portnum, (1 << motor->m1_us_pinnum));
	if(m2==1) GPIO_SetValue(motor->m_us_portnum, (1 << motor->m2_us_pinnum));
	else GPIO_ClearValue(motor->m_us_portnum, (1 << motor->m2_us_pinnum));
	if(m3==1) GPIO_SetValue(motor->m_us_portnum, (1 << motor->m3_us_pinnum));
	else GPIO_ClearValue(motor->m_us_portnum, (1 << motor->m3_us_pinnum));
}

void stop_steps(Motor *motor){
	clear_step_flag(motor->number);
	GPIO_SetValue(motor->enable_portnum, (1<< motor->enable_pinnum));//levanta pin enable, activa el driver
	motor->dir_value = DETENIDO;
}

/*
 * setea la direccion y levanta la bandera de pasos
 * para las interrupciones del timer
 */
void start_steps(Motor *motor, DIR_Value direction){
	if(direction == ANTI_HORARIA){
		GPIO_SetValue(motor->dir_portnum, (1<< motor->dir_pinnum));
		motor->dir_value = direction;
	}
	else{
		if(direction == HORARIA){
			GPIO_ClearValue(motor->dir_portnum, (1<< motor->dir_pinnum));
			motor->dir_value = direction;
		}
	}
	set_step_flag(motor->number); //abria que hacer parecido a la funcion motors
	GPIO_ClearValue(motor->enable_portnum, (1<< motor->enable_pinnum)); //baja pin enable, activa el driver
	return;
}
/*
 * cambia el periodo del paso
 */
void ch_velocity(LPC_TIM_TypeDef *TIMx, uint32_t new_period){
	TIM_UpdateMatchValue(TIMx, 0, new_period);
	return;
}
/*
 * Devuelve el Motor de la lista que se corresponde
 * su numero con el parametro motor_number
 */
Motor get_motor(uint8_t motor_number){
	Motor m_aux;
	m_aux.number = motor_number;
	return motors(&m_aux, GET);
}
/*
 * Agrega un motor a la lista de motores guardados
 */
void put_motor(Motor *motor){
	motors(motor, PUT);
	return;
}
/*
 * Gestiona la lista de los motores
 * Esta funcion se comporta distinto segun el valor del
 * parametro put_get
 * si put_get == GET (0) devuelve el motor de la lista en la
 * 						 posicion [number] del parametro motor
 * si put_get == PUT (1) agrega el parametro motor a la lista
 * 						 en la posicion [number] y lo devuelve
 */
Motor motors(Motor *motor, uint8_t put_get){
	static Motor motors_list[4];
	if(put_get == PUT){ //agregar un motor
		motors_list[motor->number] = *motor;
		return *motor;
	}
	else if(put_get == GET){//devolver un motor
		return motors_list[motor->number];
	}
	else{
		while(1){}
		return motors_list[0];
	}
}
/*
 * Levanta la bandera corresponidente al motor_number
 */
void set_step_flag(uint8_t motor_number){
	motor_flags(1, motor_number, PUT);
	return;
}
/*
 * Baja la bandera corresponidente al motor_number
 */
void clear_step_flag(uint8_t motor_number){
	motor_flags(0, motor_number, PUT);
	return;
}
/*
 * Devuelve el valor de la bandera corresponidente al motor_number
 */
uint8_t get_motor_flag(uint8_t motor_number){
	return motor_flags(0, motor_number, GET);
}

/*
 * Gestiona las banderas de los motores
 * Esta funcion se comporta distinto segun el valor del
 * parametro put_get
 * si put_get == GET (0) devuelve el valor de una bandera
 * si put_get == PUT (1) modifica una bandera y devuelve el nuevo valor
 */
uint8_t motor_flags(uint8_t value, uint8_t pos, uint8_t put_get){
	static uint8_t motor_flag[] = {0,0,0,0}; //PROBE CREAR VARIBLE GLOBAL
	if(put_get == PUT){ //change flag value
		motor_flag[pos] = value;
		return value;
	}
	else if(put_get == GET){//return flag value
		return motor_flag[pos];
	}
	else{
		while(1){}
		return motor_flag[0];
	}
}
/*
 * segun los parametros motor_a y motor b devuelve
 * 1 si hay una coincidencia critica
 * 0 si no hay una coincidencia critica
 */
uint8_t coincidence(Motor motor_a, Motor motor_b){ //devuelve 1 si coinciden en algo critico o 0 si no
	if(motor_a.number == motor_b.number) return 1; //mismo nro
	else if((motor_a.dir_portnum == motor_b.dir_portnum)
		&& (motor_a.dir_pinnum == motor_b.dir_pinnum)) return 1; //dir_pinA == dir_pinB
	else if((motor_a.dir_portnum == motor_b.step_portnum)
			&& (motor_a.dir_pinnum == motor_b.step_pinnum)) return 1; //dir_pinA == step_pinB
	else if((motor_a.step_portnum == motor_b.dir_portnum)
			&& (motor_a.step_pinnum == motor_b.dir_pinnum)) return 1; //step_pinA == dir_pinB
	else if((motor_a.step_portnum == motor_b.step_portnum)
			&& (motor_a.step_pinnum == motor_b.step_pinnum)) return 1; //step_pinA == step_pinB
	else return 0; //no hay coincidencia critica, todo OK
}
/*
 * Realiza un step
 */
void do_step(Motor* motor){
	GPIO_SetValue(motor->step_portnum, (1<< motor->step_pinnum));
	delay(20);//no se si hace falta el delay
	GPIO_ClearValue(motor->step_portnum, (1<< motor->step_pinnum));
}

void config_timer_useg(uint32_t time, LPC_TIM_TypeDef *TIMx){

	if(time>1000){
		while(1);
	}

	TIM_TIMERCFG_Type	timer_struct_config;

	timer_struct_config.PrescaleOption	    =	TIM_PRESCALE_USVAL;
	timer_struct_config.PrescaleValue		=	time;

	TIM_Init(TIMx, TIM_TIMER_MODE, &timer_struct_config);

	return;
}


void config_match(uint32_t matcher, LPC_TIM_TypeDef *TIMx){
	if(matcher>0xFFFFFFFF){
		while(1);
	}

	TIM_MATCHCFG_Type	match_ch_struct;

	match_ch_struct.MatchChannel		=	0;
	match_ch_struct.IntOnMatch			=	ENABLE;
	match_ch_struct.ResetOnMatch		=	ENABLE;
	match_ch_struct.StopOnMatch			=	DISABLE;
	match_ch_struct.ExtMatchOutputType	=	TIM_EXTMATCH_NOTHING;
	match_ch_struct.MatchValue			=	matcher;

	TIM_ConfigMatch(TIMx, &match_ch_struct);

	return;
}

IRQn_Type get_IRQn(LPC_TIM_TypeDef *TIMx){
	if(TIMx == LPC_TIM0) return TIMER0_IRQn;
	else if(TIMx == LPC_TIM1) return TIMER1_IRQn;
	else if(TIMx == LPC_TIM2) return TIMER2_IRQn;
	else if(TIMx == LPC_TIM3) return TIMER3_IRQn;
	else{
		while(1){}
		return -1;
	}
}

void delay(uint32_t tics){
	for(uint32_t i = 0; i < tics; i++){}
	return;
}

