#include "doom_nukem.h"

void	teleportation(t_linedef *line1, t_linedef *line2, t_player *player)
{
	line1 = NULL;
	line2 = NULL;
	printf("Changes sector\n");
	player->pos = (t_dot){10, 10};
}
