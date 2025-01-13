


# 🐱 **StealthKitty - Projet Robot Chat**  

 <img src="https://megabase.eduniversal.com/uploads/school/1155/logo.jpg?v=c7e3965d14625a3e4b5662bd550d93e0" alt="Logo ENSEA" width="30"/> ![STM32](https://img.shields.io/badge/STM32-Embedded-blue?style=for-the-badge&logo=stmicroelectronics)  
[![ENSEA](https://img.shields.io/badge/ENSEA-3A--ESE-green?style=for-the-badge&logo=https://upload.wikimedia.org/wikipedia/fr/8/82/Logo_ENSEA.svg)](https://www.ensea.fr)
![Status](https://img.shields.io/badge/Status-Finished-orange?style=for-the-badge)  


---

## 📜 **Description**  
StealthKitty est un projet de **système embarqué innovant** basé sur un **STM32**.  
🎯 **Objectif :** Développer plusieurs robots capables de se déplacer sur une table sans bordure.  
- 🐾 Un robot est désigné comme **"chat"** et doit attraper un autre robot, qui devient alors le nouveau **"chat"**.  

Ce projet est réalisé dans le cadre de la dernière année de la filière **électronique et systèmes embarqués (ESE)** de l'ENSEA.

### **Les contributeurs :**  
- 👨‍💻 **Vincent LAKHMECHE**  
- 👨‍💻 **Mohamed Rafik CHAIR**  
- 👨‍💻 **Meziane Ameur**  
- 👨‍💻 **Karim JERJOUB**  
![image](https://github.com/user-attachments/assets/0b8a3520-2e76-4162-91c8-48ae93badee7)
---

## 📚 **Table des matières**  
1. [📖 Contexte](#-contexte)  
2. [✨ Fonctionnalités](#-fonctionnalités)  
3. [🔧 Matériel utilisé](#-matériel-utilisé)  
4. [📐 Architecture](#-architecture)  
5. [🚀 Utilisation](#-utilisation)  
6. [👥 Auteurs](#-auteurs)  
7. [📄 Licence](#-licence)  

---

## 📖 **Contexte**  
Ce projet s'inscrit dans le cadre de la formation 3A à l'ENSEA.  
🎯 L'objectif est de concevoir un système embarqué complet, depuis la conception électronique jusqu'à l'implémentation logicielle.

---

## ✨ **Fonctionnalités**  
- 🛡️ **Détection de bords** via des capteurs ToF.  
- 🔄 **Communication entre robots** pour synchronisation.  
- 🎯 **Algorithme de poursuite et d'évitement.**  
- ⚙️ **Gestion des moteurs** à l'aide de signaux PWM.  

---

## 🔧 **Matériel utilisé**  
- **STM32G431RB** : Microcontrôleur principal.  
- **Capteurs ToF** : Détection des bordures.  
- **LiDAR** : Cartographie et détection d'autres robots.  
- **Accéléromètre (ADXL343)** : Détection des chocs pour changer de rôle (Chat/Souris).  
- **Environnement de développement :** STM32CubeIDE.  

---

## 📐 **Architecture**  
### **Schéma architectural**  
![image](https://github.com/user-attachments/assets/0f7c4c1b-3890-4360-bbe3-213a3acfd5ad)
## Explication du fonctionnement du système

1. **⚡ Alimentation principale**
   - La **batterie NiMH 7.2V 1.3Ah** alimente l'ensemble du système. Elle est connectée à des régulateurs de tension pour fournir les différentes tensions nécessaires aux composants :
     - **MP1475S** : Régulateur 5V pour alimenter les moteurs et certains capteurs.
     - **BU33SD5WG-TR** : Régulateur 3.3V pour alimenter le microcontrôleur STM32G431, l'accéléromètre et le LiDAR.

2. **🖥️ Microcontrôleur principal - STM32G431**
   - Le **STM32G431** gère toute la logique du robot et communique avec les différents composants via des bus tels que SPI, UART, I2C et PWM.
   - Il est relié à un **quartz 16 MHz** pour la gestion de l'horloge système et dispose d'un connecteur **SWD** pour la programmation et le débogage.

3. **🔍 Capteurs**
   - **Capteurs ToF** : Utilisés pour détecter les bords ou les chutes du robot.
   - **ADXL343 (Accéléromètre)** : Détecte les impacts ou les tapotements, utilisé pour détecter les collisions avec d'autres robots, et connecté au bus **SPI**.
   - **LiDAR YDLIDAR X4** : Permet de détecter les obstacles à l'aide de la communication **UART** pour transmettre les données de distance et d'angle.

4. **⚙️ Contrôle des moteurs**
   - **ZXBM5210-SP-13 (Driver de moteur)** : Utilisé pour contrôler la direction et la vitesse des moteurs à l'aide de signaux **PWM**. Le microcontrôleur STM32 contrôle les moteurs via le driver pour ajuster la vitesse et la direction du robot.

5. **🌟 Indicateurs d'état (LEDs)**
   - Les **LEDs** servent d'indicateurs pour visualiser l'état du robot, par exemple, lorsqu'il est en marche ou lorsqu'il détecte un obstacle.

### Détails des principaux outils utilisés

1. **Communication avec l'accéléromètre (ADXL343)**
   - Utilisation du protocole **SPI** pour lire les données des axes X, Y et Z.
   - Détection des chocs (taps) causés par des collisions avec d'autres robots.

2. **Contrôle des moteurs avec le driver ZXBM5210**
   - Génération de signaux **PWM** pour réguler la vitesse des moteurs.
   - Implémentation des mouvements du robot : avancer, reculer, tourner à gauche ou à droite.

3. **Gestion des capteurs de bordure/détection de chute**
   - Lecture des entrées des capteurs pour détecter les bords de la table.
   - Réaction immédiate pour stopper ou changer de direction afin d’éviter une chute.

4. **Interfaçage avec le LiDAR YDLIDAR X4**
   - Communication via **UART** pour lire les données du LiDAR.
   - Extraction des valeurs d’angles et de distances pour cartographier l'environnement et éviter les obstacles.

---

## 🚀 **Réalisation matérielle**  
La partie matérielle a été conçue avec **KiCad 8.0** et comprend :  
- 📜 **Schéma électronique**  
- 🧩 **PCB routé**  
- 🛠️ **BOM (Bill of Materials)**  
- 🗂️ **Fichiers GERBER** pour fabrication chez **JLCPCB**.  

### 🔑 [Accédez aux fichiers hardware ici.](./hardware/)

---

## 🛠️ **Développement logiciel**  
Le logiciel embarqué a été développé avec **STM32CubeIDE** et inclut :  
- 🔧 **Configuration des périphériques** (SPI, UART, PWM, etc.).  
- 📦 **Modules logiciels** pour capteurs et moteurs.  
- 🤖 **Algorithmes embarqués** pour les comportements du robot (poursuite, évitement, etc.).  

### 📂 [Accédez au code source ici.](./Software)

---

## 👥 **Auteurs**  
- **Vincent LAKHMECHE**  
- **Mohamed Rafik CHAIR**  
- **Meziane Ameur**  
- **Karim JERJOUB**

---

## 📄 **Licence**  
📝 Ce projet est sous licence **MIT**.  
Pour plus d’informations, consultez le fichier [LICENSE](./LICENSE).  


# 🚀 Explication sur l'utilisation de l'accéléromètre ADXL343 dans le robot chat


---

## 🎯 Objectifs principaux
- ✨ **Détecter les tapotements** (Single Tap/Double Tap) pour **changer de rôle** entre le robot "chat" et le robot "souris".
- 📊 Lire les valeurs d'accélération sur les axes **X, Y, Z** *(optionnel)*.

---

## ⚙️ Configuration
- **Plage d'accélération :** ±2 g *(précision pour petits mouvements)*.
- **Registres utilisés :**  
  - 🛠️ `POWER_CTL` : Activer le mode mesure.  
  - 🎚️ `THRESH_TAP`, `DUR`, `INT_ENABLE` : Configurer les seuils et activer les interruptions pour les tapotements.  
  - 🖥️ `INT_SOURCE` : Vérifier les événements de tapotements.

---

## 🔧 Fonctionnalités implémentées
### ✅ Détection des tapotements
- 🔄 Vérifie les interruptions via le registre `INT_SOURCE`.
- 📍 Met à jour la variable globale `tap_detected` pour signaler un événement.

### ✅ Lecture des accélérations
- 📥 Récupération des valeurs brutes : `acc_rawX`, `acc_rawY`, `acc_rawZ`.
- 🔄 Conversion des valeurs en unités normalisées (*g*) via un facteur de calibration.

---

## 🔁 Réactions aux tapotements
- ⚡ **Interruptions activées** pour permettre une réaction en temps réel.
- 🔀 Chaque tapotement entraîne un **changement de comportement des moteurs** (ex. direction ou rôle).

---

## 🕒 FreeRTOS Task
- Création d'une tâche principale **`vTaskADX`** pour :  
  1️⃣ Lire les données d'accélération brutes et normalisées.  
  2️⃣ Vérifier les événements de tapotement.  
  3️⃣ Ajouter un délai pour réduire la charge CPU (`vTaskDelay`).

---

# Contrôle des Moteurs et Encodeurs

## 🛠️ Composants utilisés

Liste des composants utilisés pour le projet robot avec leurs spécifications et leurs rôles dans le système.

## 🚗 Moteurs

Le contrôle des moteurs est effectué à l'aide d'un driver. Ce driver contient plusieurs fonctions pour contrôler la direction et la vitesse des moteurs du robot.

### Fonctionnalités du driver :

- **`void init_motors(void);`** : Initialisation des moteurs.
- **`void forward_r(uint16_t alpha);`** : Faire avancer le moteur droit.
- **`void forward_l(uint16_t alpha);`** : Faire avancer le moteur gauche.
- **`void reverse_r(uint16_t alpha);`** : Faire reculer le moteur droit.
- **`void reverse_l(uint16_t alpha);`** : Faire reculer le moteur gauche.
- **`void stop_r(void);`** : Arrêter le moteur droit.
- **`void stop_l(void);`** : Arrêter le moteur gauche.

### 🔧 Considérations mécaniques

Le robot utilise deux moteurs, un pour chaque roue :

- La roue droite est en **rouge** et la roue gauche est en **bleu**.
- Les roues tournent dans des directions opposées pour permettre au robot de se déplacer en avant ou en arrière.

### 🔄 Sens de marche

| Moteur Gauche  | Sens de marche Robot | Moteur Droit |
| -------------- | -------------------- | ------------ |
| Sens **reverse** | Sens **forward** | Sens **forward** |

### 🚀 Fonctionnement des moteurs

#### 1. **Moteurs en marche forward**

Pour que le robot se déplace en avant, les moteurs doivent tourner dans des directions opposées. Le code suivant configure le moteur droit et le moteur gauche pour aller en avant.

```c
// Fonction pour faire avancer le moteur droit
void forward_r(uint16_t alpha) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, alpha);  // TIM1_CH1
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   // Démarre la PWM pour le moteur droit
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1); // Arrête le canal complémentaire
}

// Fonction pour faire avancer le moteur gauche
void forward_l(uint16_t alpha) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, alpha);  // TIM1_CH2
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);          // Démarre la PWM pour le moteur gauche
}
```
2. Moteurs en marche arrière (Reverse)
Pour que le robot se déplace en arrière, les directions des moteurs doivent être inversées :

```c
// Fonction pour faire reculer le moteur droit
void reverse_r(uint16_t alpha) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, alpha);  // Inverser le sens pour moteur droit
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);   // Arrêter le moteur droit
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); // Démarrer le moteur droit en reverse
}

// Fonction pour faire reculer le moteur gauche
void reverse_l(uint16_t alpha) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, alpha);  // Inverser le sens pour moteur gauche
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   // Démarrer le moteur gauche en reverse
}
```
3. Arrêt des moteurs
Les fonctions suivantes permettent d'arrêter les moteurs :

```c
// Fonction pour arrêter le moteur droit
void stop_r(void) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);  // Arrêter la PWM pour moteur droit
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);          // Arrêter le moteur droit
}
```
🔧 Encodeur
Les encodeurs sont utilisés pour mesurer la position des moteurs et calculer leur vitesse.

Fonctions d'encodeur
1. Obtenir la position de l'encodeur
```c
// Fonction pour obtenir la position de l'encodeur
int16_t get_encoder_position(uint8_t motor) {
    int16_t position = 0;

    if (motor == MOTOR_LEFT) {
        position = __HAL_TIM_GET_COUNTER(&htim4);  // Lire le compteur du moteur gauche
    } else if (motor == MOTOR_RIGHT) {
        position = __HAL_TIM_GET_COUNTER(&htim3);  // Lire le compteur du moteur droit
    }

    return position;
}
```
2. Réinitialiser la position de l'encodeur
```c
// Fonction pour réinitialiser la position de l'encodeur
void reset_encoder(uint8_t motor) {
    if (motor == MOTOR_LEFT) {
        __HAL_TIM_SET_COUNTER(&htim4, 0);  // Réinitialiser le compteur du moteur gauche
    } else if (motor == MOTOR_RIGHT) {
        __HAL_TIM_SET_COUNTER(&htim3, 0);  // Réinitialiser le compteur du moteur droit
    }
}
```
3. Calculer la vitesse des moteurs
```c
// Fonction pour calculer la vitesse à partir de l'encodeur
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

    // Calcul de la vitesse en tours par seconde
    float speed = (float)delta_position / encoder_resolution; // Tours par intervalle
    speed *= (1000.0f / delta_time_ms); // Convertir en tours par seconde

    return speed;
}
```












