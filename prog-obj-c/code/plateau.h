#ifndef _PLATEAU_H_
#define _PLATEAU_H_

/* Structure contenant les différentes propriétés d'un plateau */
typedef struct {
  int longueur;
  int largeur;
} Plateau;

/* Méthodes applicables à un plateau */
void plateauInit (Plateau * plateau, double longueur, double largeur);
int plateauGetLongeur (Plateau * plateau);
int plateauGetLargeur (Plateau * plateau);
char plateauGetCase (Plateau * plateau, int x, int y);
void plateauAfficher (Plateau * plateau);

#endif /* _PLATEAU_H_ */
