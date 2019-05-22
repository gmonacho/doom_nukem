#include "doom_nukem.h"

void	teleportation(t_linedef *line1, t_linedef *line2, t_player *player)
{
	//printf("Changes sector\n");
	player->pos = (t_fdot){	map(player->pos.x,\
									(t_dot){line1->p1.x, line1->p2.x},\
									(t_dot){line2->p1.x, line2->p2.x}) +\
									(player->vel.x > 0 ? 1 : -1) * player->hitbox,\
							map(player->pos.y,\
									(t_dot){line1->p1.y, line1->p2.y},\
									(t_dot){line2->p1.y, line2->p2.y}) +\
									(player->vel.y > 0 ? 1 : -1) * player->hitbox};
	//printf("Changes sector fin\n");
}
