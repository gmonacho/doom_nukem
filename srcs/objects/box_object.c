#include "doom_nukem.h"

int				set_box_object(t_object *object, t_fdot_3d pos, float width_2, float height_2)
{
	t_fdot_3d	box[8];
	t_poly		*poly;
	int			i;
	int			j;

	box[0] = (t_fdot_3d){pos.x + width_2, pos.y - width_2, pos.z + height_2};
	box[1] = (t_fdot_3d){pos.x + width_2, pos.y + width_2, pos.z + height_2};
	box[2] = (t_fdot_3d){pos.x - width_2, pos.y + width_2, pos.z + height_2};
	box[3] = (t_fdot_3d){pos.x - width_2, pos.y - width_2, pos.z + height_2};
	box[4] = (t_fdot_3d){pos.x - width_2, pos.y - width_2, pos.z - height_2};
	box[5] = (t_fdot_3d){pos.x - width_2, pos.y + width_2, pos.z - height_2};
	box[6] = (t_fdot_3d){pos.x + width_2, pos.y + width_2, pos.z - height_2};
	box[7] = (t_fdot_3d){pos.x + width_2, pos.y - width_2, pos.z - height_2};

	if (!(object->poly = (t_poly *)ft_memalloc(sizeof(t_poly))))
		return (1);
	poly = object->poly;
	poly->object = object;
	poly->dots[0] = box[0];
	poly->dots[1] = box[3];
	poly->dots[2] = box[4];
	poly->dots[3] = box[7];
	
	if (!(poly->next = (t_poly *)ft_memalloc(sizeof(t_poly))))
		return (1);
	poly = poly->next;
	poly->object = object;
	poly->dots[0] = box[1];
	poly->dots[1] = box[2];
	poly->dots[2] = box[5];
	poly->dots[3] = box[6];

	if (!(poly->next = (t_poly *)ft_memalloc(sizeof(t_poly))))
		return (1);
	poly = poly->next;
	poly->object = object;
	poly->dots[0] = box[0];
	poly->dots[1] = box[1];
	poly->dots[2] = box[6];
	poly->dots[3] = box[7];

	i = 0;
	while (i < 5)
	{
		if (!(poly->next = (t_poly *)ft_memalloc(sizeof(t_poly))))
			return (1);
		poly = poly->next;
		poly->object = object;
		j = -1;
		while (++j < 4)
		{
			poly->dots[j] = box[i + j];
			// printf("Segment : %d %d\n", );
		}
		i += 2;
	}
	poly->next = NULL;
	return (0);
}