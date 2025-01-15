/*
 ******************************************************************************
 * @attention
 * Author: AMEUR
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include "ADXL343_SPI.h"

#include "spi.h"

// Macros pour contrôler le signal CS (Chip Select)
#define ADXL_CS_LOW()   HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET)
#define ADXL_CS_HIGH()  HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET)


/**
 * @brief Initialisation de l'ADXL343 via SPI.
 * @param hspi: Pointeur vers la structure SPI_HandleTypeDef utilisée pour SPI.
 * @param dev: Pointeur vers la structure ADXL343 contenant les informations du périphérique.
 * @return 0 si l'initialisation est réussie, 255 sinon.
 */
uint8_t ADXL343_initialization(SPI_HandleTypeDef *hspi, ADXL343 *dev)
{
    dev->hspiHandle = hspi;
    uint8_t regAddr = ADXL343_REG_DEVICE_ID | 0x80; // Lecture du registre DEVICE_ID
    uint8_t regData = 0;
    HAL_StatusTypeDef status;

    printf("Starting ADXL343 initialization...\r\n");

    // Lecture du DEVICE_ID
    ADXL_CS_LOW();
    status = HAL_SPI_Transmit(hspi, &regAddr, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        printf("Erreur SPI lors de l'envoi de l'adresse.\r\n");
        ADXL_CS_HIGH();
        return 255; // Erreur
    }
    status = HAL_SPI_Receive(hspi, &regData, 1, HAL_MAX_DELAY);
    ADXL_CS_HIGH();

    if (status != HAL_OK) {
        printf("Erreur SPI lors de la réception des données.\r\n");
        return 255; // Erreur
    }

    // Vérification du DEVICE_ID
    if (regData != ADXL343_DEVICE_ID) {
        printf("Erreur SPI : Device ID attendu 0xE5, reçu 0x%02X\r\n", regData);
        return 255; // Erreur
    }

    printf("ADXL343 détecté avec succès : Device ID = 0x%02X\r\n", regData);

    // Configurer le mode mesure
    uint8_t powerCtl = 0x08; // Activer le mode mesure
    ADXL343_write_register(hspi, ADXL343_REG_POWER_CTL, powerCtl);

    return 0; // Initialisation réussie
}

/**
 * @brief Écrit une valeur dans un registre de l'ADXL343.
 * @param hspi: Pointeur vers la structure SPI_HandleTypeDef utilisée pour SPI.
 * @param regAddr: Adresse du registre où écrire.
 * @param regData: Valeur à écrire dans le registre.
 */
void ADXL343_write_register(SPI_HandleTypeDef *hspi, uint8_t regAddr, uint8_t regData)
{
    uint8_t addr = regAddr & 0x7F; // Assurez-vous que le bit MSB est 0 pour l'écriture
    ADXL_CS_LOW();
    HAL_SPI_Transmit(hspi, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(hspi, &regData, 1, HAL_MAX_DELAY);
    ADXL_CS_HIGH();

    // Vérification de l'écriture
    uint8_t verify = ADXL343_read_register(hspi, regAddr);
    if (verify != regData) {
        printf("Erreur : le registre 0x%02X contient 0x%02X (attendu 0x%02X)\r\n", regAddr, verify, regData);
    } else {
        printf("Registre 0x%02X configuré avec succès : 0x%02X\r\n", regAddr, regData);
    }
}


/**
 * @brief Lit une valeur depuis un registre de l'ADXL343.
 * @param hspi: Pointeur vers la structure SPI_HandleTypeDef utilisée pour SPI.
 * @param regAddr: Adresse du registre à lire.
 * @return La valeur lue depuis le registre.
 */
uint8_t ADXL343_read_register(SPI_HandleTypeDef *hspi, uint8_t regAddr)
{
	uint8_t regData;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);  // Mettre NSS à 0 pour commencer la communication SPI
	regAddr |= 0x80;
	HAL_SPI_Transmit(hspi, &regAddr, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(hspi, &regData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);    // Mettre NSS à 1 pour terminer la communication SPI
	return regData;
}

/**
 * @brief Lit les données brutes d'accélération de l'ADXL343.
 * @param dev: Pointeur vers la structure ADXL343 contenant les informations du périphérique.
 * @return HAL_OK si la lecture est réussie, sinon une erreur HAL.
 */
HAL_StatusTypeDef ADXL343_get_acc_raw(ADXL343 *dev) {
    uint8_t data[6]; // Pour stocker les données X, Y, Z
    uint8_t regAddr = ADXL343_REG_DATAX0 | 0xC0; // Activation de lecture multiple

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET); // Mettre CS à bas
    HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY); // Envoyer l'adresse du registre
    HAL_SPI_Receive(dev->hspiHandle, data, 6, HAL_MAX_DELAY); // Recevoir les 6 octets de données
    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET); // Mettre CS à haut

    // Convertir les données en entiers 16 bits (X, Y, Z)
    dev->acc_rawX = (int16_t)((data[1] << 8) | data[0]);
    dev->acc_rawY = (int16_t)((data[3] << 8) | data[2]);
    dev->acc_rawZ = (int16_t)((data[5] << 8) | data[4]);

    // Vérification des valeurs lues
    printf("Acceleration Raw  Data brut  - X: %d, Y: %d, Z: %d\n",
           dev->acc_rawX, dev->acc_rawY, dev->acc_rawZ);

    return HAL_OK;
}

/**
 * @brief Convertit les données brutes en valeurs normalisées.
 * @param dev: Pointeur vers la structure ADXL343 contenant les informations du périphérique.
 * @return HAL_OK si la conversion est réussie.
 */
HAL_StatusTypeDef ADXL343_get_acc_norm(ADXL343 *dev) {

	float scaleFactor = 0.004;
	dev->acc_msp2X = dev->acc_rawX * scaleFactor;
	dev->acc_msp2Y = dev->acc_rawY * scaleFactor;
	dev->acc_msp2Z = dev->acc_rawZ * scaleFactor;

	return HAL_OK;
}


void ADXL343_print_raw(ADXL343 *dev, UART_HandleTypeDef *huart) {
	printf("Acceleration Raw Data normalisé - X: %d, Y: %d, Z: %d\n",
			dev->acc_rawX, dev->acc_rawY, dev->acc_rawZ);
}


/**
 * @brief Configure les paramètres de détection de tapotements ("Tap") pour l'ADXL343.
 * @param dev: Pointeur vers la structure ADXL343 contenant les informations du périphérique.
 * @param threshold: Seuil de détection du tapotement.
 * @param duration: Durée minimale pour considérer un tapotement valide.
 * @param latency: Délai pour un double tapotement.
 * @param window: Fenêtre de temps pour le double tapotement.
 */
void ADXL343_set_tap_parameters(ADXL343 *dev, uint8_t threshold, uint8_t duration, uint8_t latency, uint8_t window) {
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_THRESH_TAP, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &threshold, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	// Configurer la durée du tapotement
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_DUR, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &duration, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	// Configurer la latence pour double tapotement
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_LATENT, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &latency, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	// Configurer la fenêtre pour double tapotement
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_WINDOW, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &window, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);
}


/*void ADXL343_EnableTapInterrupts(ADXL343 *dev) {

	// Configurer l'interruption de mappage INT1
	uint8_t int_map = 0x40; // Mappage des interruptions à INT1
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_INT_MAP, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &int_map, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	uint8_t int_axes = 0x07;

	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_TAP_AXES , 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &int_axes, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	uint8_t int_enable = 0x40; // Activer SINGLE_TAP et DOUBLE_TAP (bits 6 et 5)

	// Activer les interruptions
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->hspiHandle, &ADXL343_REG_INT_ENABLE, 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(dev->hspiHandle, &int_enable, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

	uint8_t int_enable_read = ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_ENABLE);
	uint8_t int_map_read = ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_MAP);

	printf("Valeur lue du registre INT_ENABLE après écriture: 0x%02X\n", int_enable_read);

	printf("Valeur du registre INT_MAP après écriture: 0x%02X\n", int_map_read);
}*/

void ADXL343_EnableTapInterrupts(ADXL343 *dev)
{
    // Activer les interruptions SINGLE_TAP
    uint8_t intEnable = 0x40;
    ADXL343_write_register(dev->hspiHandle, ADXL343_REG_INT_ENABLE, intEnable);

    // Configurer les axes pour les tapotements
    uint8_t tapAxes = 0x07;
    ADXL343_write_register(dev->hspiHandle, ADXL343_REG_TAP_AXES, tapAxes);

    // Vérification
    uint8_t intEnableRead = ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_ENABLE);
    uint8_t tapAxesRead = ADXL343_read_register(dev->hspiHandle, ADXL343_REG_TAP_AXES);

    printf("INT_ENABLE configuré : 0x%02X\r\n", intEnableRead);
    printf("TAP_AXES configuré : 0x%02X\r\n", tapAxesRead);
}

/**
 * @brief Vérifie si un "Single Tap" a été détecté.
 * @param dev: Pointeur vers la structure ADXL343 contenant les informations du périphérique.
 * @return 1 si un tapotement est détecté, 0 sinon.
 */
int ADXL343_check_tap(ADXL343 *dev) {
    uint8_t intSource = ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_SOURCE);

    // Si un "Single Tap" est détecté
    if (intSource & 0x40) {
        printf("Single Tap detected!\n");

        // Réinitialiser le registre d'interruption pour éviter des lectures multiples
        ADXL343_read_register(dev->hspiHandle, ADXL343_REG_INT_SOURCE);

        return 1; // "Tap" détecté
    }

    // Aucun "Tap" détecté
    return 0;
}

/**
 * @brief Configure le format des données de l'ADXL343.
 * @param hspi: Pointeur vers la structure SPI_HandleTypeDef utilisée pour SPI.
 * @param range: Plage de mesure (par exemple, ±2g, ±4g, etc.).
 */
void ADXL343_SetDataFormat(SPI_HandleTypeDef *hspi, uint8_t range)
{
    uint8_t dataFormat = 0x08 | (range & 0x03); // Activer FULL_RES et définir la plage
    ADXL343_write_register(hspi, ADXL343_REG_DATA_FORMAT, dataFormat);

    printf("DATA_FORMAT configuré : 0x%02X\r\n", ADXL343_read_register(hspi, ADXL343_REG_DATA_FORMAT));
}


/*void ADXL343_SetDataFormat(SPI_HandleTypeDef *hspi) {
    // Configuration du registre DATA_FORMAT
    uint8_t data_format = 0x11; // Valeur par défaut

    // Activation du SELF_TEST (bit 7)
    data_format |= (1 << 7); // Définit le bit SELF_TEST à 1

    // Configuration en mode SPI à 3 fils (bit 6)
    data_format |= (1 << 6); // Définit le bit SPI à 1

    // Autres configurations optionnelles
    // Par exemple, pour FULL_RES (bit 3), Range ou autre; ajustez selon vos besoins.
    // data_format |= (1 << 3); // Si vous voulez activer FULL_RES

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);

    // Écriture dans le registre DATA_FORMAT
    HAL_SPI_Transmit(hspi, &ADXL343_REG_DATA_FORMAT, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(hspi, &data_format, 1, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);}*/

/*void ADXL343_SetDataFormat(ADXL343 *dev, uint8_t range) {
    uint8_t regAddr = ADXL343_REG_DATA_FORMAT;  // Adresse du registre DATA_FORMAT
    uint8_t regData = 0x08 | (range & 0x03);    // Activer FULL_RES et définir la plage de mesure
                                                // Les bits 2:1 du range doivent être configurés correctement.
    ADXL_CS_LOW();
    HAL_SPI_Transmit(dev->hspiHandle, &regAddr, 1, HAL_MAX_DELAY); // Écriture de l'adresse
    HAL_SPI_Transmit(dev->hspiHandle, &regData, 1, HAL_MAX_DELAY); // Écriture des données
    ADXL_CS_HIGH();

    printf("DATA_FORMAT configuré : 0x%02X\n", regData);
}*/


// TODO FINISH the rest of the functionalities in SPI
