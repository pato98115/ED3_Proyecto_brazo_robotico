/*
 * stepper_motors.h
 *
 *  Created on: 16 de oct. de 2019
 *      Author: Pato & Nacho
 */

#define ANTI_HORARIA		0 //direccion antihoraria
#define HORARIA				1 //direccion antihoraria
#define GET					0 //
#define PUT					1 //

typedef struct{
	uint8_t number;
	uint8_t dir_pinnum;
	uint8_t dir_portnum;
	uint8_t step_pinnum;
	uint8_t step_portnum;
	uint8_t m_us_portnum;
	uint8_t m1_us_pinnum;
	uint8_t m2_us_pinnum;
	uint8_t m3_us_pinnum;
	LPC_TIM_TypeDef *timer;
}Motor;


void motor_config(Motor *motor);//configura pines y  agrega motor
void motor_timer_init(Motor *motor, uint32_t period);//configura timer y configura match
void start_motor_timer(Motor *motor);//activa el timer asociado al motor de ese numero
void stop_motor_timer(Motor *motor);//desactiva timer asociado al motor de ese numero
void stop_steps(Motor *motor);//desactiva los steps del motor
void micro_stepping_cfg(Motor *motor, uint8_t m1, uint8_t m2, uint8_t m3);//configura el microstepping
void start_steps(Motor *motor, uint8_t direction);//activa interrupciones y levanta bandera
void do_step(Motor* motor);//realiza un step
void ch_velocity(LPC_TIM_TypeDef *TIMx, uint32_t new_period);//modifica match
Motor get_motor(uint8_t motor_number); //devuelve el motor de la lista con dicho nro
void put_motor(Motor *motor); //agrega un motor a la lista
Motor motors(Motor *motor, uint8_t put_get);// agrega un montor a la lista o devuelve un motor
void set_step_flag(uint8_t motor_number); //levanta bandera de step
void clear_step_flag(uint8_t motor_number); //baja bandera de step
uint8_t get_motor_flag(uint8_t motor_number); //devuelve valor de bandera de step
uint8_t motor_flags(uint8_t value, uint8_t pos, uint8_t put_get); // gestiona banderas de step
uint8_t coincidence(Motor motor_a, Motor motor_b); //devuelve 1 si coinciden en algo o 0 si no coinciden

void time_to_param(uint32_t param[], uint32_t time);
void config_timer_useg(uint32_t time, LPC_TIM_TypeDef *TIMx);
void config_match(uint32_t matcher, LPC_TIM_TypeDef *TIMx);
IRQn_Type get_IRQn(LPC_TIM_TypeDef *TIMx);

