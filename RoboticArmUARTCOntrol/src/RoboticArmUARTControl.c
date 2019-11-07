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
#include "lpc17xx_systick.h"

#define CANTIDAD_STEPPERS 4
#define TIMER_GRABACION LPC_TIM1
#define TIMER_GRABACION_HANDLER TIMER1_IRQHandler

typedef enum{
	AGARRAR =0,
	SOLTAR
} Garra_Cmd;

typedef enum{
	GRABACION =0,
	EJECUCION,
	MANUAL
}  Estado;

typedef enum{
	GUARDAR,
	INICIAR,
	TERMINAR,
	REPETIR
} RegStampCmd;

typedef enum{
	ACTIVADA,
	DESACTIVADA
} BanderaDeEjecucion;

typedef enum{
	cmd_null =96,
	M0DETENER,
	M0HORARIO,
	M0ANTIHORARIO,
	M1DETENER,
	M1HORARIO,
	M1ANTIHORARIO,
	M2DETENER,
	M2HORARIO,
	M2ANTIHORARIO,
	M3DETENER,
	M3HORARIO,
	M3ANTIHORARIO,
	AGARRAROSOLTAR,
	MODOMANUAL,
	GRABARMOVIMIENTO,
	DETENERYEJECUTAR
} UART_Cmd;

typedef struct{
	uint32_t tiempo;
	UART_Cmd comando;
} Reg_Stamp;


void limpiar_perifericos(void);
void confUart(void);
void confSteppers(void);
void confServo(Servo_Motor *);
Status inicializarTimerDeGrabacion(LPC_TIM_TypeDef *);
void cmdMatch0InterruptTimGrabacion(FunctionalState interupt);
uint32_t leerYResetearTimer(LPC_TIM_TypeDef *);
Estado states(Estado,uint8_t );
Estado getEstado(void);
void setEstado(Estado estado);
BanderaDeEjecucion banderaEnEstadoEjecucion(BanderaDeEjecucion,uint32_t put_get);
BanderaDeEjecucion getBanderaEnEstadoEjecucion(void);
void setBanderaEnEstadoEjecucion(BanderaDeEjecucion);
void garraCmd(Servo_Motor * , Garra_Cmd);
void ejecutarUARTCmd(UART_Cmd comando);
void detenerMovimientos(void);
int32_t posicion(uint8_t numeroDeMotor,uint8_t put_get,DIR_Value direccion);
void getPosSteppers(int32_t *);
Reg_Stamp regStampUpdate(UART_Cmd comandoUart,RegStampCmd comandoStamp,uint32_t cuentaTimer);



#define STD_PERIOD 100


int main(void) {
	confSteppers();
	Servo_Motor servo_motor;
	confServo(&servo_motor);
	SysTick_Config(SystemCoreClock/40);
	NVIC_SetPriority(SysTick_IRQn,10);
	SYSTICK_IntCmd(DISABLE);
	setEstado(MANUAL);
	confUart();
	limpiar_perifericos();
	if(inicializarTimerDeGrabacion(TIMER_GRABACION)==ERROR) // inicializamos el timer que lleva la cuenta cuando se esta en modo grabacion
			while(1){}                           //se produjo un error ya que el timer ya esta en uso( algun motor lo usa por ejemplo)
	Estado estadoActual =MANUAL;
	Estado estadoAnterior = MANUAL;
	static int32_t posicionInicial[4];
	static int32_t posicionFinal[4];
	Reg_Stamp registroTemporal;
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_SetPriority(TIMER1_IRQn,1);
	while(1)
	{
		while(estadoActual == estadoAnterior){  //Busca la transiciones de estados , mientras el estado actual sea igual al anterior se mantiene en este ciclo
			if(estadoActual == EJECUCION && getBanderaEnEstadoEjecucion()==ACTIVADA){ // Si estamos en el estado de ejecucion y se levanto la bandera,
				TIM_Cmd(TIMER_GRABACION,DISABLE);
				leerYResetearTimer(TIMER_GRABACION);// debemos ejecutar el comando determinado por el registro de comandos,pero primero
				ejecutarUARTCmd(registroTemporal.comando);				// apagamos el conteo del timer para que no haya una corrupcion cuando escribamos el match(puede que no sea necesario)
				registroTemporal = regStampUpdate(0,REPETIR,0); 		// una vez que ejecutamos el comando, extraemos el siguiente registro de comandos, y verificamos
				while(registroTemporal.tiempo == TIMER_GRABACION->MR0){	// que los eventos no hayan sido grabados al mismo tiempo (esto sucede cuando llega al final de la grabacion)
					ejecutarUARTCmd(registroTemporal.comando);			// si sucedieron al mismo tiempo, se ejecutan al mismo tiempo.
					registroTemporal = regStampUpdate(0,REPETIR,0);		//
				}														//
				TIM_UpdateMatchValue(TIMER_GRABACION,0,registroTemporal.tiempo);//Luego se actualiza el valor del match con el nuevo registro de comandos
				TIM_Cmd(TIMER_GRABACION,ENABLE);						// habilitamos el timer nuevamente y bajamos la bandera de ejecutar el comando para que se levante
				setBanderaEnEstadoEjecucion(DESACTIVADA);				// cuando interrumpa el timer con el match que seteamos
			}
			estadoActual = getEstado();
		}
		if(estadoAnterior == MANUAL || (estadoAnterior == EJECUCION && estadoActual == GRABACION)){
											//detectamos si hay una transicion de estado MANUAL a GRABACION o EJECUCION a GRABACION
			regStampUpdate(0,INICIAR,0);
			TIM_Cmd(TIMER_GRABACION,DISABLE);
			leerYResetearTimer(TIMER_GRABACION);//reseteamos el timer de grabacion y deshabilitamos las interrupciones y el reseteo por MR0
			cmdMatch0InterruptTimGrabacion(DISABLE);
			TIM_Cmd(TIMER_GRABACION,ENABLE);// en caso de que hubiese estado habilitado
			getPosSteppers(posicionInicial);	// Obtenemos la posicion inicial
			estadoAnterior = GRABACION;
		}
		if(estadoAnterior == GRABACION && estadoActual == EJECUCION){ // transicion de estado de grabacion a ejecucion
			detenerMovimientos();
			TIM_Cmd(TIMER_GRABACION,DISABLE);
			uint32_t timerDetencion;
			timerDetencion = leerYResetearTimer(TIMER_GRABACION);//reseteamos el timer de grabacion y deshabilitamos las interrupciones y el reseteo por MR0
			getPosSteppers(posicionFinal);		// en caso de que hubiese estado habilitado
			int32_t motorsFDR[4]={0,0,0,0};     //esta variable almacen la cantidad de motoes que estan fuera de referencia, es decir que su posicion final no es la inicial
			Motor *motorAuxiliar;
			for(uint8_t i = 0; i<CANTIDAD_STEPPERS;i++){	//este ciclo for nos va guardando en regStamp el comando a realizar para volver cada motor a la posicion inicial
				UART_Cmd comandoDeRetorno;
				int32_t tmp = posicionFinal[i]-posicionInicial[i];
				if(tmp == 0)// si la posicion inicial es igual a la final, el motor debe permanecer detenido
					comandoDeRetorno = 96+(i+1)*3-2;  // La operacion matematica es para obtener el comando uart que sea de detencion para cada motor(esto depende del orden del enumerado UART_Cmd)
				else{
					if(tmp>0)
						comandoDeRetorno = 96+(i+1)*3; //si la posicion final es mayor que la inicial, debemos girar antihorario, por eso restamos uno al comando uart
					else
						comandoDeRetorno = 96+(i+1)*3-1;//si la posicion final es menor que la inicial, giramos horario
					motorsFDR[i] ++;

				}
				regStampUpdate((UART_Cmd)comandoDeRetorno,GUARDAR,timerDetencion); // agrega el comando de retorno al regStamp y lo ejecuta
				ejecutarUARTCmd(comandoDeRetorno);
			}
			TIM_Cmd(TIMER_GRABACION,ENABLE);
			int32_t motorsFDRG =0;
			for(uint8_t x = 0; x<CANTIDAD_STEPPERS;x++)
				motorsFDRG += motorsFDR[x];
			while(motorsFDRG!=0){ //ejecutamos este ciclo hasta que todos los motores esten en la posicion inicial
				static uint8_t i = 0;
				if(i==0){
					getPosSteppers(posicionFinal);
				}
				int32_t tmp ;
				tmp= (posicionFinal[i]-posicionInicial[i]);
				if((tmp == 0) && (motorsFDR[i]==1)){
					//detenerMovimientos();  // esta linea no se si es necesaria, pero puede llegar a serlo en caso de que sea muy impreciso el pooling que detecta si el motor
											//llego a la posicion inicial o no
					motorAuxiliar = get_motor(i);
					stop_steps(motorAuxiliar);
					TIM_Cmd(TIMER_GRABACION,DISABLE);// si el motor llego a la posicion inicial, detiene su movimiento
					timerDetencion = leerYResetearTimer(TIMER_GRABACION);
					regStampUpdate((UART_Cmd)(96+(i+1)*3-2),GUARDAR,timerDetencion); // guarda el registro de cuando este motor debe detenerse, estado en el cual inicia la secuencia
					TIM_Cmd(TIMER_GRABACION,ENABLE);
					motorsFDR[i]--;												// de grabacion
				}
				i++;
				i = i%CANTIDAD_STEPPERS;
				motorsFDRG = 0;
				for(uint8_t x = 0; x<CANTIDAD_STEPPERS;x++)
					motorsFDRG += motorsFDR[x];
			}
			TIM_Cmd(TIMER_GRABACION,DISABLE);
			regStampUpdate(0,TERMINAR,0);									// este registro determina dentro de la funcion regStampUpdate la variable estatica que sera
			registroTemporal = regStampUpdate(0,REPETIR,0);	//la cantidad de comandos guardados. Cuando enviamos el comando REPETIR, lo que hace es retornarnos
			leerYResetearTimer(TIMER_GRABACION);
			cmdMatch0InterruptTimGrabacion(ENABLE);
			TIM_UpdateMatchValue(TIMER_GRABACION,0,registroTemporal.tiempo);	//el actual registro y incrementar uno en el indice, de manera que siempre obtengamos el sigiente
			TIM_Cmd(TIMER_GRABACION,ENABLE);
			setBanderaEnEstadoEjecucion(DESACTIVADA);						//por cada llamado. Aqui se habilita el timer y
			estadoAnterior = EJECUCION;										//
		}
		if((estadoAnterior == EJECUCION||estadoAnterior ==GRABACION) && estadoActual == MANUAL){
			setBanderaEnEstadoEjecucion(DESACTIVADA);
			cmdMatch0InterruptTimGrabacion(DISABLE);
			regStampUpdate(0,TERMINAR,0);
			estadoAnterior = MANUAL;
			detenerMovimientos();
		}
	}
	return 0 ;
}

void limpiar_perifericos(void){
	LPC_TIM0->MCR = 0;
	LPC_TIM1->MCR = 0;
	return;
}


void confUart(void){
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

	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	//configuración por defecto:
	UARTConfigStruct.Baud_rate = 9600;
	UARTConfigStruct.Databits = UART_DATABIT_8;
	UARTConfigStruct.Parity = UART_PARITY_NONE;
	UARTConfigStruct.Stopbits = UART_STOPBIT_1;
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);

	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);//Inicializa FIFO
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);

	// Habilita transmisión
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0,UART_INTCFG_THRE,ENABLE);

	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0,UART_INTCFG_RBR,ENABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0,UART_INTCFG_RLS,ENABLE);
	NVIC_EnableIRQ(UART0_IRQn);
	return;
}

void confSteppers(void){
	Motor motor_0;
	motor_0.number = 0;
	motor_0.dir_portnum = 0;
	motor_0.step_portnum = 0;
	motor_0.dir_pinnum = 17;
	motor_0.step_pinnum = 15;
	motor_0.m_us_portnum = 0;
	motor_0.m1_us_pinnum = 23;
	motor_0.m2_us_pinnum = 24;
	motor_0.m3_us_pinnum = 25;
	motor_0.timer = LPC_TIM0;
	motor_0.enable_portnum = 0;
	motor_0.dir_value = DETENIDO;
	motor_0.enable_pinnum = 16;

	Motor motor_1;
	motor_1.number = 1;
	motor_1.dir_portnum = 2;
	motor_1.step_portnum = 2;
	motor_1.dir_pinnum = 1;
	motor_1.step_pinnum = 2;
	motor_1.m_us_portnum = 2;
	motor_1.m1_us_pinnum = 3;
	motor_1.m2_us_pinnum = 4;
	motor_1.m3_us_pinnum = 5;
	motor_1.timer = LPC_TIM0;
	motor_1.enable_portnum = 0;
	motor_1.dir_value = DETENIDO;
	motor_1.enable_pinnum = 11;

	Motor motor_2;
	motor_2.number = 2;
	motor_2.dir_portnum = 2;
	motor_2.step_portnum = 2;
	motor_2.dir_pinnum = 6;
	motor_2.step_pinnum = 7;
	motor_2.m_us_portnum = 2;
	motor_2.m1_us_pinnum = 8;
	motor_2.m2_us_pinnum = 10;
	motor_2.m3_us_pinnum = 11;
	motor_2.enable_portnum = 2;
	motor_2.enable_pinnum = 12;
	motor_2.dir_value = DETENIDO;
	motor_2.timer = LPC_TIM0;

	Motor motor_3;
	motor_3.number = 3;
	motor_3.dir_portnum = 0;
	motor_3.step_portnum = 0;
	motor_3.dir_pinnum = 1;
	motor_3.step_pinnum = 0;
	motor_3.m_us_portnum = 0;
	motor_3.m1_us_pinnum = 8;
	motor_3.m2_us_pinnum = 7;
	motor_3.m3_us_pinnum = 6;
	motor_3.timer = LPC_TIM0;
	motor_3.enable_portnum = 0;
	motor_3.dir_value = DETENIDO;
	motor_3.enable_pinnum = 18;

	motor_config(&motor_0);
	motor_config(&motor_1);
	motor_config(&motor_2);
	motor_config(&motor_3);
	motor_timer_init(&motor_0, STD_PERIOD);
	micro_stepping_cfg(&motor_0, 1, 1, 1);
	micro_stepping_cfg(&motor_1, 1, 1, 1);
	micro_stepping_cfg(&motor_2, 1, 1, 1);
	micro_stepping_cfg(&motor_3, 1, 1, 1);
	start_motor_timer(&motor_0);
	return;
}

void confServo(Servo_Motor* servo_motor){
	servo_motor->match_ch = 1;
	servo_motor->tic_period = 100;
	servo_motor->duty_cycle = 14;
	servo_motor->cycle = 200;
	servo_motor->inf_limit = 14;
	servo_motor->sup_limit = 38;
	servo_motor->estado = ABIERTO;
	servo_init(servo_motor);
	servo_pin_start(servo_motor);
	put_servo_motor(servo_motor);
	return;
}

Status inicializarTimerDeGrabacion(LPC_TIM_TypeDef *timerGrabacion){
	for(uint8_t i =0 ; i<CANTIDAD_STEPPERS; i++){
		Motor *stepper = get_motor(i);
		if(stepper->timer == timerGrabacion)
			return ERROR;
	}
	TIM_TIMERCFG_Type timerCfg;
	timerCfg.PrescaleOption = TIM_PRESCALE_USVAL;
	timerCfg.PrescaleValue = 100; // que se el prescaler sea de 10 us de manera de que podamos contar un tiempo bastante largo
	TIM_Init(timerGrabacion,TIM_TIMER_MODE,&timerCfg);
	TIM_Cmd(timerGrabacion,ENABLE);
	static TIM_MATCHCFG_Type matchCfg;
	matchCfg.MatchChannel =0;
	matchCfg.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	matchCfg.StopOnMatch = DISABLE;
	matchCfg.MatchValue = 1;
	matchCfg.IntOnMatch = DISABLE;
	matchCfg.ResetOnMatch = DISABLE;
	TIM_ConfigMatch(timerGrabacion,&matchCfg);
	return SUCCESS;
}

void cmdMatch0InterruptTimGrabacion(FunctionalState interrupt){
	if(interrupt ==ENABLE)
		TIMER_GRABACION->MCR |=3;
	else{
		if(interrupt == DISABLE)
			TIMER_GRABACION->MCR &= (~3);
	}
	return;
}

uint32_t leerYResetearTimer(LPC_TIM_TypeDef* timer ){
	uint32_t cuenta;
	cuenta = timer->TC;
	TIM_ResetCounter(timer);
	return cuenta;
}

Estado states(Estado estado,uint8_t put_get){
	static Estado estado_actual;
	if(put_get == PUT){
		estado_actual = estado;
		return estado;
	}
	if(put_get == GET){
		return estado_actual;
	}
	else{
		while(1){};
		return estado_actual;
	}
}

Estado getEstado(void){
	return states(0,GET);
}

void setEstado(Estado estado){
	states(estado,PUT);
	return;
}


BanderaDeEjecucion banderaEnEstadoEjecucion(BanderaDeEjecucion nuevaBandera,uint32_t put_get){
	static BanderaDeEjecucion bandera = DESACTIVADA;
	switch(put_get){
	case PUT:
		bandera = nuevaBandera;
		return nuevaBandera;
		break;
	case GET:
		return bandera;
		break;
	default:
		while(1){}
		return nuevaBandera;
	}
}

BanderaDeEjecucion getBanderaEnEstadoEjecucion(void){
	return banderaEnEstadoEjecucion(0,GET);
}

void setBanderaEnEstadoEjecucion(BanderaDeEjecucion nuevaBandera){
	banderaEnEstadoEjecucion(nuevaBandera,PUT);
	return;
}


void garraCmd(Servo_Motor* servo_motor,Garra_Cmd comando){
	uint8_t duty,cerrado,abierto;
	duty = servo_motor->duty_cycle;
	cerrado = servo_motor->sup_limit;
	abierto = servo_motor->inf_limit;
	switch(comando){
	case AGARRAR:
		if(duty <= cerrado){
			SYSTICK_IntCmd(ENABLE);
			servo_motor->estado = CERRANDO;
		}
		break;
	case SOLTAR:
		if(duty >= abierto){
			SYSTICK_IntCmd(ENABLE);
			servo_motor->estado = ABRIENDO;
		}
		break;
	}
	return;
}




void ejecutarUARTCmd(UART_Cmd comando){
	Motor *motor0 = get_motor(0);
	Motor *motor1 = get_motor(1);
	Motor *motor2 = get_motor(2);
	Motor *motor3 = get_motor(3);
	Servo_Motor *servo_motor;
	switch(comando){
	case AGARRAROSOLTAR:
		servo_motor = get_servo_motor(1);
		if(((servo_motor->estado) == CERRADO )||((servo_motor->estado) == CERRANDO ))
			garraCmd(servo_motor,SOLTAR);
		else
			garraCmd(servo_motor,AGARRAR);
		break;
	case M0HORARIO :
		start_steps(motor0,HORARIA);
		break;
	case M0ANTIHORARIO:
		start_steps(motor0,ANTI_HORARIA);
		break;
	case M0DETENER :
		stop_steps(motor0);
		break;
	case M1HORARIO :
		start_steps(motor1,HORARIA);
		break;
	case M1ANTIHORARIO:
		start_steps(motor1,ANTI_HORARIA);
		break;
	case M1DETENER :
		stop_steps(motor1);
		break;
	case M2HORARIO:
		start_steps(motor2,HORARIA);
		break;
	case M2ANTIHORARIO:
		start_steps(motor2,ANTI_HORARIA);
		break;
	case M2DETENER:
		stop_steps(motor2);
		break;
	case M3HORARIO:
		start_steps(motor3,HORARIA);
		break;
	case M3ANTIHORARIO:
		start_steps(motor3,ANTI_HORARIA);
		break;
	case M3DETENER:
		stop_steps(motor3);
		break;
	case GRABARMOVIMIENTO:
		setEstado(GRABACION);
		break;
	case DETENERYEJECUTAR:
		setEstado(EJECUCION);
		break;
	case MODOMANUAL:
		setEstado(MANUAL);
		break;
	default:
		break;
	}
	return;
}

void detenerMovimientos(void){
	for(uint8_t i = 0;i<CANTIDAD_STEPPERS;i++){
		Motor *aux = get_motor(i);
		stop_steps(aux);
	}
	return;
}

int32_t posicion(uint8_t numeroDeMotor,uint8_t put_get,DIR_Value direccion){
	static int32_t posicionMotores[] ={0,0,0,0};
	switch(put_get){
	case PUT:
		posicionMotores[numeroDeMotor]+= direccion;
		return numeroDeMotor;
		break;
	case GET:
		return posicionMotores[numeroDeMotor];
		break;
	default:
		while(1){}
		return numeroDeMotor;
		break;
	}
}


void getPosSteppers(int32_t *posicionARetornar){
	for(uint8_t i = 0;i<CANTIDAD_STEPPERS;i++ ){
		posicionARetornar[i] = posicion(i,GET,0);
	}
}

Reg_Stamp regStampUpdate(UART_Cmd comandoUart,RegStampCmd comandoStamp,uint32_t cuentaTimer ){
	static uint32_t numeroDeRegistro = 0;
	static uint32_t cantidadDeComandosGuardados;
	static Reg_Stamp regStamp[1000];
	switch(comandoStamp){
	case INICIAR:
		detenerMovimientos();
		numeroDeRegistro = 0;
		break;
	case GUARDAR:
		TIM_Cmd(TIMER_GRABACION,DISABLE);
		regStamp[numeroDeRegistro].comando = comandoUart;
		regStamp[numeroDeRegistro].tiempo = cuentaTimer;
		TIM_ResetCounter(TIMER_GRABACION);
		TIM_Cmd(TIMER_GRABACION,ENABLE);
		numeroDeRegistro++;
		return regStamp[0];
		break;
	case TERMINAR:
		cantidadDeComandosGuardados = numeroDeRegistro;
		numeroDeRegistro = 0;
		return regStamp[0];
		break;
	case REPETIR: // retorna el actual registro guardado e incrementa el contador
		;
		uint32_t auxiliar = numeroDeRegistro;
		numeroDeRegistro++;
		numeroDeRegistro%=cantidadDeComandosGuardados;
		return regStamp[auxiliar];
		break;
	}
	return regStamp[0];
}

//Handlers

void TIMER0_IRQHandler(void){
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
	for(uint8_t i = 0; i < 4; i++){
		if(get_motor_flag(i) == 1){
			Motor *motor = get_motor(i);
			do_step(motor);
			posicion(i,PUT,motor->dir_value);
		}
	}
	return;
}

void TIMER1_IRQHandler(void){
	if(TIM_GetIntStatus(TIMER_GRABACION,TIM_MR0_INT)){
		TIM_ClearIntPending(TIMER_GRABACION, TIM_MR0_INT);
		setBanderaEnEstadoEjecucion(ACTIVADA);
	}
	return;
}



void SysTick_Handler(void){
	Servo_Motor* servo_motor = get_servo_motor(1);
	if(servo_motor->estado == CERRANDO){
		if(servo_motor->duty_cycle < servo_motor->sup_limit)
			servo_motor->duty_cycle++;
		else{
			servo_motor->estado= CERRADO;
			SYSTICK_IntCmd(DISABLE);
		}
	}
	else{
		if(servo_motor->duty_cycle > servo_motor->inf_limit)
			servo_motor->duty_cycle --;
		else{
			servo_motor->estado= ABIERTO;
			SYSTICK_IntCmd(DISABLE);
		}
	}
	servo_update_duty_cycle(servo_motor);
	return;
}

void UART0_IRQHandler(void){
	uint32_t intscr,tmp;
	static uint8_t info[3];
	intscr = UART_GetIntId((LPC_UART_TypeDef *)LPC_UART0);
	tmp = intscr & UART_IIR_INTID_MASK;
	if(tmp == UART_IIR_INTID_RDA|| tmp == UART_IIR_INTID_CTI){
		UART_Receive((LPC_UART_TypeDef *)LPC_UART0,info,sizeof(info),NONE_BLOCKING);
		*(info+1) = (uint8_t)'\r';
		*(info+2) = (uint8_t)'\n';
		UART_Send((LPC_UART_TypeDef *)LPC_UART0,info,sizeof(info),NONE_BLOCKING);
		Estado estadoActual;
		estadoActual = getEstado();
		switch(estadoActual){
		case MANUAL:
			if(info[0]!= DETENERYEJECUTAR)
				ejecutarUARTCmd(info[0]);
			else{
				uint8_t errormsg[] = "No se puede ejecutar este comando bajo modo manual\n\r";
				UART_Send((LPC_UART_TypeDef *)LPC_UART0,errormsg,sizeof(errormsg),NONE_BLOCKING);
			}
			break;
		case GRABACION:
			ejecutarUARTCmd(info[0]);
			if(info[0]!=GRABARMOVIMIENTO && info[0]!=DETENERYEJECUTAR && info[0]!= MODOMANUAL)
				regStampUpdate(info[0],GUARDAR,TIMER_GRABACION->TC);  // solo se guarda el movimiento en caso de que no se afecten a los estados,
			break; 								//	es decir que los movimientos guardados afectan solo a los motores
		case EJECUCION:
			if(info[0]==MODOMANUAL || info[0] == GRABARMOVIMIENTO){
				ejecutarUARTCmd(info[0]);
			}
			break;
		}
	}
	return;
}











