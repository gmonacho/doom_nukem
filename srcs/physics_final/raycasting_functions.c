#include "doom_nukem.h"

/*
**	Sence : Return 1 si la collision est dans le sens du vecteur directeur du rayon
**			Si la collision et l'origin du rayon sont confondu return 0
*/




// int             sence(t_cartesienne ray, t_fdot_3d collision)
// {
// 	// return (scalar_product((t_fdot_3d){ray->vx, ray->vy, ray->vz}, collision) > 0);
// 	if (ray.vx)
// 	{
// 		if (collision.x < ray.ox && ray.ox + ray.vx < ray.ox)
// 			return (1);
// 		else if (collision.x > ray.ox && ray.ox + ray.vx > ray.ox)
// 			return (1);
// 		else
// 			return (0);
// 	}
// 	else if (ray.vy)
// 	{
// 		if (collision.y < ray.oy && ray.oy + ray.vy < ray.oy)
// 			return (1);
// 		else if (collision.y > ray.oy && ray.oy + ray.vy > ray.oy)
// 			return (1);
// 		else
// 			return (0);
// 	}
// 	else
// 	{
// 		if (collision.z < ray.oz && ray.oz + ray.vz < ray.oz)
// 			return (1);
// 		else if (collision.z > ray.oz && ray.oz + ray.vz > ray.oz)
// 			return (1);
// 		else
// 			return (0);
// 	}
// }
