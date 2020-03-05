#include "doom_nukem.h"
#include "ui_error.h"

static int	ed_place_door(t_win *win, t_map *map)
{
	static t_dot	p1 = (t_dot){0, 0};
	t_object		*door;
	t_dot			p;

	door = NULL;
	p = ed_get_map_point(map, win->winui->mouse.pos);
	if (map->editor.flags & ED_DRAW_HELP)
		p = ed_is_next_to_poly(map, p, NEXT_RADIUS);
	if (!map->editor.placing_door)
	{
		p1 = p;
		printf("p1 %d %d\n", p1.x, p1.y);
		if (!(door = ft_memalloc(sizeof(t_object))))
		{
			return (ui_ret_error("ed_place_door",
			"door allocation failed", 0));
		}
		door->pos_rotz_only.x = p1.x;
		door->pos_rotz_only.y = p1.y;
		door->dir = 1;
		door->type = DOOR;
		door->height = 60;
		door->width = 30;
		map->editor.placing_door = door;
		door->texture = ft_strdup("doorfront.png");
		door->light_coef = 1;
	}
	else
	{
		printf("p1 %d %d p %d %d\n", p1.x, p1.y, p.x, p.y);
		// map->editor.placing_door->pos_rotz_only.x = (p1.x + p.x) / 2;
		// map->editor.placing_door->pos_rotz_only.y = (p1.y + p.y) / 2;
		map->editor.placing_door->width = ed_get_line_len(&(t_line){p1, p});
	}
	return (1);
}

void	ed_place_door_event(t_win *win, t_map *map)
{
	if (win->winui->mouse.releasing & UI_MOUSE_LEFT)
	{
		add_existing_object(&map->objects, map->editor.placing_door);
		map->editor.placing_door = NULL;
	}
	else if (win->winui->mouse.clicked & UI_MOUSE_LEFT)
		ed_place_door(win, map);
}