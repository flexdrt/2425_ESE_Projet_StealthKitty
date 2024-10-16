# StealthKitty

Ceci est un projet de dernière année de la meilleure filière de l'ENSEA : électronique et systèmes embarqués

Les contributeurs de ce projet sont :

- Vincent LAKHMECHE
- Mohamed Rafik CHAIR
- Meziane Ameur
- Karim JERJOUB









## Hardware

STM32G431CBU6 

symbole utilisé STM32G431CBUx ds kikad

##### 1/10/24-Suite à la revue de schématique avec le professeur les tâches à faire sont les suivantes : 

- [ ] Affecter des pins pour les interruptions de l'ioc
- [x] Capa de découplage car 3 VDD 
- [x] connecter en // les capa de découplage de l'adx
- [x] Supprimé certains signaux i2c présents en double : un seul pin I2C GPIO doit être utilisé et pas 2 
- [x] Changer les labels des broches utilisant le SPI en MISO, MOSI, etc
- [x] Pour le bus I2C placer la résistance de pull-up à 3.3 kOhms 
- [ ] Mettre des leds pour 
  - [ ]  Power BU33SD5WG-TR
- [x] ******IOC FInir affectation des GPIO sur les pins de la STM ! ******
- [x] ST LINK: choisir la dernière empreinte du symbol placé grâce au prof
- [x] Mettre une capa en // sur NRST
- [x] COnnecter VDDA à 3.3VA : finalement pas besoin
- [x] Découpler VDDA à l'aide de la datasheet de 

​	![image-20241002105134742](/home/vincent/snap/typora/90/.config/Typora/typora-user-images/image-20241002105134742.png)

	Electrical characteristics STM32G431x6 STM32G431x8 STM32G431xB
68/198 DS12589 Rev 6
5.1.6 Power supply scheme
Figure 16. Power supply scheme
Caution: Each power supply pair (V DD /V SS, V DDA /VSSA etc.) must be decoupled with filtering ceramic
capacitors as shown above. These capacitors must be placed as close as possible to, or
below, the appropriate pins on the underside of the PCB to ensure the good functionality of
the device

- [x] Connecter Vref+ au 3.3V et le découpler 
- [x] Découpler NRST 
  ![image-20241002105813090](/home/vincent/snap/typora/90/.config/Typora/typora-user-images/image-20241002105813090.png)
- [x] Supprimer les fils autour de l'inductance 5V/3A
- [ ] labels hiérarchiques si temps dispo 



##### 3/10/24 - post deuxième séance après la revue (celle du mercredi)

- [x] Ré Affectation des pins pour les interruptions de l'ioc (commencé 30 dernières minutes à la fin de la séance du mercredi 2 octobre).
- [x] ajout de points de test pour les interruptions de l'adx et du bus SPI.

​	![Sans titre](/home/vincent/Documents/ese_3a/projet/StealthKitty/annexes/Sans titre.jpeg)

Pour assigner les pins sur les conseils de Mr.Fiack, l'ordre de "priorité d'assignation" est le suivant :

- RCC Osc

- Debug

- Timers (proches des uns des autres au même endroit)

- SPI

- TX - RX

- GPIOs

  

  

###### 									Tableau récapitulatif de l'assignation des pins refaite le 03/10/24

|         |                      |                      |                      |          |
| ------- | -------------------- | -------------------- | -------------------- | -------- |
| 2/10/24 | LABEL/Signal         | GPIO                 | PIN STM32            | 03/10/24 |
| v       | SCLk_adx renommé SCK | SPI_1_SCLk           | PA5                  | v        |
| v       | SDO_adx              | SPI_1_MISO           | PA6                  | v        |
| v       | SDIO_adx             | SPI_1_MOSI           | PA7                  | v        |
| v       | nCS                  | GPIO_Output          | PC4                  | v        |
| v       | SDA-TOF              | I2C1-SDA             | PB9                  | v        |
| v       | SCL-TOF              | I2C1_SCL             | PB8                  | v        |
| v       | LIDAR UART_RX        | UART4_RX             | PC11                 | v        |
| v       | LIDAR UART_TX        | UART4_TX             | PC10                 | v        |
| v       | LIDAR DEV_EN         | GPIO_Output          | PB11                 | v        |
| v       | LIDAR M_EN           | GPIO_Output          | PB10                 | v        |
| v       | LIDAR M_SCTR         | TIM1_CH3             | PA10                 | v        |
|         |                      |                      |                      |          |
| v       | TX_link (stlink)     | USART2_TX            | PA2                  | v        |
| v       | RX_link(stlink)      | USART2_RX            | PA3                  | v        |
| v       | PWM_MOT1_CH1         | TIM1_CH1             | PA8                  | v        |
| v       | PWM_MOT1_CH2         | TIM1_CH1n            | PA11                 | v        |
| v       | PWM_MOT2_CH1         | TIM1_CH2             | PA9                  | v        |
| v       | PWM_MOT2_CH2         | TIM1_CH2n            | PA12                 | v        |
| v       | Codeur1_PH1          | TIM3_CH1             | PA0 remplacé par PB4 | v        |
| v       | Codeur1_PH2          | TIM3_CH2             | PA1 remplacé par PB5 | v        |
| v       | Codeur2_PH1          | TIM4_CH1             | PA2 remplacé par PB6 | v        |
| v       | Codeur2_PH2          | TIM4_CH2             | PA3 remplacé par PB7 | v        |
| v       | INT_TOF1             | TIM4_CH2 GPIO_EXTI13 | PC13                 | v        |
| v       | INT_TOF2             | TIM4_CH3 GPIO_EXTI14 | PC14                 | v        |
| v       | INT1_ADX             | GPIO_EXTI0           | PA0                  | v        |
| v       | INT2_ADX             | GPIO_EXTI1           | PA1                  | v        |
| v       | Bouton 1             | GPIO_EXTI2           | PB2                  | v        |
| v       | status orangeq       | GPIO_Output          | PB14                 | v        |
| v       | status purple        | GPIO_Output          | PB0                  | v        |

##### 05/10/24 à 06/10/24 -Placement 

Nous avons entamés la partie routage sur PCB editor, et nous avons effectués le placement des composant

##### 07/10/24 -revue du placement 

Nous avons revu et optimisé le placement des composants avec l'équipe en minimisant les longueurs de traces et en regroupant les éléments par fonction pour améliorer l'efficacité et la fiabilité du circuit.

08/10 

- [x] ajout de vias pour connecter au gnd les drivers avec pads afin de mieux dissiper leurs chaleurs
- [x] mettre des connecteurs tof coudés en dehors de la carte si besoins
- [x] mettre les capacités au plus proche des broches d'alimentation :
- -  vdd mettre proche des pins  25 35 48 de la stm
- - adc broche 21 pareil 
- - empreintes de pas prendre Handsolder 

- [x] R502 et autres res à mettre en 0402
- [x] changer l'empreinte de l'interrupteur
- [ ] inverser les pins de l'int adx et celui du bouton
- [ ] zones de cuivre pour 5 v tof sur la couche in2
- [ ] zone de cuivre 
- [x] pointe de test inutile si présente sur le connecteur tof (int tof)
- [ ] in1 masse
- un plan vbat dans le plan niveau de priorité = îlot dans l'intérieur
- [ ] couche paste sur kikad
- [ ] signaux qui passe au bottom
- [ ] scxsc
- [ ] faire les zones de cuivres

## Les changements effectués dans les contraintes 10/10/2024:

- •	Largeur min d’anneau on a passé de  0.1  à  0.075
- •	Nombre min de rayon de frein thermique de  2 à 1 
- •	Isolation trou a trou de  0.25 à  0.2
- •	Isolation cuivre/contour de CI  de  0.5  à  0.3
- •	Diamètre min via passe de  0.5  à 0.3
- •	Zone d’isolation couches GND de 0.4 à 0.3 
- •	Classes equipots isolation de  0.2 à 0.15
- •	Zone d’isolation 5V passer de 0.5  à 0.3
- •	Isolation +VBAT de 0.5 à 0.15

## Remarque routage Mr Fiack après revue routage le 16/10/2024

- •	On fait passer 3A donc on doit mettre une piste plus grande  OK : Après utilisation outil calcul de KiCad il nous fait 1.4mm, on a fait une zone de plan (voir photo calcul sur le git)
- •	A vérifier avec  le convertisseur si la bonne empreinte que MP14 ..s  OK
- •	Entre via et via min 0.2mm pour respecter les via  OK
- •	Ajouter les sommets pour ne pas avoir des angles droits  OK
- •	Le faire sur l’autre plan de moteurs OK
- •	Les vias les mettre a  0.45/ 0.3 OK
- •	Changer empreinte  des capacités 4.7uF, 1uF en 0.6 OK
- •	Piste de la batterie a 1.7mm car tout le courant de la carte passe par là (1.7mm car c’est la taille de la pastille) OK
- •	Si on a des comportements d’antennes on mit un via sinon on supprime  une zone 
- •	Chaque fois on a un coin on utilise un via  OK






## Software
