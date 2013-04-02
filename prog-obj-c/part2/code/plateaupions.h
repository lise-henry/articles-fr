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
