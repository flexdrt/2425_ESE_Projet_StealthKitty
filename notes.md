1. |      | ~~Mardi 5/11~~ | ~~Mercredi 6/11~~ |
   | ---- | -------------- | ----------------- |
   |      |                | Mercredi 13/11    |
   |      | Mardi 19/11    | Mercredi 20/11    |
   |      | Mardi 26/11    | Mercredi 27/11    |
   |      |                | Mercredi 4/12     |
   |      |                | Mercredi 11/12    |
   |      |                | Mercredi 18/12    |
   |      |                |                   |
   |      |                |                   |

   |                                                              |
   | ------------------------------------------------------------ |
   | PWM<br />   Génération de signaux PWM pour contrôler la vitesse des moteurs. Implémentation de la logique de déplacement du robot (en avant, en arrière, tourner à gauche/droite). |
   | Lidar<br />    Communication UART pour lire les données du LiDAR. Traitement des données pour la détection d'obstacles (vous devrez probablement utiliser des bibliothèques spécifiques pour le YDLIDAR X4). |
   | TOF <br />Lire les entrées des capteurs de bordure et réagir pour arrêter ou changer la direction du robot en cas de détection d'obstacle ou de chute. |
   | Accéléromètre<br />Code I2C pour lire les données de l'accéléromètre (accélérations sur les axes X, Y, Z). Implémenter une logique de détection de chute basée sur ces données. |
   | Code comportement déplacement                                |
   |                                                              |

   

   |         |               |      |
   | ------- | ------------- | ---- |
   | Vincent | PWM           |      |
   | Karim   | LIDAR         |      |
   | Meziane | Accéléromètre |      |
   | Rafik   | TOF           |      |
   |         |               |      |
   |         |               |      |

   

   

   

   

   ~~**Initialisation du microcontrôleur (STM32)** :~~

   - ~~Configurer les horloges du système.~~
   - ~~Initialiser les périphériques comme les GPIO (pour les LEDs et les capteurs), l'I2C (pour l'accéléromètre), le PWM (pour les moteurs), et l'UART (pour le LiDAR).~~
   - ~~Utiliser l'environnement STM32CubeMX pour générer la base du code~~.

2. ~~**Gestion des régulateurs de tension** :~~

   - ~~Assurez-vous que les régulateurs sont actifs et fournissent les bonnes tensions avant de démarrer les autres composants. Il est possible d'ajouter une vérification de l'alimentation.~~

   

3. **Communication avec l'accéléromètre (ADXL343)** :

   - Code I2C pour lire les données de l'accéléromètre (accélérations sur les axes X, Y, Z).
   - Implémenter une logique de détection de chute basée sur ces données.

4. **Contrôle des moteurs avec le driver ZXBM5210** :

   - Génération de signaux PWM pour contrôler la vitesse des moteurs.
   - Implémentation de la logique de déplacement du robot (en avant, en arrière, tourner à gauche/droite).

5. **Gestion des capteurs de bordure/détection de chute** :

   - Lire les entrées des capteurs de bordure et réagir pour arrêter ou changer la direction du robot en cas de détection d'obstacle ou de chute.

6. **Interfaçage avec le LiDAR YDLIDAR X4** :

   - Communication UART pour lire les données du LiDAR.
   - Traitement des données pour la détection d'obstacles (vous devrez probablement utiliser des bibliothèques spécifiques pour le YDLIDAR X4).

7. **Gestion des LEDs** :

   - Programmer les LEDs pour indiquer différents états du robot (par exemple : vert pour normal, rouge pour erreur, clignotement pour détection d'obstacle).

8. **Fonction de sécurité et gestion d'énergie** :

   - Implémenter des routines pour surveiller les niveaux d'énergie et gérer le passage à des modes de basse consommation si nécessaire.

Codage comportementale et pilotage:





