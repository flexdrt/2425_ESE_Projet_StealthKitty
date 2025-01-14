/**
* @file drv_LIDAR.c
* @brief Implémentation du driver LIDARX4
*/

#include "drv_LIDAR.h"
#include "stdio.h"

/**
* @brief Démarre le scan LIDAR
* @details Configure la réception DMA et envoie la commande de démarrage
*
* @param dev Pointeur vers la structure du périphérique
* @return 0 si succès
*/
int sns_begin(sensor_handle_t * dev){
   uint8_t cmd_buff[CMD_LEN] = {CMD_HEADER, CMD_INIT};
   dev->serial_drv.dma_receive(dev->raw_data, BUFFER_SIZE);
   dev->serial_drv.transmit(cmd_buff, CMD_LEN);
   return 0;
}

/**
* @brief Arrête le scan LIDAR
* @param dev Pointeur vers la structure du périphérique
* @return 0 si succès
*/
int sns_end(sensor_handle_t * dev){
   uint8_t cmd_buff[CMD_LEN] = {CMD_HEADER, CMD_HALT};
   dev->serial_drv.transmit(cmd_buff, CMD_LEN);
   return 0;
}

/**
* @brief Interroge le statut du LIDAR et affiche ses informations
* @details Récupère et affiche:
* - Version firmware/hardware
* - Numéro de série
* - Mode de fonctionnement
*
* @param dev Pointeur vers la structure du périphérique
* @return 0 si succès
*/
int sns_query_status(sensor_handle_t * dev){
   uint8_t cmd_buff[CMD_LEN] = {CMD_HEADER, CMD_STATUS};
   dev->serial_drv.transmit(cmd_buff, CMD_LEN);
   dev->serial_drv.poll_receive(dev->status_data, INFO_BUFF_SIZE);

   // Parse des informations reçues
   dev->dev_info.start_sign = (dev->status_data[0]<<8) | dev->status_data[1];
   dev->dev_info.lenght = (dev->status_data[2]) | (dev->status_data[3]<<8) | (dev->status_data[4]<<16);
   dev->dev_info.mode = dev->status_data[5];
   dev->dev_info.type_code = dev->status_data[6];
   dev->dev_info.model = dev->status_data[7];

   // Formatage du numéro de version firmware
   snprintf(dev->dev_info.firmware, 6, "%d.%d", dev->status_data[8], dev->status_data[9]);
   dev->dev_info.hardware = dev->status_data[10];

   // Formatage du numéro de série
   snprintf(dev->dev_info.serial, 17, "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            dev->status_data[11], dev->status_data[12], dev->status_data[13],
            dev->status_data[14], dev->status_data[15], dev->status_data[16],
            dev->status_data[17], dev->status_data[18], dev->status_data[19],
            dev->status_data[20], dev->status_data[21], dev->status_data[22],
            dev->status_data[23], dev->status_data[24], dev->status_data[25],
            dev->status_data[26]);

   // Affichage des informations
   printf("Start sign : %x\r\n", dev->dev_info.start_sign);
   printf("Length : %ld\r\n", dev->dev_info.lenght);
   printf("Mode : %x\r\n", dev->dev_info.mode);
   printf("Type code : %x\r\n", dev->dev_info.type_code);
   printf("Model %x\r\n", dev->dev_info.model);
   printf("Firmware version : %s\r\n", dev->dev_info.firmware);
   printf("Hardware version : %d\r\n", dev->dev_info.hardware);
   printf("Serial number : %s\r\n", dev->dev_info.serial);

   return 0;
}





/**
* @brief Vérifie l'état de santé du LIDAR
* @details Récupère les codes de diagnostic et d'erreur
*
* @param dev Pointeur vers la structure du périphérique
* @return 0 si succès
*/
int sns_check_health(sensor_handle_t * dev){
   uint8_t cmd_buff[CMD_LEN] = {CMD_HEADER, CMD_DIAG};
   dev->serial_drv.transmit(cmd_buff, 2);
   dev->serial_drv.poll_receive(dev->diag_data, HEALTH_BUFF_SIZE);

   // Parse des données de diagnostic
   dev->health_stat.start_sign = (dev->diag_data[0]<<8) | dev->diag_data[1];
   dev->health_stat.lenght = (dev->diag_data[2]) | (dev->diag_data[3]<<8) | (dev->diag_data[4]<<16);
   dev->health_stat.mode = dev->diag_data[5];
   dev->health_stat.type_code = dev->diag_data[6];
   dev->health_stat.status_code = dev->diag_data[7];
   dev->health_stat.error_code = dev->diag_data[8] | (dev->diag_data[9]<<8);

   // Affichage du diagnostic
   printf("Start sign : %x\r\n", dev->health_stat.start_sign);
   printf("Length : %ld\r\n", dev->health_stat.lenght);
   printf("Mode : %x\r\n", dev->health_stat.mode);
   printf("Type code : %x\r\n", dev->health_stat.type_code);
   printf("Status code : %x\r\n", dev->health_stat.status_code);
   printf("Error code : %x\r\n", dev->health_stat.error_code);

   return 0;
}






/**
* @brief Réinitialise le LIDAR
* @param dev Pointeur vers la structure du périphérique
* @return 0 si succès
*/
int sns_reset(sensor_handle_t * dev){
   uint8_t cmd_buff[CMD_LEN] = {CMD_HEADER, CMD_RESET};
   dev->serial_drv.transmit(cmd_buff, CMD_LEN);
   return 0;
}

/**
* @brief Parse les données brutes du LIDAR en distances et angles
* @details Convertit les données binaires en :
* - Distance en mm
* - Angle en degrés
* Applique un filtrage initial (100mm < distance < 2000mm)
*
* @param dev Pointeur vers la structure du périphérique
*/
void sns_parse_data(sensor_handle_t * dev){
   uint16_t Si;      // Signal brut
   int Di;          // Distance
   int Ai;          // Angle
   int AngleFSA = (dev->processing.FSA>>1);    // Angle de début
   int AngleLSA = (dev->processing.LSA>>1);    // Angle de fin
   int index;

   // Traitement de chaque point du scan
   for(int i=0; i<dev->processing.idx/2; i++){
       // Reconstruction du signal 16 bits
       Si = dev->processing.packet_data[2*i] | (dev->processing.packet_data[2*i+1]<<8);
       Di = Si/4;  // Conversion en distance

       // Calcul de l'angle interpolé
       Ai = AngleFSA/64 + i*(AngleLSA-AngleFSA)/64/(dev->processing.LSN-1);
       index = round(Ai);

       // Filtrage des distances invalides
       if(Di>2000 || Di<100){
           dev->processing.scan_data[index] = 0;
       } else {
           dev->processing.scan_data[index] = Di;
       }
   }
}

/**
* @brief Calcule la distance moyenne sur un segment
* @param distances Tableau des distances
* @param debut Index de début
* @param fin Index de fin
* @return Distance moyenne en mm
*/
int calc_avg_dist(int distances[], int debut, int fin) {
   int somme = 0;
   for (int i = debut; i <= fin; i++) {
       somme += distances[i];
   }
   return somme / (fin - debut + 1);
}

/**
* @brief Détecte les objets dans le scan
* @details
* Algorithme de clustering qui :
* - Détecte les discontinuités de distance
* - Calcule le centre des clusters
* - Détermine la distance moyenne
*
* @param dev Pointeur vers la structure du périphérique
*/
void detect_objects(sensor_handle_t * dev) {
   int * distances = dev->processing.clean_data;
   int obj_count = 0;
   int debut_cluster = 0;

   // Parcours des points du scan
   for (int i = 1; i < NB_DEGRES; i++) {
       // Détection d'une discontinuité
       if (fabs(distances[i] - distances[i - 1]) > DETECT_THRESHOLD) {
           // Calcul des caractéristiques du cluster
           int distance_moyenne = calc_avg_dist(distances, debut_cluster, i - 1);
           int angle_moyen = (debut_cluster + i - 1) / 2;

           // Sauvegarde de l'objet détecté
           dev->processing.objects[obj_count].angle_moyen = angle_moyen;
           dev->processing.objects[obj_count].distance_moyenne = distance_moyenne;
           dev->processing.objects[obj_count].count = i - debut_cluster;

           debut_cluster = i;
           obj_count++;
       }
   }

   // Traitement du dernier cluster
   int distance_moyenne = calc_avg_dist(distances, debut_cluster, NB_DEGRES - 1);
   int angle_moyen = (debut_cluster + NB_DEGRES - 1) / 2;
   dev->processing.objects[obj_count].angle_moyen = angle_moyen;
   dev->processing.objects[obj_count].distance_moyenne = distance_moyenne;
   dev->processing.objects[obj_count].count = NB_DEGRES - debut_cluster;
   obj_count++;

   dev->processing.obj_count = obj_count;
}

///**
//* @brief Lisse les données par filtre médian
//* @details
//* Applique un filtre médian 5 points pour :
//* - Réduire le bruit
//* - Éliminer les valeurs aberrantes
//* - Préserver les discontinuités
//*
//* @param dev Pointeur vers la structure du périphérique
//*/
//void smooth_data(sensor_handle_t * dev) {
//   int * signal = dev->processing.scan_data;
//   int signal_length = NB_DEGRES;
//   int window[5];
//   int i, j, k, middle;
//   middle = 5 / 2;
//
//   // Application du filtre
//   for (i = 0; i < signal_length; i++) {
//       // Remplissage de la fenêtre
//       for (j = 0; j < 5; j++) {
//           int index = i - middle + j;
//           if (index < 0) index = 0;
//           if (index >= signal_length) index = signal_length - 1;
//           window[j] = signal[index];
//       }
//
//       // Tri de la fenêtre
//       for (j = 0; j < 5; j++) {
//           for (k = j + 1; k < 5; k++) {
//               if (window[j] > window[k]) {
//                   int temp = window[j];
//                   window[j] = window[k];
//                   window[k] = temp;
//               }
//           }
//       }
//       // Sauvegarde de la valeur médiane
//       dev->processing.clean_data[i] = window[middle];
//   }
//}
//



/**
 * @brief Lisse les données du LIDAR par filtrage adaptatif
 *
 * @details Cette fonction implémente un algorithme de lissage avancé en trois étapes :
 *          1. Détection adaptative des outliers basée sur l'écart-type local
 *          2. Moyenne mobile pondérée avec poids gaussiens
 *          3. Préservation des discontinuités significatives
 *
 * Le filtrage s'adapte automatiquement aux caractéristiques locales du signal :
 * - Zones bruitées : filtrage plus agressif
 * - Discontinuités : préservation des transitions importantes
 * - Zones stables : lissage léger pour préserver les détails
 *
 * @param dev          Pointeur vers la structure du périphérique LIDAR
 *
 * @note Les données filtrées sont stockées dans dev->processing.clean_data
 *       Le type des données reste inchangé (int)
 *       La taille du signal reste NB_DEGRES (360)
 */
void smooth_data(sensor_handle_t* dev) {
    int* signal = dev->processing.scan_data;
    const int signal_length = NB_DEGRES;
    const int window_size = 5;
    const int half_window = window_size / 2;

    // Coefficients gaussiens pour la pondération (somme = 1.0)
    const float gaussian_weights[5] = {0.06, 0.24, 0.4, 0.24, 0.06};

    // Buffer temporaire pour les calculs
    float temp_buffer[NB_DEGRES];

    // Pour chaque point du signal
    for(int i = 0; i < signal_length; i++) {
        float sum = 0;
        float weight_sum = 0;
        float local_std = 0;
        float local_mean = 0;
        int valid_samples = 0;

        // 1. Calcul de la moyenne locale
        for(int j = -half_window; j <= half_window; j++) {
            int idx = i + j;
            if(idx >= 0 && idx < signal_length && signal[idx] > 0) {
                local_mean += signal[idx];
                valid_samples++;
            }
        }
        if(valid_samples > 0) {
            local_mean /= valid_samples;
        }

        // 2. Calcul de l'écart-type local
        valid_samples = 0;
        for(int j = -half_window; j <= half_window; j++) {
            int idx = i + j;
            if(idx >= 0 && idx < signal_length && signal[idx] > 0) {
                local_std += (signal[idx] - local_mean) * (signal[idx] - local_mean);
                valid_samples++;
            }
        }
        if(valid_samples > 1) {
            local_std = sqrt(local_std / (valid_samples - 1));
        }

        // 3. Application du filtre avec seuil adaptatif
        for(int j = -half_window; j <= half_window; j++) {
            int idx = i + j;
            if(idx >= 0 && idx < signal_length) {
                float weight = gaussian_weights[j + half_window];

                // Détection des outliers (seuil = 2 écarts-types)
                if(signal[idx] > 0 &&
                   fabs(signal[idx] - local_mean) < 2 * local_std) {
                    sum += signal[idx] * weight;
                    weight_sum += weight;
                }
            }
        }

        // 4. Calcul de la valeur filtrée
        if(weight_sum > 0) {
            temp_buffer[i] = sum / weight_sum;
        } else {
            temp_buffer[i] = signal[i];  // Garde la valeur originale si pas de voisins valides
        }
    }

    // 5. Transfert des résultats avec préservation du type
    for(int i = 0; i < signal_length; i++) {
        dev->processing.clean_data[i] = (int)round(temp_buffer[i]);
    }
}
