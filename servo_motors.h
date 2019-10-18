/*
===============================================================================
 Name        : servo_motors.h
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_pwm.h"

#define GET					0 //
#define PUT					1 //

typedef struct {
	uint8_t match_ch;		// 1 a 6
	uint32_t tic_period;	//[us]
	uint32_t cycle;
	uint32_t duty_cycle;
	uint32_t inf_limit;		// < sup_limit
	uint32_t sup_limit;		// < cycle
}Servo_Motor;

void servo_init(Servo_Motor *servo);
void servo_pin_start(Servo_Motor *servo);
void servo_pin_stop(Servo_Motor *servo);
void servo_update_duty_cycle(Servo_Motor *servo);
Servo_Motor *get_servo_motor(uint8_t match_ch);
void put_servo_motor(Servo_Motor *servo);
Servo_Motor *servo_motors(Servo_Motor *servo, uint8_t put_get);


