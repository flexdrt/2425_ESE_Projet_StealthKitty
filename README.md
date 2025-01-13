# StealthKitty - Projet Robot Chat

## Description
StealthKitty est un projet de système embarqué innovant basé sur un STM32. L'objectif est de développer plusieurs robots capables de se déplacer sur une table sans bordure. Un robot est désigné comme "chat" et doit attraper un autre robot, qui devient alors le nouveau "chat". Ceci est un projet de dernière année de la meilleure filière de l'ENSEA : électronique et systèmes embarqués (ESE)

Les contributeurs de ce projet sont :

**- Vincent LAKHMECHE**  
**- Mohamed Rafik CHAIR**  
**- Meziane Ameur**  
**- Karim JERJOUB**

![image](https://github.com/user-attachments/assets/0b8a3520-2e76-4162-91c8-48ae93badee7)

## Table des matières
1. [Contexte](#contexte)  
2. [Fonctionnalités](#fonctionnalités)  
3. [Matériel utilisé](#matériel-utilisé)  
4. [Architecture](#architecture)  
5. [Utilisation](#utilisation)  
6. [Auteurs](#auteurs)  
7. [Licence](#licence)

## Contexte
Ce projet s'inscrit dans le cadre de la formation 3A à l'ENSEA. Il vise à concevoir un système embarqué complet, de la conception électronique à l'implémentation logicielle.

## Fonctionnalités
- Détection de bords via des capteurs ToF.
- Communication entre robots pour synchronisation.
- Algorithme de poursuite et d'évitement.
- Gestion des moteurs via des signaux PWM.

## Matériel utilisé
- **STM32G431RB** : Microcontrôleur principal.
- **Capteurs ToF** : Détection des bordures.
- **LiDAR** : Cartographie de l'environnement et détection des autres robots.
- **Accéléromètre** : Détection des chocs avec les autres robots afin de changer de rôle (Chat/Souris).
- **STM32CubeIDE** : Environnement de développement.

## Architecture
### Schéma architectural
Le schéma suivant illustre l'architecture générale du projet :  

![image](https://github.com/user-attachments/assets/0f7c4c1b-3890-4360-bbe3-213a3acfd5ad)

### Explication du fonctionnement du système

1. **Alimentation principale**
   - La **batterie NiMH 7.2V 1.3Ah** alimente l'ensemble du système. Elle est connectée à des régulateurs de tension pour fournir les différentes tensions nécessaires aux composants du robot.

2. **Régulateurs de tension**
   - **MP1475S** : Régulateur 5V pour alimenter les moteurs et certains capteurs.
   - **BU33SD5WG-TR** : Régulateur 3.3V pour alimenter le microcontrôleur STM32G431, l'accéléromètre et le LiDAR.

3. **Microcontrôleur principal - STM32G431**
   - Le **STM32G431** gère toute la logique du robot et communique avec les différents composants via des bus tels que SPI, UART, I2C et PWM.
   - Il est relié à un **quartz 16 MHz** pour la gestion de l'horloge système et dispose d'un connecteur **SWD** pour la programmation et le débogage.

4. **Capteurs**
   - **Capteurs de bordure** : Utilisés pour détecter les bords ou les chutes du robot.
   - **ADXL343 (Accéléromètre)** : Détecte les impacts ou les tapotements (utilisé pour détecter les collisions avec d'autres robots). Il est connecté au bus **SPI**.
   - **LiDAR YDLIDAR X4** : Permet de détecter les obstacles à l'aide de la communication **UART** pour transmettre les données de distance et d'angle.

5. **Contrôle des moteurs**
   - **ZXBM5210-SP-13 (Driver de moteur)** : Utilisé pour contrôler la direction et la vitesse des moteurs à l'aide de signaux **PWM**. Le microcontrôleur STM32 contrôle les moteurs via le driver pour ajuster la vitesse et la direction du robot.

6. **Indicateurs d'état (LEDs)**
   - Les **LEDs** servent d'indicateurs pour visualiser l'état du robot, par exemple, lorsque le robot est en marche ou lorsqu'il détecte un obstacle.

### Détails des principaux outils utilisés

1. **Communication avec l'accéléromètre (ADXL343)** :
   - Utilisation du protocole SPI pour lire les données des axes X, Y et Z.
   - Détection des chocs (taps) causés par des collisions avec d'autres robots.

2. **Contrôle des moteurs avec le driver ZXBM5210** :
   - Génération de signaux PWM pour réguler la vitesse des moteurs.
   - Implémentation des mouvements du robot : avancer, reculer, tourner à gauche ou à droite.

3. **Gestion des capteurs de bordure/détection de chute** :
   - Lecture des entrées des capteurs pour détecter les bords de la table.
   - Réaction immédiate pour stopper ou changer de direction afin d’éviter une chute.

4. **Interfaçage avec le LiDAR YDLIDAR X4** :
   - Communication via UART pour lire les données du LiDAR.
   - Extraction des valeurs d’angles et de distances pour cartographier l'environnement et éviter les obstacles.

---

## Réalisation matérielle
La partie matérielle a été conçue avec **KiCad 8.0** et comprend :
- **Schéma électronique** : Disponible dans le [dossier hardware](./hardware/).
- **PCB routé** : Les fichiers de routage sont inclus dans le dossier.
- **BOM (Bill of Materials)** : Liste des composants nécessaires.
- **Fichiers GERBER** : Pour la fabrication chez **JLCPCB**.

## Développement logiciel
Le logiciel embarqué a été développé avec **STM32CubeIDE** et comprend :  

- **Configuration des périphériques** : SPI, UART, PWM, et autres paramètres matériels, configurés via le fichier `.ioc`.  
- **Modules logiciels** : Gestion des capteurs, contrôle des moteurs, et communication.  
- **Algorithmes embarqués** : Implémentation des comportements du robot (détection des bords, poursuite, et évitement).  

Tout le code source et les configurations sont centralisés dans le dossier [Software](./Software).


## Utilisation de l'accéléromètre ADXL343 dans le robot chat

### 1. Objectifs principaux
- **Détecter les tapotements** (Single Tap/Double Tap) pour changer le rôle entre le robot "chat" et "souris".
- Lire les valeurs d'accélération sur les axes X, Y, Z (optionnel).

---

### 2. Configuration
- **Plage d'accélération :** ±2 g (meilleure précision pour petits mouvements).
- **Registres principaux :**
  - `POWER_CTL` : Activer le mode mesure.
  - `THRESH_TAP`, `DUR`, `INT_ENABLE` : Configurer et activer les interruptions pour les tapotements.
  - `INT_SOURCE` : Vérifier les événements de tapotements.

---

### 3. Fonctionnalités implémentées
1. **Détection des tapotements :**
   - Vérifie les interruptions pour détecter les tapotements via le registre `INT_SOURCE`.
   - Mise à jour d'une variable globale `tap_detected` pour signaler un événement.

2. **Lecture des accélérations :**
   - Valeurs brutes (`acc_rawX`, `acc_rawY`, `acc_rawZ`) converties en unités normalisées (g) avec un facteur de calibration.

---

### 4. Réactions aux tapotements
- Activation des interruptions pour que le système réagisse en temps réel.
- À chaque tapotement détecté, les moteurs changent de comportement (ex. changement de direction ou de rôle).

---

### 5. FreeRTOS
- Création d'une tâche principale `vTaskADX` pour :
  - Lire les données brutes et normalisées.
  - Vérifier l'état des tapotements.
  - Utiliser un délai pour limiter la charge CPU.

---


























