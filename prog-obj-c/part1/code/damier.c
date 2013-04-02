#include <stdio.h>
#include <stdlib.h>

#include "plateau.h"

typedef struct {
  Plateau parent;
  char **pions;
} PlateauPions;


int main (int argc, char **argv)
{
  Plateau * plateau = calloc (1, sizeof (Plateau));

  plateau_init (plateau, 8, 8);
  plateau_afficher (plateau);
  free (plateau);

  return 0;
}
