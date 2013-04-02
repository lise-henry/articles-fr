% Programmation orientée objet en C, partie 1
% Élisabeth Henry

# Introduction #

Le C n'est pas un langage orienté objet, contrairement à C++, Java,
Objective-C, C#, Vala, etcaetera. Cela ne veut pas pour autant dire
qu'il n'est pas possible d'utiliser des concepts issus de la
programmation orientée objet dans ce langage. C'est notamment ce que
permet de faire la bibliothèque GLib (et plus 
spéficiquement GObject), utilisée notamment (mais pas que) par Gtk+ et
Gnome. En effet, cette bibliothèque écrite en C permet
l'utilisation de l'essentiel des concepts de la programmation orientée
objet (classes, héritage, polymorphisme, interfaces, etc.) dans ce langage.

Le but de cette série d'articles n'est pas de montrer comment utiliser
cette bibliothèque (même si je prévois de fournir un ou deux exemples
d'utilisation), mais d'implémenter *from scratch* certains mécanismes
de programmation objet.

On peut légitimement se poser la question de l'intérêt de «réinventer la
roue» et d'implémenter soi-même un certain nombre de concepts liés à
la programmation objet dans un langage qui ne les intègre pas à la
base (en l'occurrence, le C), plutôt que d'utiliser directement un
autre langage ou au moins une bibliothèque orientée objet.

Il y a différentes raisons techniques qui peuvent justifier un tel
choix : dans le domaine de l'embarqué par exemple, il peut être
nécessaire de réduire au minimum vital les bibliothèques et les
environnements *runtime* que l'on utilise. Cela dit, ce que je trouve
peut-être plus intéressant à première vue dans cette démarche, c'est
de devoir «mettre les mains dans le cambouis», et d'être ainsi obligé·e
de se pencher un peu plus sur ce qu'implique concrètement les
différents concepts liés à la programmation objet, ainsi que les
différentes façons dont ils peuvent être implémentés. 

L'objectif de cette série d'articles n'est donc pas de vous convaincre
d'abandonner votre langage orienté objet préféré pour écrire en C,
mais de vous donner (et *me* donner, au passage) une idée de la façon
dont cela peut être implémenté, et peut-être, à travers cela, de mieux
maîtriser certains de ces concepts qui, dans les langages plus hauts
niveaux, peuvent parfois apparaître quelque peu « magiques ».

# Quelques concepts de programmation orientée objet #

Pour résumer de façon brutale, la notion d'« objet » consiste à
associer des données (les attributs) et des procédures manipulant ces
données (les méthodes). La programmation orientée objet est aussi liée
à un certain nombre de fonctionnalités (que l'on ne retrouve pas
forcément toutes dans tous les langages orientés objet) :

* l'encapsulation : l'idée est de « cacher » les structures de données
  internes à un objet, et de ne permettre la manipulation des données
  internes à l'objet que par une interface prédéfinie — typiquement,
  des méthodes getFoo ou setBar. 
* l'héritage : un objet peut hériter d'un autre objet (ou, dans les
  modèles se basant sur les classes, une classe va hériter d'une autre
  classe) ; il est donc possible de réutiliser le code existant.
* le polymorphisme : si on appelle deux objets de deux classes
  différentes avec la même méthode, chaque objet est capable de
  choisir une implémentation différente. 
  
Plus concrètement, dans cette série d'articles, je propose de montrer
comment implémenter en C : 

* des classes et des objets ;
* de l'héritage simple entre classes ;
* des méthodes virtuelles, permettant une certaine forme de
  polymorphisme.
  
# Exemple : un plateau de jeu #

Afin de montrer l'utilisation d'un certain nombre de concepts de la
programmation orientée objet, nous allons prendre l'exemple d'un
plateau de jeu, qui consiste à afficher en ASCII-art un
damier. Ce plateau est caractérisé par sa longueur et sa largeur (ses
attributs). Un certain nombre d'opérations (méthodes) peuvent être
effectuées sur ce plateau :

* `getLongueur` et `getLargeur` permettent d'accéder aux attributs
  `longueur` et `largeur`. Elles ne prennent pas de paramètres et
  renvoient un entier.
* `getCase` permet d'obtenir le caractère représentant la case passée
  en paramètre. On veut en effet qu'une case sur deux soit blanche
  (représentée par un espace) et que l'autre soit noire (représentée
  par un #). Cette méthode prend donc en paramètres deux entiers, x et
  y, et renvoie un caractère.
* `afficher` sert à afficher le damier sur le terminal. Pour cela,
  cette méthode utilise la méthode `getCase`. Elle ne prend pas de
  paramètre et ne renvoie rien (l'affichage se faisant sur la sortie
  standard). 

La classe `Plateau` ne permet pas en elle-même d'ajouter des pions sur le damier,
uniquement d'en créer un vide et de le représenter. Pour cela 
il est nécessaire de créer une *instance* de la classe
`Plateau`, c'est-à-dire un objet. On peut par exemple créer une
instance `plateau1`, de longueur 8 et de largeur 4, et une
instance `plateau2`, de longueur 2 et de largeur 3. Ces deux
objets partageront les mêmes opérations et seront définis par les
mêmes attributs ; en revanche, la valeur de ces attributs est
spécifique à chaque instance. 

Afin de pouvoir ajouter et représenter des pions, on créera une autre 
classe, appelée `PlateauPions`. Cette classe *hérite*
(ou est dérivée) de la classe `Plateau`. Cela veut dire que les
opérations valables pour des instances de la classe `Plateau` sont
toujours valables pour des instances de la
classe `PlateauPions`. Par exemple, il est toujours possible
d'utiliser la méthode `afficher` : l'héritage évite ainsi de
réécrire un certain nombre de lignes de code.

La classe `PlateauPions` ajoute un certain nombre de choses à
la classe `Plateau`, et notamment un tableau à deux dimensions
de booléens, représentant les pions présents sur le damier (vrai s'il y a
un pion, faux s'il n'y en a pas). Elle permet de faire plus d'opérations
:  

* `ajouterPion`, qui prend en paramètre un pion et deux coordonnées x
  et y, et ajoute le pion à cet emplacement ;
* `enleverPion`, qui prend en paramètre deux coordonnées x et y et
  retire le pion — s'il y en a un — présent sur cette case.
  
Cette classe redéfinit également la méthode `getCase`. En effet, on ne
veut plus seulement afficher une case blanche ou noire, mais également
les pions qui sont dessus. Concrètement, cette méthode renvoie un 'O'
si un pion se trouve sur la case, et utilise la méthode définie par la
classe parente sinon. Par conséquent, la méthode `getCase` devra être
une méthode *virtuelle* : on ne veut pas qu'elle effectue le même
traitement selon que l'objet soit un `Plateau` ou un
`PlateauPions`. En particulier, on veut que lorsqu'on appelle la 
méthode `afficher` de la classe `Plateau` sur une 
instance de `PlateauPions`, on utilise bien l'implémentation de la
méthode `getCase` de la classe `PlateauPions` : sinon, on
afficherait toujours un plateau désespérément vide. 

![Représentation UML des deux classes `Plateau` et `PlateauPions`](code/minijeu.png)

Bien entendu, dans la « vraie vie », cette conception n'aurait pas
beaucoup de sens (quel est l'intérêt de ne pas implémenter l'ajout de
pions directement dans la classe `Plateau` ?). Cependant, cet exemple
a l'avantage de rester simple tout en utilisant un certain nombre de
concepts de la programmation orientée objet 
(classes avec des attributs et des méthodes, objets instance d'une
classe, héritage, méthode virtuelle). La question est donc : comment
implémenter cela en C ?

## Implémenter des classes et objets avec attributs ##

### Attributs ###

La première chose à faire est de pouvoir créer une classe, et de
l'utiliser pour en instancier des objets. Si le C ne comporte rien qui
permet de gérer des objets, la structure `struct` permet déjà
d'en gérer les attributs.

En ce qui concerne la classe `Plateau`, on peut déjà commencer à
définir un type `Plateau` à l'aide d'une structure : 

```C
typedef struct {
  int longueur;
  int largeur;
} Plateau;
```

Il devient ainsi possible de créer des « objets » de type `Plateau`, et de
manipuler leurs attributs :

```C
Plateau A;
A.largeur = 6;
A.longueur = 4;
```

### Méthodes ###

Le C ne permet pas, à l'inverse du C++, de rattacher facilement des
méthodes à une classe. Une façon de faire est donc d'utiliser une fonction prenant comme premier
paramètre un pointeur sur l'objet :

~~~~ {#mycode .C}
void plateau_init (Plateau * plateau, int longueur, int largeur)
{
    plateau->longueur = longueur;
    plateau->largeur = largeur;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

En effet, le fait de passer un pointeur sur l'objet permet d'accéder
aux attributs de l'objet, comme le font les méthodes d'une classe. Le
fait qu'il s'agisse d'un pointeur rend possible à cette fonction
d'accéder en lecture comme en écriture à tous les attributs de
l'objet.

Les noms de méthode sont de la forme `classe_méthode` car il est
nécessaire de différencier des méthodes homonymes de classes
différentes. J'ai choisi ici d'utiliser des conventions de
nommage typiques du C, c'est-à-dire avec des
noms séparés par des *underscores*. C'est aussi ce type de conventions
que vous avez déjà pu utiliser si vous vous êtes déjà servi de Gtk+ ou
d'autres bibliothèques C utilisant GObject.

Une fois que l'on a compris comment créer des méthodes, il devient
facile de terminer la classe `Plateau` :

```C
char plateau_get_case (Plateau * plateau, int x, int y)
{
  if ((x + y) % 2 == 0)
    {
      return ' ';
    }
  else
    {
      return '#';
    }
}

void plateau_afficher (Plateau * plateau)
{
  int i;
  int j;

  /* Bordure du haut */
  printf (" ");
  for (j = 0; j < plateau->largeur - 1;j++)
    {
      printf ("__");
    }
  printf ("_\n");

  /* Lignes du damier */
  for (i = 0; i < plateau->longueur; i++)
    {
      printf ("|");
      for (j = 0; j < plateau->largeur; j++)
        {
          printf ("%c|", plateauGetCase (plateau, j, i));
        }
      printf ("\n");
    }

  /* Bordure du bas */
  /* Bordure du haut */
  printf (" ");
  for (j = 0; j < plateau->largeur-1;j++)
    {
      printf ("--");
    }
  printf ("-\n");
}
```

On peut alors très facilement créer et manipuler des instances de la
classe `Plateau`, soit en créant une variable de
type `Plateau` et en passant aux méthodes sa référence: 

```C
  Plateau plateau;
  plateau_init (&plateau, 8, 8);
  plateau_afficher (&plateau);
```

soit en utilisant un pointeur et en allouant (et libérant !) la
mémoire nécessaire : 

```C
  Plateau * plateau = calloc (1, sizeof (Plateau));

  plateau_init (plateau, 8, 8);
  plateau_afficher (plateau);
  free (plateau);
```

Si, dans cet exemple, la structure `Plateau` se limite à deux entiers
(la largeur et la longueur), en général les objets seront plus
volumineux (surtout par la suite) et on utilisera plutôt l'allocation
dynamique. Par conséquent, plutôt que de devoir appeler successivement
`calloc` et `plateau_init`, il est sans doute plus simple de créer une
méthode `plateau_new` :

```C
Plateau * plateau_new (int longueur, int largeur)
{
  Plateau * plateau = calloc (1, sizeof (Plateau));
  plateau_init (plateau, longueur, largeur);
  return plateau;
}
```

Pour instancier et utiliser un objet de la classe `Plateau`,  nous
n'avons donc plus qu'à faire : 

```C
  Plateau * plateau = plateau_new (8, 8);
  plateau_afficher (plateau);
  free (plateau);
```

# Conclusion provisoire #

Il y a certes des redondances dans le code. On est obligé d'écrire :


```C
    classe_methode (objet, parametres);
```

alors que, dans la plupart des langages objets, on pourrait se
contenter de :

```Cpp
    objet.methode (parametres);
```

Malheureusement, il ne sera pas possible d'améliorer cela. Cependant,
dans l'ensemble, on peut trouver que notre morceau de code, à part
cette différence de syntaxe, commence vaguement à ressembler à de
l'objet.

Il n'y a pas de quoi s'emballer pour autant. Pour l'instant, on a
juste rajouté un peu d'*encapsulation* : on n'accède pas directement
aux attributs de l'« objet » (c'est-à-dire, aux valeurs
`plateau->longueur` et `plateau->largeur`), mais on passe par des
méthodes pour manipuler et afficher cet objet. Cette encapsulation
reste très relative : on doit se contenter du fait qu'un utilisateur
ne *devrait pas* manipuler directement la structure `Plateau`, il est
impossible de déclarer des attributs *privés*. Par ailleurs, notre
« modèle objet » n'est capable ni d'*héritage*, ni de
*polymorphisme*. Autant dire que notre « modèle objet » n'en est pour
l'instant pas un.

Dans le prochain article, on verra comment il est possible (et assez
facile) d'implémenter la notion d'*héritage* en C, ce qui nous
permettra de nous rapprocher un peu de notre objectif.

# Annexe : code source complet #

## plateau.h ##

```C
#ifndef _PLATEAU_H_
#define _PLATEAU_H_

/* Structure contenant les différentes propriétés d'un plateau */
typedef struct {
  int longueur;
  int largeur;
} Plateau;

/* Méthodes applicables à un plateau */
void plateau_init (Plateau * plateau, int longueur, int largeur);
Plateau * plateau_new (int longueur, int largeur);
int plateau_get_longeuur (Plateau * plateau);
int plateau_get_largeur (Plateau * plateau);
char plateau_get_case (Plateau * plateau, int x, int y);
void plateau_afficher (Plateau * plateau);

#endif /* _PLATEAU_H_ */
```

## plateau.c ##
```C
#include <stdio.h>
#include <stdlib.h>

#include "plateau.h"

/**
 * Initialise un plateau déjà alloué
 **/
void plateau_init (Plateau * plateau, int longueur, int largeur)
{
  plateau->longueur = longueur;
  plateau->largeur = largeur;
}

/**
 * Alloue la mémoire nécessaire pour stocker un Plateau et l'initialise
 **/
Plateau * plateau_new (int longueur, int largeur)
{
  Plateau * plateau = calloc (1, sizeof (Plateau));
  plateau_init (plateau, longueur, largeur);
  return plateau;
}

/**
 * Renvoie la longueur d'un plateau
 **/
int plateau_get_longueur (Plateau * plateau)
{
  return plateau->longueur;
}

/**
 * Renvoie la largeur d'un plateau
 **/
int plateau_get_largeur (Plateau * plateau)
{
  return plateau->largeur;
}

/**
 * Renvoie le caractère correspondant à la case située à la position
 * (x, y) du plateau
 **/
char plateau_get_case (Plateau * plateau, int x, int y)
{
  if ((x + y) % 2 == 0)
    {
      return ' ';
    }
  else
    {
      return '#';
    }
}

/**
 * Affiche le plateau sur la sortie standard
 **/
void plateau_afficher (Plateau * plateau)
{
  int i;
  int j;

  /* Bordure du haut */
  printf (" ");
  for (j = 0; j < plateau->largeur - 1;j++)
    {
      printf ("__");
    }
  printf ("_\n");

  /* Lignes du damier */
  for (i = 0; i < plateau->longueur; i++)
    {
      printf ("|");
      for (j = 0; j < plateau->largeur; j++)
        {
          printf ("%c|", plateau_get_case (plateau, j, i));
        }
      printf ("\n");
    }

  /* Bordure du bas */
  /* Bordure du haut */
  printf (" ");
  for (j = 0; j < plateau->largeur-1;j++)
    {
      printf ("--");
    }
  printf ("-\n");
}
```

## damier.c ##

```C
#include <stdio.h>
#include <stdlib.h>

#include "plateau.h"

int main (int argc, char **argv)
{
  Plateau * plateau1 = plateau_new (8, 8);
  Plateau * plateau2 = plateau_new (12, 6);

  printf ("Affichage de plateau1 (%d x %d)\n", 
    plateau_get_longueur (plateau1),
    plateau_get_largeur (plateau1));
  plateau_afficher (plateau1);

  printf ("Affichage de plateau2 (%d x %d)\n", 
    plateau_get_longueur (plateau2),
    plateau_get_largeur (plateau2));
  plateau_afficher (plateau2);

  free (plateau1);
  free (plateau2);

  return 0;
}
```
