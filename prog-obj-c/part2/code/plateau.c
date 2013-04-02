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

