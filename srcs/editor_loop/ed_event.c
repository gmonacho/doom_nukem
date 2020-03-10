#include "doom_nukem.h"
#include "ui_error.h"

static void	ed_move_player_z(t_map *map)
{
	t_poly		*p;
	t_mob		*mob;
	t_object	*obj;
	int			i;

	p = map->polys;
	while (p)
	{
		i = 0;
		while (i < 4)
			p->dots[i++].z -= map->player.pos.z;
		p = p->next;
	}
	obj = map->objects;
	while (obj)
	{
		obj->pos_rotz_only.z -= map->player.pos.z;
		obj = obj->next;
	}
	mob = map->mob;
	while (mob)
	{
		mob->pos.z -= map->player.pos.z;
		mob = mob->next;
	}
}

static void	ed_delete_event(t_win *win, t_map *map)
{
	ed_clean_property(win, 1);
	if (map->editor.selected_poly)
	{
		delete_poly(&map->polys, map->editor.selected_poly);
		map->editor.selected_poly = NULL;
	}
	else if (map->editor.selected_mob)
	{
		ed_delete_mob(&map->mob, map->editor.selected_mob);
		map->editor.selected_mob = NULL;
	}
	else if (map->editor.selected_obj)
	{
		ed_delete_obj(map, &map->objects, map->editor.selected_obj);
		map->editor.selected_obj = NULL;
	}
}

static void	ed_next_events(t_win *win, t_map *map, const Uint8 *state)
{
	if (state[SDL_SCANCODE_SPACE])
	{
		if (map->editor.flags & ED_DRAW_HELP)
			map->editor.flags ^= ED_DRAW_HELP;
	}
	else
	{
		if (!(map->editor.flags & ED_DRAW_HELP))
			map->editor.flags |= ED_DRAW_HELP;
	}
	if (win->winui->event.type == SDL_MOUSEWHEEL)
	{
		map->editor.unit += win->winui->event.wheel.y * 0.01;
		win->winui->event.wheel.y = 0;
	}
}

static void	ed_edit_vel(const Uint8 *state, t_dot *pos)
{
	int vel;

	vel = 3;
	if (state[SDL_SCANCODE_LSHIFT])
		vel += 4;
	if (state[SDL_SCANCODE_A])
		pos->x -= vel;
	if (state[SDL_SCANCODE_D])
		pos->x += vel;
	if (state[SDL_SCANCODE_W])
		pos->y -= vel;
	if (state[SDL_SCANCODE_S])
		pos->y += vel;
}

int			ed_event(t_win *win, t_map *map)
{
	const Uint8	*state;

	state = SDL_GetKeyboardState(NULL);
	if (map->player.pos.z != 0)
	{
		ed_move_player_z(map);
		map->player.pos.z = 0;
	}
	if (!win->winui->ui.clicked_button)
		ed_edit_vel(state, &map->editor.pos);
	if (state[SDL_SCANCODE_DELETE]
		&& (map->editor.selected_poly
			|| map->editor.selected_mob
			|| map->editor.selected_obj))
		ed_delete_event(win, map);
	ed_next_events(win, map, state);
	ed_action(win, map);
	return (1);
}
