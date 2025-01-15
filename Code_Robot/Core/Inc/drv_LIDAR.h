/**
 * @file drv_LIDAR.h
 * @brief Header du driver LIDAR
 * @date 2024
 *
 * @details
 * Ce fichier définit les constantes, variables globales et prototypes
 * des fonctions pour le pilotage du LIDAR
 */

#ifndef DRV_LIDAR_H_
#define DRV_LIDAR_H_

#include <stdint.h>
#include <math.h>

/**
 * @defgroup Buffer_Sizes Tailles des buffers
 * @{
 */
#define INFO_BUFF_SIZE    27      /**< Taille buffer informations */
#define HEALTH_BUFF_SIZE  10      /**< Taille buffer diagnostic */
#define BUFFER_SIZE       2000    /**< Taille buffer DMA */
#define PACKET_SIZE       90      /**< Taille d'un paquet de données */
#define SCAN_SIZE         360     /**< Points par scan complet */
#define NB_DEGRES         360     /**< Résolution angulaire */
#define MAX_OBJECTS       100     /**< Nombre maximum d'objets détectables */
/** @} */

/**
 * @defgroup Thresholds Seuils de détection
 * @{
 */
#define DETECT_THRESHOLD  150     /**< Seuil détection objet (mm) */
#define MIN_DISTANCE      100     /**< Distance minimale valide (mm) */
#define MAX_DISTANCE      2000    /**< Distance maximale valide (mm) */
/** @} */

/**
 * @defgroup Commands Commandes LIDAR
 * @{
 */
#define CMD_HEADER        0xA5    /**< En-tête de commande */
#define CMD_INIT         0x60    /**< Démarrage scan */
#define CMD_HALT         0x65    /**< Arrêt scan */
#define CMD_STATUS       0x90    /**< Requête statut */
#define CMD_DIAG         0x91    /**< Diagnostic */
#define CMD_RESET        0x80    /**< Reset */
/** @} */

/**
 * @defgroup Global_Buffers Buffers de données globaux
 * @{
 */
extern uint8_t g_raw_data[BUFFER_SIZE];           /**< Buffer données brutes */
extern uint8_t g_status_data[INFO_BUFF_SIZE];     /**< Buffer statut */
extern uint8_t g_diag_data[HEALTH_BUFF_SIZE];     /**< Buffer diagnostic */
extern uint8_t g_packet_data[PACKET_SIZE];        /**< Buffer paquet courant */
/** @} */

/**
 * @defgroup Processing_Data Variables de traitement
 * @{
 */
extern int g_scan_data[SCAN_SIZE];                /**< Données scan brut */
extern int g_clean_data[SCAN_SIZE];               /**< Données filtrées */
extern int g_current_idx;                         /**< Index courant */
/** @} */

/**
 * @defgroup Object_Detection Données objets détectés
 * @{
 */
extern int g_object_angles[MAX_OBJECTS];          /**< Angles des objets */
extern int g_object_distances[MAX_OBJECTS];       /**< Distances des objets */
extern int g_object_points[MAX_OBJECTS];          /**< Points par objet */
extern int g_object_count;                        /**< Nombre d'objets */
/** @} */

/**
 * @defgroup Frame_Data Variables de trame
 * @{
 */
extern uint16_t g_frame_header;                   /**< En-tête de trame */
extern uint16_t g_frame_length;                   /**< Longueur de trame */
extern uint8_t g_frame_type;                      /**< Type de trame */
extern uint16_t g_start_angle;                    /**< Angle de début */
extern uint16_t g_end_angle;                      /**< Angle de fin */
extern uint16_t g_checksum;                       /**< Checksum de trame */
/** @} */

/**
 * @defgroup Communication_Callbacks Pointeurs fonctions communication
 * @{
 */
// Déclaration des pointeurs de fonction
extern int (*uart_transmit)(uint8_t *p_data, uint16_t size);
extern int (*uart_it_transmit)(uint8_t *p_data, uint16_t size);
extern int (*uart_dma_transmit)(uint8_t *p_data, uint16_t size);
extern int (*uart_poll_receive)(uint8_t *p_data, uint16_t size);
extern int (*uart_it_receive)(uint8_t *p_data, uint16_t size);
extern int (*uart_dma_receive)(uint8_t *p_data, uint16_t size);
/** @} */

/**
 * @defgroup Driver_Functions Fonctions du driver
 * @{
 */

/**
 * @brief Démarre le scan LIDAR
 * @return 0 si succès
 */
int lidar_begin(void);

/**
 * @brief Arrête le scan LIDAR
 * @return 0 si succès
 */
int lidar_end(void);

/**
 * @brief Interroge le statut du LIDAR
 * @details Récupère et affiche:
 * - Version firmware/hardware
 * - Numéro de série
 * - Mode de fonctionnement
 * @return 0 si succès
 */
int lidar_query_status(void);

/**
 * @brief Vérifie l'état de santé du LIDAR
 * @details Récupère les codes de diagnostic et d'erreur
 * @return 0 si succès
 */
int lidar_check_health(void);

/**
 * @brief Réinitialise le LIDAR
 * @return 0 si succès
 */
int lidar_reset(void);

/**
 * @brief Parse les données brutes en distances et angles
 * @details Convertit les données binaires en distances (mm) et angles (degrés)
 */
void lidar_parse_data(void);

/**
 * @brief Calcule la distance moyenne sur un segment
 * @param debut Index de début du segment
 * @param fin Index de fin du segment
 * @return Distance moyenne en mm
 */
int calc_avg_dist(int debut, int fin);

/**
 * @brief Lisse les données par filtrage adaptatif
 * @details Applique un filtre gaussien avec détection d'outliers
 */
void smooth_data(void);

/**
 * @brief Détecte les objets dans le scan
 * @details Identifie les clusters et calcule leurs caractéristiques
 */
void detect_objects(void);
/** @} */

#endif /* DRV_LIDAR_H_ */
