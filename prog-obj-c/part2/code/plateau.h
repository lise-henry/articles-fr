#ifndef _PLATEAU_H_
#define _PLATEAU_H_

/* Structure contenant les différentes propriétés d'un plateau */
typedef struct Plateau {
  int longueur;
  int largeur;
  char (*get_case) (struct Plateau * self, int x, int y);
} Plateau;

/* Méthodes applicables à un plateau */
void plateau_init (Plateau * plateau, int longueur, int largeur);
Plateau * plateau_new (int longueur, int largeur);
int plateau_get_longeuur (Plateau * plateau);
int plateau_get_largeur (Plateau * plateau);
char plateau_get_case (Plateau * plateau, int x, int y);
void plateau_afficher (Plateau * plateau);

#endif /* _PLATEAU_H_ */
