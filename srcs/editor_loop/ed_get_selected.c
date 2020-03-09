#include "doom_nukem.h"

static SDL_bool	ed_is_mob_selected(t_win *win,
								const t_map *map,
								const t_mob *mob)
{
	if (ed_get_line_len(&(t_line){ed_get_map_point(map, win->winui->mouse.pos),
						(t_dot){mob->pos.x, mob->pos.y}}) < mob->width)
		return (SDL_TRUE);
	else
		return (SDL_FALSE);
}

t_mob			*ed_get_selected_mob(t_win *win, const t_map *map)
{
	t_mob	*m;

	m = map->mob;
	while (m)
	{
		if (ed_is_mob_selected(win, map, m))
			return (m);
		m = m->next;
	}
	return (NULL);
}

t_player		*ed_get_selected_player(t_win *win, t_map *map)
{
	if (ed_get_line_len(&(t_line){ed_get_map_point(map, win->winui->mouse.pos),
			(t_dot){map->player.pos.x, map->player.pos.y}}) < map->player.width)
		return (&map->player);
	else
		return (NULL);
}

static SDL_bool	ed_is_obj_selected(t_win *win,
								const t_map *map,
								const t_object *object)
{
	if (ed_get_line_len(&(t_line){ed_get_map_point(map, win->winui->mouse.pos),
						(t_dot){object->pos_rotz_only.x,
								object->pos_rotz_only.y}}) < object->width)
		return (SDL_TRUE);
	else
		return (SDL_FALSE);
}

t_object		*ed_get_selected_obj(t_win *win, const t_map *map)
{
	t_object	*obj;

	obj = map->objects;
	while (obj)
	{
		if (ed_is_obj_selected(win, map, obj))
			return (obj);
		obj = obj->next;
	}
	return (NULL);
}
