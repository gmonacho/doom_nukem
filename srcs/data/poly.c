#include "doom_nukem.h"

void    add_poly(t_poly **poly)
{
  t_poly  *new_poly;

  if (!(new_poly = (t_poly *)ft_memalloc(sizeof(t_poly))))
    return (ft_putendl("poly allocation failed in poly.c"));
  new_poly->next = *poly;
  *poly = new_poly;
}
