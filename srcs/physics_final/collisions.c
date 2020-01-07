#include "doom_nukem.h"

static int	is_collision(t_poly *poly, int zc, int width)
{
	int		ad;
	int		bd;
	int		bs;

	if (!poly->equation_rotz_only.v.y)
		return (1);
	ad = poly->equation_rotz_only.v.x / poly->equation_rotz_only.v.y;
	bd = (poly->equation_rotz_only.v.z * zc + poly->equation_rotz_only.d) / poly->equation_rotz_only.v.y;
	bs = 2 * ad * bd;
	if (bs * bs - 4 * (1 - ad * ad) * (bd * bd - width * width) < 0)
		return (0);
	else
		return (1);
}

void		collisions(t_player *player, t_poly *poly)
{
	int		z;

	while (poly)
	{
		z = player->pos_up.z - player->height;
		while (z++ < player->pos_up.z)
			printf("Collision : %d\n", is_collision(poly, z, player->width));
		poly = poly->next;
	}
	printf("\n\n");
}