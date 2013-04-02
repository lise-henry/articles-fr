#include <stdio.h>

#include "plateau.h"

void plateauInit (Plateau * plateau, double longueur, double largeur)
{
  plateau->longueur = longueur;
  plateau->largeur = largeur;
}

int plateauGetLongueur (Plateau * plateau)
{
  return plateau->longueur;
}

int plateauGetLargeur (Plateau * plateau)
{
  return plateau->largeur;
}

char plateauGetCase (Plateau * plateau, int x, int y)
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

void plateauAfficher (Plateau * plateau)
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

