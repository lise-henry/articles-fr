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
  PlateauPions * p_p = plateau_pions_new (6, 8);

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