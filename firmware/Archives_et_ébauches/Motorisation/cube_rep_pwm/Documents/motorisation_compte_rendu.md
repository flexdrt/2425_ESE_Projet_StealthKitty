Phénomène Observé dans le Comportement du Robot

Lors de l'appel de la fonction Bot_init avec deux modes différents (MOTION_FWD ou MOTION_BACKWD), le robot présente un comportement inattendu, malgré un rapport cyclique de 50 % dans les deux cas, comme défini dans l'appel dans main.c :

    Mode Avant (Forward) :
        La roue droite tourne correctement.
        La roue gauche ne démarre pas automatiquement. Il est impossible de la lancer manuellement, et elle reste immobile jusqu'à ce qu'un changement de condition permette son mouvement.

    Mode Arrière (Backward) :
        La roue gauche tourne très rapidement, bien plus vite que la roue droite.
        La roue droite ne démarre pas automatiquement et il est impossible de la lancer manuellement.