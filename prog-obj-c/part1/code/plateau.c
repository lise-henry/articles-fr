#include <stdio.h>
#include <stdlib.h>

#include "plateau.h"

/**
 * Initialise un plateau déjà alloué
 **/
void plateau_init (Plateau * plateau, int longueur, int largeur)
{
  plateau->longueur = longueur;
  plateau->largeur = largeur;
}

/**
 * Alloue la mémoire nécessaire pour stocker un Plateau et l'initialise
 **/
Plateau * plateau_new (int longueur, int largeur)
{
  Plateau * plateau = calloc (1, sizeof (Plateau));
  plateau_init (plateau, longueur, largeur);
  return plateau;
}

/**
 * Renvoie la longueur d'un plateau
 **/
int plateau_get_longueur (Plateau * plateau)
{
  return plateau->longueur;
}

/**
 * Renvoie la largeur d'un plateau
 **/
int plateau_get_largeur (Plateau * plateau)
{
  return plateau->largeur;
}

/**
 * Renvoie le caractère correspondant à la case située à la position
 * (x, y) du plateau
 **/
char plateau_get_case (Plateau * plateau, int x, int y)
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
void plateau_afficher (Plateau * plateau)
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
          printf ("%c|", plateau_get_case (plateau, j, i));
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

