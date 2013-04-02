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
