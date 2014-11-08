Plane-Detection-in-Point-Clouds
===============================
Projet INF552 - Détection de plans dans un nuage de points
===============================


Fonctions à implémenter
===============================
Implémentation d’un octree : partition des nuages pour avoir un faible nombre de plans par bloc

RANSAC : détection d’un seul plan

Définition d’un critère d'ambiguïté sur le nombre de plans

Définition d’un critère de fin de la recherche (pas de plan dans un bloc)

Partition de plans en polygones

Fusion de plans/polygone au-delà des blocs

(Bonus) Détection d’arêtes


Structures de données
===============================
-PointCloud : pointeur point -> plan/polygone

-Octree

-Plan :

    vecteur normal + point OU equation (4 coordonnées) + incertitudes ?

    listes des points issus du RANSAC

-Polygone

    plan d’origine

    listes des points inclus

    enveloppe convexe

    point moyen + rayon pour détection de polygones voisins

-(Bonus) Arête


Idées
===============================
-k means

-détection de normales locales et croissance


Bibliographie
===============================
data set : http://www.pointclouds.org/news/2013/01/07/point-cloud-data-sets/

github : https://github.com/brandonpelfrey/SimpleOctree

http://www.ipb.uni-bonn.de/fileadmin/publication/pdf/Yang2010Plane.pdf

