/*
 * drv_mot.h
 *
 *  Created on: Dec 4, 2024
 *      Author: vincent lkme
 */

#ifndef INC_MYLIBS_H_DRV_MOT_H_
#define INC_MYLIBS_H_DRV_MOT_H_

#include "main.h"
#define ARR_MAX 4250
#define alpha_MAX 2800
#define alpha_MIN 1000
#define adapt_RATIO 0.03 //pour adapter bien

//Test values
//#define adapt_RATIO 1
//#define adapt_RATIO 0.11
//#define adapt_RATIO 0.092

//Value kept : 0.03
//#define adapt_RATIO 0.020

#define MAX_SPEED 0
// Définitions des canaux PWM
#define MTR1_CHANNEL TIM_CHANNEL_1
#define MTR2_CHANNEL TIM_CHANNEL_2

//Définition du sens de marche du robot
#define MOTION_FWD 1
#define MOTION_BACKWD 0


typedef struct drv_mot
{


	//ce n'est pas le channel qui contrôle le sens de rotation mais le signal PWM et son complémentaire
//	uint32_t Channel_Forward;
//	uint32_t Channel_Backward;

	uint32_t alpha;
	uint32_t alphastop;

	uint32_t motor_chan_id;
	uint32_t set_rpm;
	uint16_t max_speed;

	int mode;

} Motor_drv_struct;
//fresh
// Prototypes des fonctions de base
//unitary motor control : rotation
void DRV_MOT_startMot_fwd(Motor_drv_struct *motor);
void DRV_MOT_startMot_rev(Motor_drv_struct *motor);

//Bot direction control
void DRV_MOT_bot_dualFwd(Motor_drv_struct *mtr1, Motor_drv_struct *mtr2);// the bot goes forward
void DRV_MOT_bot_dualRev(Motor_drv_struct *mtr1, Motor_drv_struct *mtr2); //the bot goes backward

//General purpose fonctions needed
void DRV_MOT_Init(Motor_drv_struct *mtrinit, uint32_t mtr_chan/*, uint16_t max_speed*/);

void Bot_init (uint8_t,uint16_t pulsePercent);
void DRV_MOT_SetSpeed(Motor_drv_struct *motr, uint16_t speed);


//**********ancien à conserver *******************************
void start_pwm(int,uint32_t);
void pwm_strt (Motor_drv_struct *mtr);
void stop_alpha (Motor_drv_struct *mtr);
void turn_right();
void turn_left();



uint32_t duty_asserv(uint16_t e,uint32_t);

//**************************encodeur ********************************************************************
typedef struct __Codeur_HandleTypedef{
	TIM_HandleTypeDef *htimEncodeur;
	int frequency;
}Codeur_HandleTypeDef;

extern Codeur_HandleTypeDef hCodeurLeft;
extern Codeur_HandleTypeDef hCodeurRight;

void codeur_init(Codeur_HandleTypeDef *hcodeur, TIM_HandleTypeDef *htimEncodeur, int frequency);
int Get_speed(Codeur_HandleTypeDef *hcodeur);

#endif /* INC_MYLIBS_H_DRV_MOT_H_ */
