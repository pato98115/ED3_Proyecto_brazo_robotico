/*
===============================================================================
 Name        : servo_motors.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_pwm.h"
#include "servo_motors.h"

void PWM_pin_config(uint8_t match_ch);
void servo_PWM_config(uint32_t tick_time_us, uint32_t cycle, uint32_t duty_cycle, uint8_t match_ch);
void PWM_timer_config(uint32_t tick_time_us);
void PWM_config_M0(uint32_t cycle);
void PWM_config_Mx(uint32_t duty_cycle, uint8_t match_ch, uint8_t mode_option);
void PWM_start();
void PWM_stop();


/*
 * configura el pin del puerto 2 correspondiente al canal del PWM
 */
void PWM_pin_config(uint8_t match_ch){
	if((match_ch < 1)||(match_ch > 6)) while(1){} //error
	//utiliza las salidas pwm del puerto 2
	PINSEL_CFG_Type pinsel_cfg;
	pinsel_cfg.Funcnum = PINSEL_FUNC_1;
	pinsel_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	pinsel_cfg.Pinmode = PINSEL_PINMODE_PULLUP;
	pinsel_cfg.Portnum = PINSEL_PORT_2;
	pinsel_cfg.Pinnum = (match_ch - 1);
	PINSEL_ConfigPin(&pinsel_cfg);
	return;
}

/*
void servo_PWM_config(uint32_t tick_time_us, uint32_t cycle, uint32_t duty_cycle, uint8_t match_ch){

	PWM_TIMERCFG_Type timer_config;
	timer_config.PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
	timer_config.PrescaleValue = tick_time_us;

	PWM_Init(LPC_PWM1, PWM_MODE_TIMER, &timer_config);

	//config match 0
	PWM_MATCHCFG_Type match_config;
	match_config.IntOnMatch = DISABLE;
	match_config.StopOnMatch = DISABLE;
	match_config.MatchChannel = 0;
	match_config.ResetOnMatch = ENABLE;
	PWM_ConfigMatch(LPC_PWM1, &match_config);

	PWM_MatchUpdate(LPC_PWM1, 0, cycle, PWM_MATCH_UPDATE_NOW);

	//config match chanel
	match_config.MatchChannel = match_ch;
	match_config.ResetOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &match_config);

	PWM_MatchUpdate(LPC_PWM1, match_ch, duty_cycle, PWM_MATCH_UPDATE_NOW);

	// Reset and Start counter
	PWM_ResetCounter(LPC_PWM1);
	PWM_CounterCmd(LPC_PWM1, ENABLE);

    // Start PWM now
    PWM_Cmd(LPC_PWM1, ENABLE);
	//arranca pwm chanel
	PWM_ChannelCmd(LPC_PWM1, match_ch, ENABLE);

	return;
}
*/


/*
 * configura el timer del modulo PWM
 * el valor del prescaler es en microsegundos
 */
void PWM_timer_config(uint32_t tick_time_us){
	PWM_TIMERCFG_Type timer_config;
	timer_config.PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
	timer_config.PrescaleValue = tick_time_us;

	PWM_Init(LPC_PWM1, PWM_MODE_TIMER, &timer_config);
	return;
}
/*
 * configura el match 0, es el que corresponde al valor de todo el ciclo
 * deberia ser el unico match que haga reset al timer
 */
void PWM_config_M0(uint32_t cycle){
	//config match 0
	PWM_MATCHCFG_Type match_config;
	match_config.IntOnMatch = DISABLE;
	match_config.StopOnMatch = DISABLE;
	match_config.MatchChannel = 0;
	match_config.ResetOnMatch = ENABLE;
	PWM_ConfigMatch(LPC_PWM1, &match_config);
	PWM_MatchUpdate(LPC_PWM1, 0, cycle, PWM_MATCH_UPDATE_NOW);
	return;
}
/*
 * configura el match x pasado por parametro, es el que corresponde al valor del duty cycle
 *
 */
void PWM_config_Mx(uint32_t duty_cycle, uint8_t match_ch, uint8_t mode_option){
	if((match_ch < 1) || (match_ch > 6)) while(1){}
	//configura channel
	if(match_ch > 1) PWM_ChannelConfig(LPC_PWM1, match_ch, mode_option);
	//configura match
	PWM_MATCHCFG_Type match_config;
	match_config.IntOnMatch = DISABLE;
	match_config.StopOnMatch = DISABLE;
	match_config.MatchChannel = match_ch;
	match_config.ResetOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &match_config);
	PWM_MatchUpdate(LPC_PWM1, match_ch, duty_cycle, PWM_MATCH_UPDATE_NOW);

	return;
}
/*
 * arranca el PWM
 */
void PWM_start(){
	PWM_ResetCounter(LPC_PWM1);
	PWM_CounterCmd(LPC_PWM1, ENABLE);
    PWM_Cmd(LPC_PWM1, ENABLE);
	return;
}
/*
 * detiene el PWM
 */
void PWM_stop(){
	PWM_CounterCmd(LPC_PWM1, DISABLE);
    PWM_Cmd(LPC_PWM1, DISABLE);
	return;
}
/*
 * configura todo el modulo PWM para los valores del servo
 * y arranca dicho modulo
 */
void servo_init(Servo_Motor *servo){
	PWM_pin_config(servo->match_ch);
	PWM_timer_config(servo->tic_period);
	PWM_config_M0(servo->cycle);
	PWM_config_Mx(servo->duty_cycle, servo->match_ch, PWM_CHANNEL_SINGLE_EDGE);
	PWM_start();
	return;
}
/*
 * saca la senial de PWM por el canal del servo
 */
void servo_pin_start(Servo_Motor *servo){
	PWM_ChannelCmd(LPC_PWM1, servo->match_ch, ENABLE);
	return;
}
/*
 * deja de sacar la senial de PWM por el canal del servo
 */
void servo_pin_stop(Servo_Motor *servo){
	PWM_ChannelCmd(LPC_PWM1, servo->match_ch, DISABLE);
	return;
}
/*
 * modifica el duty cycle del canal del servo
 */
void servo_update_duty_cycle(Servo_Motor *servo){
	PWM_MatchUpdate(LPC_PWM1, servo->match_ch, servo->duty_cycle, PWM_MATCH_UPDATE_NOW);
	return;
}

Servo_Motor *get_servo_motor(uint8_t match_ch){
	Servo_Motor aux;
	aux.match_ch = match_ch;
	return servo_motors(&aux, GET);
}
void put_servo_motor(Servo_Motor *servo){
	servo_motors(servo, PUT);
	return;
}
Servo_Motor *servo_motors(Servo_Motor *servo, uint8_t put_get){
	static Servo_Motor *motors_list[7];
	if(put_get == PUT){ //agregar un motor
		motors_list[servo->match_ch] = servo;
		return servo;
	}
	else if(put_get == GET){//devolver un motor
		return motors_list[servo->match_ch];
	}
	else{
		while(1){}
		return motors_list[0];
	}
}



