/**
* @file drv_sensor.h
* @brief Header du driver LIDAR LD19
* @author Jerjoub
* @date 2024
*
* @details
* Ce fichier définit l'interface du driver LIDAR incluant :
* - Les structures de données
* - Les commandes du périphérique
* - Les prototypes des fonctions
*/

#ifndef DRV_SENSOR_H_
#define DRV_SENSOR_H_

#include <stdint.h>
#include "math.h"

/**
* @defgroup Buffer_Sizes Tailles des buffers
* @{
*/
#define INFO_BUFF_SIZE 27      /**< Taille buffer informations */
#define HEALTH_BUFF_SIZE 10    /**< Taille buffer diagnostic */
#define CMD_LEN 2              /**< Longueur des commandes */
#define BUFFER_SIZE 2000       /**< Taille buffer DMA */
#define PACKET_SIZE 90         /**< Taille d'un paquet de données */
#define SCAN_SIZE 360          /**< Points par scan complet */
#define NB_DEGRES 360          /**< Résolution angulaire */
#define DETECT_THRESHOLD 150   /**< Seuil détection objet (mm) */
/** @} */

/**
* @enum device_command_t
* @brief Commandes du LIDAR
*/
typedef enum device_command_enum {
   CMD_HEADER = 0xA5,   /**< En-tête de commande */
   CMD_INIT = 0x60,     /**< Démarrage scan */
   CMD_HALT = 0x65,     /**< Arrêt scan */
   CMD_STATUS = 0x90,   /**< Requête statut */
   CMD_DIAG = 0x91,     /**< Diagnostic */
   CMD_RESET = 0x80     /**< Reset */
} device_command_t;

/**
* @defgroup Serial_Callbacks Callbacks série
* @{
*/
typedef int (*sns_transmit_t)(uint8_t *p_data, uint16_t size);      /**< Transmission bloquante */
typedef int (*sns_it_transmit_t)(uint8_t *p_data, uint16_t size);   /**< Transmission IT */
typedef int (*sns_dma_transmit_t)(uint8_t *p_data, uint16_t size);  /**< Transmission DMA */
typedef int (*sns_poll_receive_t)(uint8_t *p_data, uint16_t size);  /**< Réception bloquante */
typedef int (*sns_it_receive_t)(uint8_t *p_data, uint16_t size);    /**< Réception IT */
typedef int (*sns_dma_receive_t)(uint8_t *p_data, uint16_t size);   /**< Réception DMA */
/** @} */

/**
* @struct serial_driver_t
* @brief Interface de communication série
*/
typedef struct serial_driver_struct {
   sns_transmit_t transmit;          /**< Transmission bloquante */
   sns_transmit_t it_transmit;       /**< Transmission IT */
   sns_transmit_t dma_transmit;      /**< Transmission DMA */
   sns_poll_receive_t poll_receive;  /**< Réception bloquante */
   sns_it_receive_t it_receive;      /**< Réception IT */
   sns_dma_receive_t dma_receive;    /**< Réception DMA */
} serial_driver_t;

/**
* @struct dev_health_t
* @brief État de santé du LIDAR
*/
typedef struct dev_health_struct {
   uint16_t start_sign;    /**< Signature début */
   uint32_t lenght;        /**< Longueur données */
   uint8_t mode;           /**< Mode opération */
   uint8_t type_code;      /**< Type périphérique */
   uint8_t status_code;    /**< Code statut */
   uint16_t error_code;    /**< Code erreur */
} dev_health_t;

/**
* @struct dev_info_t
* @brief Informations du LIDAR
*/
typedef struct dev_info_struct {
   uint16_t start_sign;     /**< Signature début */
   uint32_t lenght;         /**< Longueur données */
   uint8_t mode;            /**< Mode opération */
   uint8_t type_code;       /**< Type périphérique */
   uint8_t model;           /**< Modèle */
   char firmware[6];        /**< Version firmware */
   uint8_t hardware;        /**< Version hardware */
   char serial[17];         /**< Numéro série */
} dev_info_t;

/**
* @struct object_data_t
* @brief Données d'un objet détecté
*/
typedef struct object_data_struct {
   int angle_moyen;         /**< Position angulaire (degrés) */
   int distance_moyenne;    /**< Distance (mm) */
   int count;              /**< Nombre de points */
} object_data_t;

/**
* @struct data_proc_t
* @brief Données de traitement
*/
typedef struct data_proc_struct {
   uint16_t PH;                         /**< Package Header */
   uint8_t CT;                          /**< Package Type */
   uint8_t LSN;                         /**< Sample Quantity */
   uint16_t FSA;                        /**< Start Angle */
   uint16_t LSA;                        /**< End Angle */
   uint16_t CS;                         /**< CheckSum */
   uint8_t idx;                         /**< Index courant */

   uint8_t receive_buff[BUFFER_SIZE];   /**< Buffer réception */
   uint8_t packet_data[PACKET_SIZE];    /**< Données paquet */
   int scan_data[SCAN_SIZE];            /**< Données scan */
   int clean_data[SCAN_SIZE];           /**< Données filtrées */
   object_data_t objects[100];          /**< Objets détectés */
   int obj_count;                       /**< Nombre objets */
} data_proc_t;

/**
* @struct sensor_handle_t
* @brief Structure principale du driver
*/
typedef struct sensor_handle_struct {
   serial_driver_t serial_drv;                  /**< Interface série */
   dev_health_t health_stat;                    /**< État santé */
   dev_info_t dev_info;                         /**< Infos device */

   uint8_t status_data[INFO_BUFF_SIZE];        /**< Données statut */
   uint8_t diag_data[HEALTH_BUFF_SIZE];        /**< Données diagnostic */
   uint8_t raw_data[BUFFER_SIZE];              /**< Données brutes */

   data_proc_t processing;                      /**< Données traitement */
} sensor_handle_t;

/* Prototypes des fonctions du driver */
int sns_begin(sensor_handle_t * dev);              /**< Démarre le LIDAR */
int sns_end(sensor_handle_t * dev);                /**< Arrête le LIDAR */
int sns_query_status(sensor_handle_t * dev);       /**< Interroge le statut */
int sns_check_health(sensor_handle_t * dev);       /**< Vérifie la santé */
int sns_reset(sensor_handle_t * dev);              /**< Reset du LIDAR */
void sns_parse_data(sensor_handle_t *dev);         /**< Parse les données */
int calc_avg_dist(int distances[], int debut, int fin);  /**< Calcule distance moyenne */
void detect_objects(sensor_handle_t * dev);        /**< Détecte les objets */
void smooth_data(sensor_handle_t * dev);           /**< Lisse les données */

#endif /* DRV_SENSOR_H_ */
