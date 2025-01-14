/*
 ******************************************************************************
 * @attention
 * Author: RAFIK CHAIR
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef MOTEUR_MOTEUR_H_
#define MOTEUR_MOTEUR_H_

#include "stm32G4xx_hal.h"

#define MOTOR_LEFT   0
#define MOTOR_RIGHT  1

void init_motors(void);

void forward_r(uint16_t alpha);
void forward_l(uint16_t alpha);
void reverse_r(uint16_t alpha);
void reverse_l(uint16_t alpha);
void stop_r(void);
void stop_l(void);

// Fonctions pour les encodeurs
int16_t get_encoder_position(uint8_t motor);  // Lire la position de l'encodeur
void reset_encoder(uint8_t motor);            // Réinitialiser l'encodeur

// Nouvelle fonction : Calculer la vitesse
float calculate_motor_speed(uint8_t motor, uint32_t delta_time_ms, uint16_t encoder_resolution);
float compute_pid(float target_speed, float measured_speed, float *integral_error, float *previous_error, float kp, float ki, float kd);
void motorcontrol_souris(void);
void motorcontrol_predateur(void);
void pivot(void);


#endif /* MOTEUR_MOTEUR_H_ */
