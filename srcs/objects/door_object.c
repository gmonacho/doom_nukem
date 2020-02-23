#include "doom_nukem.h"

int				interact_door(t_object	*door)
{
	if (!door || door->type != DOOR || !door->poly || !door->poly->next)
		return (0);
	if (door->poly->next->visible && door->poly->next->collide)
	{
		printf("next is visible\n");	
		door->poly->next->visible = 0;
		door->poly->next->collide = 0;
		door->poly->visible = 1;
		door->poly->collide = 1;
	}
	else if (door->poly->visible && door->poly->collide)
	{
		printf("front is visible\n");	
		door->poly->visible = 0;
		door->poly->collide = 0;
		door->poly->next->visible = 1;
		door->poly->next->collide = 1;
	}
	printf("done interact door\n");
	return (1);
}

int				set_door_object(t_object *object, t_fdot_3d pos, float width_2, float height_2)
{
	t_fdot_3d	box[6];
	t_poly		*poly;

	box[0] = (t_fdot_3d){pos.x - width_2, pos.y - width_2, pos.z + height_2};
	box[1] = (t_fdot_3d){pos.x + width_2, pos.y - width_2, pos.z + height_2};
	box[2] = (t_fdot_3d){pos.x + width_2, pos.y - width_2, pos.z - height_2};
	box[3] = (t_fdot_3d){pos.x - width_2, pos.y - width_2, pos.z - height_2};
	box[4] = (t_fdot_3d){pos.x - width_2, pos.y + width_2, pos.z + height_2};
	box[5] = (t_fdot_3d){pos.x - width_2, pos.y + width_2, pos.z - height_2};

	if (!(object->poly = (t_poly *)ft_memalloc(sizeof(t_poly))))
		return (1);
	poly = object->poly;
	poly->object = object;
	poly->light_coef = object->light_coef;
	poly->visible = 1;
	poly->collide = 1;
	poly->dots_rotz_only[0] = box[0];
	poly->dots_rotz_only[1] = box[1];
	poly->dots_rotz_only[2] = box[2];
	poly->dots_rotz_only[3] = box[3];
	if (!(poly->next = (t_poly *)ft_memalloc(sizeof(t_poly))))
		return (1);
	poly = poly->next;
	poly->object = object;
	poly->light_coef = object->light_coef;
	poly->visible = 0;
	poly->collide = 0;
	poly->dots_rotz_only[0] = box[0];
	poly->dots_rotz_only[1] = box[4];
	poly->dots_rotz_only[2] = box[5];
	poly->dots_rotz_only[3] = box[3];
	poly->next = NULL;
	object->collide = 0;
	printf("created door at %p, with polys %p |%d|and %p |%d|\n", object, object->poly, object->poly->visible, object->poly->next, object->poly->next->visible);
	return (0);
}