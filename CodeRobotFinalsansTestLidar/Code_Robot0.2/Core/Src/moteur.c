/*
 ******************************************************************************
 * @attention
 * Author: CHAIR
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include "moteur.h"
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"

// Gains PID
float kp = 1.0;   // Gain proportionnel
float ki = 0.1;   // Gain intégral
float kd = 0.05;  // Gain dérivé

// Erreurs intégrales pour chaque moteur
float integral_error_left = 0.0;
float integral_error_right = 0.0;

// Erreurs précédentes pour chaque moteur
float previous_error_left = 0.0;
float previous_error_right = 0.0;


// Pointeurs vers les timers utilisés pour les moteurs
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3; // TIM3 pour l'encodeur moteur droit
extern TIM_HandleTypeDef htim4; // TIM4 pour l'encodeur moteur gauche


/**
 * @brief Initialisation des moteurs (PWM et encodeurs).
 */
void init_motors(void) {
	// Initialisation des PWM pour les moteurs
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   // TIM1_CH1
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   // TIM1_CH2
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);

	// Initialisation des encodeurs
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // Moteur 1 (encodeur sur TIM3)
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // Moteur 2 (encodeur sur TIM4)

	// Réinitialisation des compteurs d'encodeurs
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	__HAL_TIM_SET_COUNTER(&htim4, 0);
}


/**
 * @brief Avance le moteur droit.
 * @param alpha: Valeur du rapport cyclique (0-100%).
 */
void forward_r(uint16_t alpha) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, alpha);  // TIM1_CH1
	//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);     // TIM1_CH2N
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   // TIM1_CH1
	//HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);   // TIM1_CH2

	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
}

/**
 * @brief Avance le moteur gauche.
 * @param alpha: Valeur du rapport cyclique (0-100%).
 */
void forward_l(uint16_t alpha) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, alpha);  // TIM1_CH2
	//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);     // TIM1_CH1N
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);   // TIM1_CH1
	//HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);   // TIM1_CH2

	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
}

/**
 * @brief Recule le moteur droit.
 * @param alpha: Valeur du rapport cyclique (0-100%).
 */
void reverse_r(uint16_t alpha) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, alpha); // TIM1_CH2N
	//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);      // TIM1_CH1
	//    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   // TIM1_CH1
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);   // TIM1_CH2

	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	//   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);

}

/**
 * @brief Recule le moteur gauche.
 * @param alpha: Valeur du rapport cyclique (0-100%).
 */
void reverse_l(uint16_t alpha) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, alpha); // TIM1_CH1N
	// __HAL_TIM_SET_COMPARE(&htim1, TIM_CH0ANNEL_2, 0);      // TIM1_CH2
	//HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   // TIM1_CH1
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   // TIM1_CH2

	HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
	//   HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
}

/**
 * @brief Arrête le moteur droit.
 */
void stop_r(void) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);      // TIM1_CH1
	//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);     // TIM1_CH2N
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);   // TIM1_CH1N
	//HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);   // TIM1_CH2

}

/**
 * @brief Arrête le moteur gauche.
 */
void stop_l(void) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);      // TIM1_CH2
	//__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);     // TIM1_CH1N
	//	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);   // TIM1_CH1N
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);   // TIM1_CH2
}

/**
 * @brief Lit la position actuelle de l'encodeur.
 * @param motor: Identifiant du moteur (MOTOR_LEFT ou MOTOR_RIGHT).
 * @return Position actuelle de l'encodeur.
 */
int16_t get_encoder_position(uint8_t motor) {
	int16_t position = 0;

	if (motor == MOTOR_LEFT) {
		position = __HAL_TIM_GET_COUNTER(&htim4);  // Lire le compteur du moteur gauche
	} else if (motor == MOTOR_RIGHT) {
		position = __HAL_TIM_GET_COUNTER(&htim3);  // Lire le compteur du moteur droit
	}

	return position;
}

/**
 * @brief Réinitialise la position de l'encodeur.
 * @param motor: Identifiant du moteur (MOTOR_LEFT ou MOTOR_RIGHT).
 */
void reset_encoder(uint8_t motor) {
	if (motor == MOTOR_LEFT) {
		__HAL_TIM_SET_COUNTER(&htim4, 0);  // Réinitialiser le compteur du moteur gauche
	} else if (motor == MOTOR_RIGHT) {
		__HAL_TIM_SET_COUNTER(&htim3, 0);  // Réinitialiser le compteur du moteur droit
	}
}



/**
 * @brief Calcule la vitesse actuelle du moteur.
 * @param motor: Identifiant du moteur (MOTOR_LEFT ou MOTOR_RIGHT).
 * @param delta_time_ms: Intervalle de temps en ms.
 * @param encoder_resolution: Résolution de l'encodeur (pulsations par tour).
 * @return Vitesse en tours par seconde.
 */
float calculate_motor_speed(uint8_t motor, uint32_t delta_time_ms, uint16_t encoder_resolution) {
	static int16_t last_position_motor1 = 0;
	static int16_t last_position_motor2 = 0;

	int16_t current_position = 0;
	int16_t delta_position = 0;

	if (motor == MOTOR_LEFT) { // Moteur gauche
		current_position = __HAL_TIM_GET_COUNTER(&htim3); // TIM3 pour moteur gauche
		delta_position = current_position - last_position_motor1;
		last_position_motor1 = current_position;
	} else if (motor == MOTOR_RIGHT) { // Moteur droit
		current_position = __HAL_TIM_GET_COUNTER(&htim4); // TIM4 pour moteur droit
		delta_position = current_position - last_position_motor2;
		last_position_motor2 = current_position;
	}

	// Calculer la vitesse en tours par seconde
	float speed = (float)delta_position / encoder_resolution; // Tours par intervalle
	speed *= (1000.0f / delta_time_ms); // Convertir en tours par seconde

	return speed;
}


/**
 * @brief Calcule la commande PID pour un moteur.
 * @param target_speed: Vitesse cible.
 * @param measured_speed: Vitesse mesurée.
 * @param integral_error: Accumulateur d'erreur intégrale.
 * @param previous_error: Dernière erreur mesurée.
 * @param kp: Gain proportionnel.
 * @param ki: Gain intégral.
 * @param kd: Gain dérivé.
 * @return Commande PID calculée.
 */
float compute_pid(float target_speed, float measured_speed, float *integral_error, float *previous_error, float kp, float ki, float kd) {
	float error = target_speed - measured_speed;

	// Calcul de la partie intégrale
	*integral_error += error;

	// Calcul de la partie dérivée
	float derivative = error - *previous_error;

	// Mise à jour de l'erreur précédente
	*previous_error = error;

	// Calcul de la commande PID
	float output = kp * error + ki * (*integral_error) + kd * derivative;
	return output;
}

/**
 * @brief Mode "Souris" pour un déplacement normal.
 */
void motorcontrol_souris(void) {
	uint16_t alpha = 3400;  // 40% de 8500
	uint16_t distance = 0;  // Distance mesurée par le capteur
	const uint32_t delta_time_ms = 50;  // Intervalle de temps pour le calcul
	const uint16_t encoder_resolution = 1024;  // Nombre d'impulsions par tour

	printf("Mode Souris : moteurs avancent à vitesse normale.\n");
	forward_r(alpha + 450);
	forward_l(alpha + 10);

	// Calcul et affichage des vitesses
	float speed_left = calculate_motor_speed(MOTOR_LEFT, delta_time_ms, encoder_resolution);
	float speed_right = calculate_motor_speed(MOTOR_RIGHT, delta_time_ms, encoder_resolution);

	printf("Vitesse moteur gauche : %.2f tours/s\n", speed_left);
	printf("Vitesse moteur droit : %.2f tours/s\n", speed_right);

	printf("Position moteur gauche : %d\n", __HAL_TIM_GET_COUNTER(&htim3));
	printf("Position moteur droit : %d\n", __HAL_TIM_GET_COUNTER(&htim4));


	vTaskDelay(pdMS_TO_TICKS(delta_time_ms)); // Vérification toutes les 100 ms
}


/**
 * @brief Mode "Prédateur" pour un déplacement rapide.
 */
void motorcontrol_predateur(void) {
	uint16_t alpha = 3400;  // 30% de 8500
	uint16_t distance = 0;  // Distance mesurée par le capteur
	const uint32_t delta_time_ms = 50;  // Intervalle de temps pour le calcul
	const uint16_t encoder_resolution = 1024;  // Nombre d'impulsions par tour

	printf("Mode Prédateur : moteurs avancent à grande vitesse.\n");
	forward_r(alpha + 1850);
	forward_l(alpha+ 50);


	// Calcul et affichage des vitesses
	float speed_left = calculate_motor_speed(MOTOR_LEFT, delta_time_ms, encoder_resolution);
	float speed_right = calculate_motor_speed(MOTOR_RIGHT, delta_time_ms, encoder_resolution);

	printf("Vitesse moteur gauche : %.2f tours/s\n", speed_left);
	printf("Vitesse moteur droit : %.2f tours/s\n", speed_right);

	printf("Position moteur gauche : %d\n", __HAL_TIM_GET_COUNTER(&htim3));
	printf("Position moteur droit : %d\n", __HAL_TIM_GET_COUNTER(&htim4));


	vTaskDelay(pdMS_TO_TICKS(delta_time_ms)); // Vérification toutes les 100 ms
}

/**
 * @brief Effectue un pivot arrière en cas d'obstacle.
 */
void pivot(void) {
	uint16_t alpha = 1200;  // 30% de 8500

	// Arrêter les moteurs
	stop_r();
	stop_l();
	printf("Danger détecté : moteur arrêté. Pivot arrière en cours...\n");

	// Effectuer un pivot en arrière
	reverse_r(alpha + 3050);       // Moteur droit recule
	reverse_l(alpha );   // Moteur gauche recule plus lentement pour pivoter
	vTaskDelay(pdMS_TO_TICKS(600)); // Pivot pendant 1,5 seconde

	// Arrêter les moteurs après le pivot
	stop_r();
	stop_l();

	vTaskDelay(pdMS_TO_TICKS(200));

	forward_r(alpha+ 2450);

	vTaskDelay(pdMS_TO_TICKS(320));

	printf("Pivot terminé, moteurs arrêtés.\n");
}

