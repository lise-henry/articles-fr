#include <stdio.h>
#include <stdlib.h>

typedef struct {
} Object;

typedef struct {
  Object parent;
  double x;
  double y;
  double z;
} Point;

typedef struct {
  Point parent;
  unsigned char r;
  unsigned char g;
  unsigned char b;
} PointCouleur;

void PointAfficherCoordonnees (Object * self)
{
  Point * point = (Point *) self;
  printf ("(%f, %f, %f)\n", point->x, point->y, point->z);
}

void PointDeplacer (Object * self, double deltax, double deltay, double deltaz)
{
  Point * point = (Point *) self;
  point->x += deltax;
  point->y += deltay;
  point->z += deltaz;
}

void PointInit (Object * self, double x, double y, double z)
{
  Point * point = (Point *) self;
  point->x = x;
  point->y = y;
  point->z = z;
}

void PointCouleurInit (Object * self, double x, double y, double z, unsigned char r, unsigned char g, unsigned char b)
{
  PointCouleur * pointcouleur = (PointCouleur *) self;

  PointInit (self, x, y, z);

  pointcouleur->r = r;
  pointcouleur->g = g;
  pointcouleur->b = b;
}

void PointDecrire (Object * self)
{
  printf ("Un point de coordonnées ");
  PointAfficherCoordonnees (self);
}



int main (int argc, char *argv[])
{
  Object * A = malloc (sizeof (PointCouleur));

  PointCouleurInit (A, 1, 1, 1, 255, 0, 0);

  PointAfficherCoordonnees (A);
  return 0;
}
