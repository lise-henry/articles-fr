#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "plateau.h"
#include "plateaupions.h"

PlateauPions * plateau_pions_new (int longueur, int largeur)
{
  PlateauPions * self = calloc (1, sizeof (PlateauPions));
  plateau_pions_init (self, longueur, largeur);
  return self;
}

void plateau_pions_init (PlateauPions * self, int longueur, int largeur)
{
  int i;

  plateau_init ((Plateau *) self, longueur, largeur);
  self->parent.get_case = (char (*) (Plateau *, int, int)) plateau_pions_get_case;

  self->pions = calloc (largeur, sizeof (char *));
  assert (self->pions);
  for (i = 0; i < largeur; i++)
    {
      self->pions[i] = calloc (longueur, sizeof (char));
    }
}

void plateau_pions_ajouter_pion (PlateauPions * self, int x, int y)
{
  self->pions[x][y] = 1;
}

void plateau_pions_retirer_pion (PlateauPions * self, int x, int y)
{
  self->pions[x][y] = 0;
}

char plateau_pions_get_case (PlateauPions * self, int x, int y)
{
  if (self->pions[x][y])
    {
      return 'O';
    }
  else
    {
      return plateau_get_case ((Plateau *) self, x, y);
    }
}

void plateau_pions_free (PlateauPions *self)
{
  int largeur = plateau_get_largeur ((Plateau *) self);
  int i;

  for (i = 0; i < largeur; i++)
    {
      free (self->pions[i]);
    }
  free (self->pions);
  free (self);
}
