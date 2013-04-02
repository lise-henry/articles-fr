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
