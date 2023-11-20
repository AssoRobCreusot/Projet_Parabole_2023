# Détail de l'expérience
Cette manipulation  est répartie en plusieurs sous-systèmes, la structure principale contenant la partie mécanique (le moteur et le piston), 
le boîtier électrique (contenant l’alimentation, le driver et l’Arduino) et finalement l’ordinateur de contrôle.

Le moteur est commandé par le biais d’impulsions électriques, la valeur permettant de commander le moteur est la valeur entre chaque impulsion, 
qui se trouve être en millisecondes. Par conséquent, plus la valeur de commande est petite plus le moteur va tourner rapidement, jusqu’à 
ce qu’il atteigne sa puissance maximum. 
En 1G nous avons déterminé 110 bpm avec une valeur de 135ms.

Les valeurs enregistrées dans le cadre du masscoeur, sont la consommation, 
la réussite de la compression et la vidéo du système en fonctionnement. 
La consommation du système est mesurée sur la prise du système, 
la réussite de la compression est déterminée par le signal sonore du mannequin.


# Détail du code
L'application masscoeur envoi sur le port Série les commandes suivantes : 

Lors de l'appui sur le bouton : 
<vitesse départ>;<vitesse finale>;<temps secondes>;
exemple : 
sts1000;spd100;tm10s;

L'arduino doit répondre : 
"start" si elle à bien reçu la commande
sinon l'arduino renvoi autre chose


# Résultats de l'expérience
Nous avons observé deux différences entre le fonctionnement 1G et le fonctionnement 0G.  
La première est une baisse très légère du nombre de compression par minute d’environ 5 compressions par minutes, 
cela peut être dû aux imprécisions dû fait que l’on compte le nombre de compression par le biais des vidéos, 
mais aussi car le temps de parabole ne permet pas de prendre un grand temps de mesure.  

La seconde différence, est une différence de consommation. Nous avons observé une différence de consommation de 50 à 90mA 
(en 230v) entre la phase d’apesanteur et de gravité normale. Pour une valeur de commande de 120ms (120bpm en gravité normale) 
avec un démarrage progressif, nous avons observé 630mA en gravité normale et 560mA en apesanteur à 230V. 
Avec une vitesse plus élevée on observe une différence plus grande. 
Cette  différence peut être dû à la répartition du poids dans le système de compression, 
le poids du moteur et de la structure n’est pas centré avec la force du piston, 
donc il y a un effet levier qui se créait et qui est donc sensible à la gravité.
