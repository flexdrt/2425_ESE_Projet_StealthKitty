/*
 * drv_mot.c
 *
 *  Created on: Dec 4, 2024
 *      Author: vincent lkme
 */

#include "drv_mot.h"
//#include "drv_mot_3.h"
#include "tim.h"
#include "stm32g4xx_hal_tim.h"

// Variable statique pour stocker le handle du timer utilisé
static TIM_HandleTypeDef *motor_timer = NULL;

/**
 * @brief Initialisation des moteurs en PWM.
 * @param htim : Handle du timer utilisé pour les moteurs.
 */
//start motor in Forward
void DRV_MOT_startMot_fwd(Motor_drv_struct *motor){

	/*CH1*/	HAL_TIM_PWM_Start(&htim1, motor->motor_chan_id); //PWM -->pin FWD PWM
	/*CH1n*/	HAL_TIMEx_PWMN_Stop(&htim1, motor->motor_chan_id); //L --> PIN REV

}

//start motor in Reverse
void DRV_MOT_startMot_rev(Motor_drv_struct *motor){

	HAL_TIMEx_PWMN_Start(&htim1, motor->motor_chan_id); // PWM --> PIN REV L
	HAL_TIM_PWM_Stop(&htim1, motor->motor_chan_id); // L-->pin FWD
}

// stopper un moteur
void DRV_MOT_stoptMot (Motor_drv_struct *mtr){
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, &mtr->alpha_stop);
}

//Two wheels robot same rotation sign
//
void DRV_MOT_bot_dualFwd(Motor_drv_struct *mtr1, Motor_drv_struct *mtr2){

	//Un moteur dans un sens FWD
	DRV_MOT_startMot_fwd(mtr1);

	//un moteur dans le sens ReV
	DRV_MOT_startMot_rev(mtr2);

}

void DRV_MOT_bot_dualRev(Motor_drv_struct *mtr1, Motor_drv_struct *mtr2){
	//the motors goes in direction opposite defined in fonction	DRV_MOT_bot_dualFwd

	//Un moteur dans un sens Rev
	DRV_MOT_startMot_rev(mtr1);

	//un moteur dans le sens ReV
	DRV_MOT_startMot_fwd(mtr2);
}



//2 motors passed as arguments
//Constructor
void DRV_MOT_Init(Motor_drv_struct *mtrinit, uint32_t mtr_chan) {

	mtrinit->motor_chan_id=mtr_chan;

}

void DRV_MOT_SetSpeed(Motor_drv_struct *motr, uint16_t speed) {

	__HAL_TIM_SET_COMPARE(&htim1, motr->motor_chan_id, speed);
}

void Bot_init (uint8_t direction,uint16_t pulsePercent)
//bot component
{//we create one motor
	Motor_drv_struct mtr_left;// motor considered as the left for the bot
	Motor_drv_struct mtr_right;//motor considered as the right for the bot


	// init les deux motors (affecter un channel a chaque)

	DRV_MOT_Init(&mtr_left,MTR1_CHANNEL);//channel1
	DRV_MOT_Init(&mtr_right,MTR2_CHANNEL);//channel 2



	//Launch in forward the bot

	// si on veut que le bot avance au début à définir
	// si oui

	//Convertir le rapport cyclique from  percent to ARR

	// Récupération de la valeur maximale (ARR) du timer
	uint32_t ARR = __HAL_TIM_GET_AUTORELOAD(&htim1);

	// Conversion du pourcentage en valeur ARR utilisable
	uint32_t pulse;

	pulse = (pulsePercent * ARR) / 100;


	//rapport cyclique fixe
	//pulse=2125;

	if(direction==1){
		DRV_MOT_bot_dualFwd(&mtr_left,&mtr_right);
		//To update the speed when the direction change, some direction may need different speed

		//		DRV_MOT_SetSpeed(&mtr_left, 2125);
		//		DRV_MOT_SetSpeed(&mtr_right,2125);

//correct
			DRV_MOT_SetSpeed(&mtr_left,pulse);
			DRV_MOT_SetSpeed(&mtr_right, adapt_RATIO * pulse);//adaptation sur moteur + rapide motor right
//wrong
//			DRV_MOT_SetSpeed(&mtr_left, adapt_RATIO * pulse);//moteur le plus  rapide theorie
//				DRV_MOT_SetSpeed(&mtr_right, pulse);
	}

	else{//si on veut qu'il recule
		DRV_MOT_bot_dualRev(&mtr_left,&mtr_right);

		DRV_MOT_SetSpeed(&mtr_left, adapt_RATIO * pulse);
		DRV_MOT_SetSpeed(&mtr_right,pulse);

	}

}

