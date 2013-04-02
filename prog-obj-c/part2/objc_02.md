Programmation orientée objet en C, partie 2
===========================================

# Résumé des épisodes précédents #

Dans l'article précédent, on a commencé à implémenter en C une classe
`Plateau`. Pour l'instant, si cela nous a permis une encapsulation du
code (par exemple, passer par `plateau_get_longueur` plutôt que
d'utiliser `plateau->longueur`) très limitée, on n'a pas abordé ni la
notion d'*héritage*, ni celle de *polymorphisme*.

Par ailleurs, si notre classe `Plateau` permet d'afficher un plateau
de dimension (longueur, largeur), il n'est pas possible d'ajouter de
pions dessus. 

Dans cet article, on va commencer à voir comment implémenter la classe
`PlateauPions`, qui permettra d'ajouter et d'afficher des pions. Pour
cela, nous allons avoir besoin d'*héritage*.

# Héritage #

Un intérêt important de la programmation de la programmation orientée
objet est de permettre l'héritage ; autrement dit, ce qui marche pour
un objet d'une classe marche toujours pour les objets de la classe
dérivée. Cela évite ainsi d'avoir à dupliquer le code. 

En l'occurrence, on veut créer la nouvelle classe `PlateauPions`, qui
héritera de la classe `Plateau`, avec de nouveaux
attributs  – un tableau à deux dimensions de booléens – et de nouvelles
méthodes – `ajouterPion` et `enleverPion` (ou, plus exacetement,
`plateau_pions_ajouter_pion` et `plateau_pions_enlever_pion`, ce qui
est tout de suite moins succint). Elle rédéfinit également une méthode de la classe parente, `get_case`.

Dans un langage orienté objet, nous nous attendons évidemment à ce que
les méthodes déjà utilisées par la classe `Plateau` soient toujours
applicables à `PlateauPions`. Mais qu'en est-il en C ?

En C, il n'y a pas d'héritage en tant que tel, mais nous pouvons
toujours profiter du type *struct*. Pour cela, il est
nécessaire de comprendre à quoi correspond exactement une structure.

Concrètement, une structure sert à regrouper un certain nombre de
variables à la suite. Ainsi, lorsque l'on déclare : 

~~~~ {#mycode .C}
Plateau p;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

le compilateur va traduire cela par «allouer de la mémoire pour deux
entiers (la longueur, et la largeur). Ces deux entiers seront placés
en mémoire à la suite des autres. Pour le voir plus clairement,
affichons l'emplacement mémoire de `p`, `p.longueur` et `p.largeur` :

~~~~ {#mycode .C}
  Plateau p;
  printf ("Emplacement mémoire de...\n"
    "p: %x\n"
    "p.longueur: %x\n"
    "p.largeur: %x\n",
    &p, &p.longueur, &p.largeur);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le résultat obtenu ressemblera à :

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Emplacement mémoire de...
p: ffc6d2e8
p.longueur: ffc6d2e8
p.largeur: ffc6d2ec
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Si on regarde plus attentivement, on peut observer que, comme prévu,
la largeur est stockée «un peu après» la longueur. Surtout, ce qui est
plus intéressant, on constate que l'adresse de `p.longueur` est
exactement la même que celle de `p`. C'est quelque chose qui est tout
le temps vrai : l'adresse du premier élément d'une structure est la
même que celle de la structure. 

C'est cette propriété qui va permettre de réaliser l'héritage en C. En
effet, déclarons la structure `PlateauPions` :

~~~~ {#mycode .C}
/* Structure PlateauPions, héritant de Plateau */
typedef struct {
  Plateau parent;
  char **pions;
} PlateauPions;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Le fait de commencer par inclure une structure de type `Plateau`,
comme premier champ de `PlateauPions`, fait que, d'une certaine
manière, `PlateauPions` «hérite» de `Plateau`.

En effet, comme dans toutes les méthodes que l'on a définies
précédemment, on prend comme premier paramètre un pointeur de type
`Plateau`, il suffit de *caster* un pointeur de type `PlateauPions`
pour que ces méthodes demeurent opérationnelles. Comme l'adresse
mémoire d'une variable de type `PlateauPions` est la même que celle du
premier champ `parent`, les méthodes en question pourront donc accéder
aux valeurs `longueur` et `largeur`.

Pour voir cela en pratique, commençons à définir les méthodes pour
initialiser un objet de la classe `PlateauPions` :

~~~~ {#mycode .C}
PlateauPions * plateau_pions_new (int longueur, int largeur)
{
  PlateauPions * self = calloc (1, sizeof (PlateauPions));
  plateau_pions_init (self, longueur, largeur);
  return self;
}

void plateau_pions_init (PlateauPions * self, int longueur, int largeur)
{
  plateau_init ((Plateau *) self, longueur, largeur);
  self->pions = NULL;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Pour la méthode `plateau_pions_init`, on fait déjà appel à l'héritage
: en effet, celle-ci commence par appeller une méthode de la classe
parente, `plateau_init`, pour assigner correctement les attributs
`longueur` et `largeur`. Pour cela, la seule contrainte est d'utiliser
un *cast*. 

Comme pour la classe `Plateau`, nous avons utilisé deux méthodes
différentes pour initialiser et allouer la mémoire d'une instance de
`PlateauPions`. La raison pour laquelle cela est nécessaire est
apparente lorsqu'on examine le code de `plateau_pions_init` : en
effet, celle-ci utilise la méthode `plateau_init` pour commencer son
initialisation, alors que la mémoire a déjà été allouée. Cela ne
serait pas possible si cette initialisation se faisait dans la méthode
`plateau_new`.

Maintenant que nous savons comment obtenir un objet de type
`PlateauPions`, essayons de voir s'il est toujours possible d'utiliser
les méthodes de la classe `Plateau` :

~~~~ {#mycode .C}
  PlateauPions * p_p = plateau_pions_new (6, 4);
  
  printf ("Affichage de p_p (%d x %d)\n", 
    plateau_get_longueur ((Plateau *) p_p),
    plateau_get_largeur ((Plateau *) p_p));
  plateau_afficher ((Plateau *) p_p);

  free (p_p);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Après compilation et exécution, on obtient le résultat suivant : 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Affichage de p_p (6 x 4)
 _______
| |#| |#|
|#| |#| |
| |#| |#|
|#| |#| |
| |#| |#|
|#| |#| |
 -------
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

L'héritage fonctionne donc bel et bien : les méthodes `get_longueur`,
`get_largeur` et `afficher` de la classe `Plateau` ont pu être
utilisées sur une instance de la classe `PlateauPions`.

Cela dit, cette implémentation n'est pas idéale : en effet il est
nécessaire de *caster* explicitement un objet vers sa classe
parente. Si cela a l'inconvénient de demander à taper plus de code, le
problème principal est surtout qu'on perd les informations sur le type
de la variable. Cela est problématique, car à aucun moment on ne
vérifie que la variable `p_p` est effectivement une instance d'une
classe dérivée de la classe `Plateau` : ni au moment de la
compilation, ni au moment de l'exécution.  Cela est très gênant car
cela peut entraîner un comportement complétement imprévisible de notre
programme si on essaie d'appeler `plateau_get_longueur ((Plateau *) x`
sur un objet `x` qui n'est *pas* dérivé de la classe
`Plateau` (dans la meilleur des cas, cela correspondra une erreur de
segmentation ; dans le pire des case, un comportement complétement
inattendu, rendant la source du problème difficile à détecter). Nous
verrons plus tard comment il est possible de pallier à ce problème.  

En attendant, nous pouvons continuer à implémenter notre classe
`PlateauPions`, en commençant par terminer la méthode
`plateau_pions_init` (qui pour l'instant n'alloue pas de mémoire pour
le tableau de booléens — ou en l'occurence de caractères, le C n'ayant
de base pas de booléens), puis en ajoutant les méthodes
`ajouter_pion`, `retirer_pion` et `get_case`.

~~~~ {#mycode .C}
void plateau_pions_init (PlateauPions * self, int longueur, int largeur)
{
  int i;

  plateau_init ((Plateau *) self, longueur, largeur);

  self->pions = calloc (largeur, sizeof (char *));
  for (i = 0; i < largeur; i++)
    {
      self->pions[i] = calloc (longueur, sizeof (char));
    }
    
  self->pions = NULL;
}

void plateau_pions_ajouter_pion (PlateauPions * self, int x, int y)
{
  self->pions[x][y] = 1;
}

void plateau_pions_retirer_pion (PlateauPions * self, int x, int y)
{
  self->pions[x][y] = 0;
}

char plateau_pions_get_case (PlateauPions * self, int x, int y)
{
  if (self->pions[x][y])
    {
      return 'O';
    }
  else
    {
      return plateau_get_case ((Plateau *) self, x, y);
    }
}

void plateau_pions_free (PlateauPions *self)
{
  int largeur = plateau_get_largeur ((Plateau *) self);
  int i;

  for (i = 0; i < largeur; i++)
    {
      free (self->pions[i]);
    }
  free (self->pions);
  free (self);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Comme on alloue maintenant de la mémoire dans l'initialisation, il
faut aussi penser à la supprimer, d'où la création de la méthode
`plateau_pions_free`, qui libère la mémoire de `self->pions` avant de
libérer celle de `self` (on verra plus tard pourquoi cette approche
n'est pas idéale, mais elle fera l'affaire pour l'instant).

On peut maintenant vérifier que notre classe `PlateauPions` fonctionne
correctement : 

~~~~ {#mycode .C}
  PlateauPions * p_p = plateau_pions_new (9, 8);

  plateau_pions_ajouter_pion (p_p, 7, 4);
  plateau_pions_ajouter_pion (p_p, 7, 5);

  printf ("Contenu de quelques cases...\n"
    "(7, 3) -> %c\n"
    "(7, 4) -> %c\n"
    "(7, 5) -> %c\n"
    "(7, 6) -> %c\n",
    plateau_pions_get_case (p_p, 7, 3),
    plateau_pions_get_case (p_p, 7, 4),
    plateau_pions_get_case (p_p, 7, 5),
    plateau_pions_get_case (p_p, 7, 6));
  
  plateau_afficher ((Plateau *) p_p);
  plateau_pions_free (p_p);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ce code affiche le résultat suivant :

~~~~ {#mycode .C}
Contenu de quelques cases...
(7, 3) ->  
(7, 4) -> O
(7, 5) -> O
(7, 6) -> #

_______________
| |#| |#| |#| |#|
|#| |#| |#| |#| |
| |#| |#| |#| |#|
|#| |#| |#| |#| |
| |#| |#| |#| |#|
|#| |#| |#| |#| |
| |#| |#| |#| |#|
|#| |#| |#| |#| |
| |#| |#| |#| |#|
 ---------------
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

La bonne nouvelle, c'est : notre classe `PlateauPions` fonctionne : on
peut ajouter un pion, et lorsqu'on regarde la valeur de la case avec
`get_case`, on obtient effectivement la valeur attendue (`O` s'il y a
un pion, une espace ou `#` s'il n'y en a pas).

La mauvaise nouvelle c'est que notre classe fonctionne, mais pas comme
on le voudrait, comme en témoigne le damier complétement vide que nous
donne la méthode `afficher`.

La raison à cela est simple : dans `plateau_afficher`, on appelle en
boucle `plateau_get_case` pour obtenir la valeur de la case. Or, dans
ce cas là, il faudrait que ce soit l'implémentation de la classe
`PlateauPions` qui soit appelée. 

Pour cela, il est nécessaire de déclarer la méthode `get_case` comme
une *méthode virtuelle*. Cela dit, contrairement aux langages orientés
objet, cela va être un peu plus compliqué que de juste rajouter le
mot-clé `virtual`...

# Méthodes virtuelles #

En C, l'implémentation de méthodes virtuelles va se faire par des
pointeurs sur fonction. En effet, il est tout à fait possible de
rajouter un nouveau champ `get_case` à la structure `Plateau` qui
serait un pointeur sur fonction, et qui aurait des valeurs différentes
pour les objets de type `Plateau` et de type `PlateauPions` : 

```C
typedef struct Plateau {
  int longueur;
  int largeur;
  char (*get_case) (struct _Plateau * self, int x, int y);
} Plateau;
```

Le type `Plateau` a maintenant un nouveau champ, `get_case`, qui est
un pointeur vers une fonction qui prend comme argument un `Plateau` et
deux entiers, et renvoie un caractère (à l'intérieur de la définition
de la structure, comme le `typedef` n'a pas encore pu avoir effet, il
n'est pas possible d'utiliser directement le type `Plateau`, d'où
l'utilisation de `struct _Plateau`). 

Il faut maintenant assigner cette variable au moment de la création
d'un `Plateau`, ainsi que d'un `PlateauPions` :

```C
void plateau_init (Plateau * plateau, int longueur, int largeur)
{
  plateau->longueur = longueur;
  plateau->largeur = largeur;
  plateau->get_case = plateau_get_case;
}

void plateau_pions_init (PlateauPions * self, int longueur, int largeur)
{
  int i;

  plateau_init ((Plateau *) self, longueur, largeur);
  self->parent.get_case = (char (*) (Plateau *, int, int)) plateau_pions_get_case;
  /* [...] (allocation de self->pions) */
}
```

Mis à part le *cast* sur le type de fonction, qui n'est pas très joli
à voir, cela reste assez simple, et on peut maintenant utiliser
`p->get_case (...)` pour appeler, selon la classe de `p`,
l'implémentation de `get_case` par `Plateau` ou `PlateauPions`. Il ne
nous reste donc plus qu'à modifier la méthode `plateau_afficher` pour
qu'elle utilise `self->get_case` au lieu de `plateau_get_case` :

```C
for (...)
{
    for (...)
        {
            printf ("%c|", self->get_case (self, j, i));
        }
    }    
}
```

Une fois ces changements faits, on peut recompiler notre programme
précédent. La méthode `afficher` marche maintenant correctement pour
la classe `PlateauPions` :

```
Contenu de quelques cases...
(7, 3) ->  
(7, 4) -> O
(7, 5) -> O
(7, 6) -> #
 _______________
| |#| |#| |#| |#|
|#| |#| |#| |#| |
| |#| |#| |#| |#|
|#| |#| |#| |#| |
| |#| |#| |#| |O|
|#| |#| |#| |#|O|
| |#| |#| |#| |#|
|#| |#| |#| |#| |
| |#| |#| |#| |#|
 ---------------
```

On voit effectivement maintenant deux «pions» (sous forme de O) à
droite de l'échiquier, en position (7, 4) et (7, 5).

# Conclusion #

Avec l'implémentation de classes, d'objets, d'héritage et de méthode
virtuelle, nous avons maintenant un petit programme, écrit en C, qui
utilise les mécanismes de programmation orienté objet. 

On pourrait s'arrêter là — le programme fonctionne, après tout (même
s'il ne fait pas grand chose) — mais il n'en reste pas moins que notre
approche présente un certain nombre de problèmes :

* Dans l'état actuel des choses, on peut appeler soit
  `plateau_pions_get_case`, soit `plateau_get_case`, soit
  `p->get_case`. Seule la dernière est virtuelle. Idéalement, il
  faudrait que seule celle-ci puisse être appelée, mis à part par les
  classes dérivées ; le C ne fournissant pas de mot-clé `protected`
  (ni `private`, d'ailleurs), cela risque d'être compliqué, mais on
  peut au moins faire en sorte que cela soit plus clair pour
  l'utilisateur de ces classee.
* Par ailleurs, il serait bien qu'il ait une certaine homogénéité
  entre les appels de méthode et d'éviter d'avoir à appeler
  alternativement `classe_méthode` ou `objet->méthode` selon qu'une
  méthode est virtuelle ou pas. 
* Le pointeur sur la fonction `get_case` est stocké directement dans
  l'instance alors que, pour une méthode virtuelle, la valeur sera la
  même pour tous les objets de la classe. Si on crée beaucoup
  d'objets avec beaucoup de méthodes virtuelles, cela peut entraîner
  une consommation accrue d'espace mémoire.
* À aucun moment, on ne vérifie que l'objet qu'une méthode prend en
  paramètre est bien d'un type «compatible» (c'est-à-dire, est une
  instance de la classe, ou d'une classe dérivée). 
* La méthode `plateau_pions_free` n'est pas sans poser problème, car
  elle libère à la fois la mémoire allouée par un objet et l'objet en
  question : si une nouvelle classe hérite de PlateauPions, elle devra
  réimplémenter complétement cette méthode.

Nous tâcherons de résoudre — au moins en partie — ces problèmes dans
le prochain article.

# Annexe : code source complet #

## plateau.h ##

```C
#ifndef _PLATEAU_H_
#define _PLATEAU_H_

/* Structure contenant les différentes propriétés d'un plateau */
typedef struct _Plateau {
  int longueur;
  int largeur;
  char (*get_case) (struct _Plateau * self, int x, int y);
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
void plateau_init (Plateau * self, int longueur, int largeur)
{
  self->longueur = longueur;
  self->largeur = largeur;
  self->get_case = plateau_get_case;
}

/**
 * Alloue la mémoire nécessaire pour stocker un Plateau et l'initialise
 **/
Plateau * plateau_new (int longueur, int largeur)
{
  Plateau * self = calloc (1, sizeof (Plateau));
  plateau_init (self, longueur, largeur);
  return self;
}

/**
 * Renvoie la longueur d'un plateau
 **/
int plateau_get_longueur (Plateau * self)
{
  return self->longueur;
}

/**
 * Renvoie la largeur d'un plateau
 **/
int plateau_get_largeur (Plateau * self)
{
  return self->largeur;
}

/**
 * Renvoie le caractère correspondant à la case située à la position
 * (x, y) du plateau
 **/
char plateau_get_case (Plateau * self, int x, int y)
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
void plateau_afficher (Plateau * self)
{
  int i;
  int j;

  /* Bordure du haut */
  printf (" ");
  for (j = 0; j < self->largeur - 1;j++)
    {
      printf ("__");
    }
  printf ("_\n");

  /* Lignes du damier */
  for (i = 0; i < self->longueur; i++)
    {
      printf ("|");
      for (j = 0; j < self->largeur; j++)
        {
          printf ("%c|", self->get_case (self, j, i));
        }
      printf ("\n");
    }

  /* Bordure du bas */
  /* Bordure du haut */
  printf (" ");
  for (j = 0; j < self->largeur-1;j++)
    {
      printf ("--");
    }
  printf ("-\n");
}
```C

## plateaupions.h ##

```C
#ifndef _PLATEAU_PIONS_H_
#define _PLATEAU_PIONS_H_

#include "plateau.h"

/* Structure PlateauPions, héritant de Plateau */
typedef struct {
  Plateau parent;
  char **pions;
} PlateauPions;

PlateauPions * plateau_pions_new (int longueur, int largeur);
void plateau_pions_init (PlateauPions * self, int longueur, int largeur);
void plateau_pions_ajouter_pions (PlateauPions * self, int x, int y);
void plateau_pions_retirer_pions (PlateauPions * self, int x, int y);
char plateau_pions_get_case (PlateauPions * self, int x, int y);
void plateau_pions_free (PlateauPions * self);

#endif /* _PLATEAU_PIONS_H_ */
```

## plateaupions.c ##

```C
#include <stdlib.h>

#include "plateau.h"
#include "plateaupions.h"

PlateauPions * plateau_pions_new (int longueur, int largeur)
{
  PlateauPions * self = calloc (1, sizeof (PlateauPions));
  plateau_pions_init (self, longueur, largeur);
  return self;
}

void plateau_pions_init (PlateauPions * self, int longueur, int largeur)
{
  int i;

  plateau_init ((Plateau *) self, longueur, largeur);
  self->parent.get_case = (char (*) (Plateau *, int, int)) plateau_pions_get_case;

  self->pions = calloc (largeur, sizeof (char *));
  for (i = 0; i < largeur; i++)
    {
      self->pions[i] = calloc (longueur, sizeof (char));
    }
}

void plateau_pions_ajouter_pion (PlateauPions * self, int x, int y)
{
  self->pions[x][y] = 1;
}

void plateau_pions_retirer_pion (PlateauPions * self, int x, int y)
{
  self->pions[x][y] = 0;
}

char plateau_pions_get_case (PlateauPions * self, int x, int y)
{
  if (self->pions[x][y])
    {
      return 'O';
    }
  else
    {
      return plateau_get_case ((Plateau *) self, x, y);
    }
}

void plateau_pions_free (PlateauPions *self)
{
  int largeur = plateau_get_largeur ((Plateau *) self);
  int i;

  for (i = 0; i < largeur; i++)
    {
      free (self->pions[i]);
    }
  free (self->pions);
  free (self);
}
```

## damier.c ##

```C
#include <stdio.h>
#include <stdlib.h>

#include "plateau.h"
#include "plateaupions.h"

int main (int argc, char **argv)
{
  PlateauPions * p_p = plateau_pions_new (9, 8);

  plateau_pions_ajouter_pion (p_p, 7, 4);
  plateau_pions_ajouter_pion (p_p, 7, 5);

  printf ("Contenu de quelques cases...\n"
    "(7, 3) -> %c\n"
    "(7, 4) -> %c\n"
    "(7, 5) -> %c\n"
    "(7, 6) -> %c\n",
    plateau_pions_get_case (p_p, 7, 3),
    plateau_pions_get_case (p_p, 7, 4),
    plateau_pions_get_case (p_p, 7, 5),
    plateau_pions_get_case (p_p, 7, 6));
  
  plateau_afficher ((Plateau *) p_p);
  plateau_pions_free (p_p);

  return 0;
}
```
