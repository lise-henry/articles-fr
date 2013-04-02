#include <stdio.h>

#include "plateau.h"

int main (int argc, char **argv)
{
  Plateau p;
  printf ("Emplacement mémoire de...\n"
    "p: %x\n"
    "p.longueur: %x\n"
    "p.largeur: %x\n"
    "diff: %d\n",
    &p, &p.longueur, &p.largeur, (&p.largeur)-(&p.longueur));
  return 0;
}
