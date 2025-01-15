/**
 * @file drv_LIDAR.c
 * @brief Implémentation du driver LIDARX4
 */

#include "drv_LIDAR.h"
#include "stdio.h"
#include <math.h>

// Variables globales pour la communication
uint8_t g_raw_data[BUFFER_SIZE];          // Buffer de données brutes
uint8_t g_status_data[INFO_BUFF_SIZE];    // Buffer de statut
uint8_t g_diag_data[HEALTH_BUFF_SIZE];    // Buffer de diagnostic
uint8_t g_packet_data[PACKET_SIZE];       // Buffer de paquet en cours

// Variables de traitement
int g_scan_data[SCAN_SIZE];               // Données du scan brut
int g_clean_data[SCAN_SIZE];              // Données après filtrage
int g_current_idx = 0;                    // Index de traitement courant

// Variables d'analyse des objets
int g_object_angles[MAX_OBJECTS];         // Angles des objets détectés
int g_object_distances[MAX_OBJECTS];      // Distances des objets détectés
int g_object_points[MAX_OBJECTS];         // Nombre de points par objet
int g_object_count = 0;                   // Nombre d'objets détectés

// Variables de trame
uint16_t g_frame_header;                  // En-tête de trame
uint16_t g_frame_length;                  // Longueur de trame
uint8_t g_frame_type;                     // Type de trame
uint16_t g_start_angle;                   // Angle de début
uint16_t g_end_angle;                     // Angle de fin
uint16_t g_checksum;                      // Checksum de trame


// Définition des pointeurs de fonction
int (*uart_transmit)(uint8_t *p_data, uint16_t size) = NULL;
int (*uart_it_transmit)(uint8_t *p_data, uint16_t size) = NULL;
int (*uart_dma_transmit)(uint8_t *p_data, uint16_t size) = NULL;
int (*uart_poll_receive)(uint8_t *p_data, uint16_t size) = NULL;
int (*uart_it_receive)(uint8_t *p_data, uint16_t size) = NULL;
int (*uart_dma_receive)(uint8_t *p_data, uint16_t size) = NULL;

// Définition des variables globales
uint8_t g_raw_data[BUFFER_SIZE];
//int g_current_idx = 0;
uint16_t g_PH = 0;
uint8_t g_CT = 0;
uint8_t g_LSN = 0;
uint16_t g_start_angle = 0;
uint16_t g_end_angle = 0;
uint8_t g_packet_data[PACKET_SIZE];
int g_object_angles[MAX_OBJECTS];
int g_object_distances[MAX_OBJECTS];
int g_obj_count = 0;



/**
 * @brief Démarre le scan LIDAR
 * @return 0 si succès
 */
int lidar_begin() {
    uint8_t cmd_buff[2] = {CMD_HEADER, CMD_INIT};
    uart_dma_receive(g_raw_data, BUFFER_SIZE);
    uart_transmit(cmd_buff, 2);
    return 0;
}

/**
 * @brief Arrête le scan LIDAR
 * @return 0 si succès
 */
int lidar_end() {
    uint8_t cmd_buff[2] = {CMD_HEADER, CMD_HALT};
    uart_transmit(cmd_buff, 2);
    return 0;
}

/**
 * @brief Interroge le statut du LIDAR
 * @return 0 si succès
 */
int lidar_query_status() {
    uint8_t cmd_buff[2] = {CMD_HEADER, CMD_STATUS};
    uart_transmit(cmd_buff, 2);
    uart_poll_receive(g_status_data, INFO_BUFF_SIZE);

    // Format et affiche les informations reçues
    printf("Start sign : %x\r\n", (g_status_data[0]<<8) | g_status_data[1]);
    printf("Length : %ld\r\n", (uint32_t)(g_status_data[2] | (g_status_data[3]<<8) | (g_status_data[4]<<16)));
    printf("Mode : %x\r\n", g_status_data[5]);
    printf("Type code : %x\r\n", g_status_data[6]);
    printf("Model : %x\r\n", g_status_data[7]);

    // Format version firmware
    char firmware[6];
    snprintf(firmware, 6, "%d.%d", g_status_data[8], g_status_data[9]);
    printf("Firmware version : %s\r\n", firmware);
    printf("Hardware version : %d\r\n", g_status_data[10]);

    // Format numéro de série
    char serial[17];
    snprintf(serial, 17, "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
             g_status_data[11], g_status_data[12], g_status_data[13],
             g_status_data[14], g_status_data[15], g_status_data[16],
             g_status_data[17], g_status_data[18], g_status_data[19],
             g_status_data[20], g_status_data[21], g_status_data[22],
             g_status_data[23], g_status_data[24], g_status_data[25],
             g_status_data[26]);
    printf("Serial number : %s\r\n", serial);

    return 0;
}

/**
 * @brief Vérifie l'état de santé du LIDAR
 * @return 0 si succès
 */
int lidar_check_health() {
    uint8_t cmd_buff[2] = {CMD_HEADER, CMD_DIAG};
    uart_transmit(cmd_buff, 2);
    uart_poll_receive(g_diag_data, HEALTH_BUFF_SIZE);

    // Affiche les données de diagnostic
    printf("Start sign : %x\r\n", (g_diag_data[0]<<8) | g_diag_data[1]);
    printf("Length : %ld\r\n", (uint32_t)(g_diag_data[2] | (g_diag_data[3]<<8) | (g_diag_data[4]<<16)));
    printf("Mode : %x\r\n", g_diag_data[5]);
    printf("Type code : %x\r\n", g_diag_data[6]);
    printf("Status code : %x\r\n", g_diag_data[7]);
    printf("Error code : %x\r\n", g_diag_data[8] | (g_diag_data[9]<<8));

    return 0;
}

/**
 * @brief Réinitialise le LIDAR
 * @return 0 si succès
 */
int lidar_reset() {
    uint8_t cmd_buff[2] = {CMD_HEADER, CMD_RESET};
    uart_transmit(cmd_buff, 2);
    return 0;
}

/**
 * @brief Parse les données brutes en distances et angles
 */
void lidar_parse_data() {
    uint16_t signal;
    int distance;
    int angle;
    int angle_start = (g_start_angle >> 1);
    int angle_end = (g_end_angle >> 1);
    int index;

    for(int i = 0; i < g_current_idx/2; i++) {
        // Reconstruction du signal 16 bits
        signal = g_packet_data[2*i] | (g_packet_data[2*i+1] << 8);
        distance = signal/4;  // Conversion en distance

        // Calcul de l'angle interpolé
        angle = angle_start/64 + i*(angle_end-angle_start)/64/(g_frame_type-1);
        index = round(angle);

        // Filtrage des distances invalides
        if(distance > 2000 || distance < 100) {
            g_scan_data[index] = 0;
        } else {
            g_scan_data[index] = distance;
        }
    }
}

/**
 * @brief Calcule la distance moyenne sur un segment
 */
int calc_avg_dist(int debut, int fin) {
    int somme = 0;
    for(int i = debut; i <= fin; i++) {
        somme += g_clean_data[i];
    }
    return somme / (fin - debut + 1);
}

/**
 * @brief Lisse les données par filtrage adaptatif
 */
void smooth_data() {
    float temp_buffer[NB_DEGRES];
    const float gaussian_weights[5] = {0.06, 0.24, 0.4, 0.24, 0.06};
    const int half_window = 2;

    for(int i = 0; i < NB_DEGRES; i++) {
        float sum = 0;
        float weight_sum = 0;
        float local_mean = 0;
        float local_std = 0;
        int valid_samples = 0;

        // Calcul moyenne locale
        for(int j = -half_window; j <= half_window; j++) {
            int idx = i + j;
            if(idx >= 0 && idx < NB_DEGRES && g_scan_data[idx] > 0) {
                local_mean += g_scan_data[idx];
                valid_samples++;
            }
        }
        if(valid_samples > 0) {
            local_mean /= valid_samples;
        }

        // Calcul écart-type local
        valid_samples = 0;
        for(int j = -half_window; j <= half_window; j++) {
            int idx = i + j;
            if(idx >= 0 && idx < NB_DEGRES && g_scan_data[idx] > 0) {
                local_std += (g_scan_data[idx] - local_mean) * (g_scan_data[idx] - local_mean);
                valid_samples++;
            }
        }
        if(valid_samples > 1) {
            local_std = sqrt(local_std / (valid_samples - 1));
        }

        // Application du filtre
        for(int j = -half_window; j <= half_window; j++) {
            int idx = i + j;
            if(idx >= 0 && idx < NB_DEGRES) {
                float weight = gaussian_weights[j + half_window];
                if(g_scan_data[idx] > 0 && fabs(g_scan_data[idx] - local_mean) < 2 * local_std) {
                    sum += g_scan_data[idx] * weight;
                    weight_sum += weight;
                }
            }
        }

        temp_buffer[i] = (weight_sum > 0) ? sum / weight_sum : g_scan_data[i];
    }

    // Copie des résultats
    for(int i = 0; i < NB_DEGRES; i++) {
        g_clean_data[i] = (int)round(temp_buffer[i]);
    }
}

/**
 * @brief Détecte les objets dans le scan
 */
void detect_objects() {
    g_object_count = 0;
    int debut_cluster = 0;

    // Parcours des points du scan
    for(int i = 1; i < NB_DEGRES; i++) {
        // Détection discontinuité
        if(fabs(g_clean_data[i] - g_clean_data[i-1]) > DETECT_THRESHOLD) {
            // Calcul caractéristiques cluster
            int dist_moy = calc_avg_dist(debut_cluster, i-1);
            int angle_moy = (debut_cluster + i-1) / 2;

            // Sauvegarde objet détecté
            if(g_object_count < MAX_OBJECTS) {
                g_object_angles[g_object_count] = angle_moy;
                g_object_distances[g_object_count] = dist_moy;
                g_object_points[g_object_count] = i - debut_cluster;
                g_object_count++;
            }

            debut_cluster = i;
        }
    }

    // Traitement dernier cluster
    if(g_object_count < MAX_OBJECTS) {
        int dist_moy = calc_avg_dist(debut_cluster, NB_DEGRES-1);
        int angle_moy = (debut_cluster + NB_DEGRES-1) / 2;
        g_object_angles[g_object_count] = angle_moy;
        g_object_distances[g_object_count] = dist_moy;
        g_object_points[g_object_count] = NB_DEGRES - debut_cluster;
        g_object_count++;
    }
}
