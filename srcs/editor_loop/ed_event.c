#include "doom_nukem.h"
#include "ui_error.h"

static int	ed_add_mob(t_win *win, t_map *map)
{
	t_mob	*mob;

	if (!(mob = ft_memalloc(sizeof(t_mob))))
		return (ui_ret_error("ed_add_mob", "mob allocation failed", 0));
	mob->pos.x = ed_get_map_x(map, win->winui->mouse.pos.x);
	mob->pos.y = ed_get_map_y(map, win->winui->mouse.pos.y);
	mob->pos.z = map->editor.settings.mob.z;
	mob->width = map->editor.settings.mob.width;
	mob->height = map->editor.settings.mob.height;
	mob->health = map->editor.settings.mob.health;
	mob->damage = map->editor.settings.mob.damage;
	mob->vel = map->editor.settings.mob.velocity;
	mob->texture = ft_strdup(map->editor.settings.texture);
	add_existing_mob(&map->mob, mob);
	return (1);
}

static void	ed_place_poly(t_win *win, t_map *map)
{
	SDL_bool	call_place;

	call_place = SDL_FALSE;
	if (win->winui->mouse.releasing & UI_MOUSE_LEFT && !(map->editor.flags & ED_FLAT)
													&& !(map->editor.flags & ED_INCLINED))
	{
		if (map->editor.flags & ED_MOB)
			ed_add_mob(win, map);
		else
		{
			add_existing_poly(&map->polys, map->editor.placing_poly);
			map->editor.placing_poly = NULL;
		}
	}
	else if (win->winui->mouse.clicked & UI_MOUSE_LEFT)
	{
		if (map->editor.flags & ED_WALL)
			ed_place_wall(win, map);
	}
	if (map->editor.flags & ED_FLAT || map->editor.flags & ED_INCLINED)
	{
		if (win->winui->mouse.clicking & UI_MOUSE_LEFT
		|| (win->winui->mouse.releasing & UI_MOUSE_LEFT && map->editor.place_step != 0))
			map->editor.place_step++;
		if (map->editor.place_step == 0
		&& win->winui->mouse.clicking & UI_MOUSE_LEFT)
			call_place = 1;
		else if (map->editor.place_step == 1
		&& win->winui->mouse.clicked & UI_MOUSE_LEFT)
			call_place = 1;
		else if (map->editor.place_step == 2)
			call_place = 1;
		else if (map->editor.place_step == 3)
		{
			add_existing_poly(&map->polys, map->editor.placing_poly);
			map->editor.placing_poly = NULL;
			map->editor.place_step = 0; 
		}
		if (call_place)
		{
			if (map->editor.flags & ED_FLAT)
				ed_place_flat(win, map);
			else if (map->editor.flags & ED_INCLINED)
				ed_place_inclined(win, map);
		}
	}
}

static void	ed_place_item(t_win *win, t_map *map)
{
	t_object	*obj;
	t_dot		pos;

	if (win->winui->mouse.releasing & UI_MOUSE_LEFT)
	{
		if ((obj = (t_object*)ft_memalloc(sizeof(t_object))))
		{
			pos = ed_get_map_point(map, win->winui->mouse.pos);
			if (map->editor.flags & ED_HEAL)
			{
				obj->type = HEAL;
				obj->texture = ft_strdup("healthPotion.png");
			}
			else if (map->editor.flags & ED_SHIELD)
			{
				obj->type = ARMOR;
				obj->texture = ft_strdup("shieldPotion.png");
			}
			else if (map->editor.flags & ED_GRAVITY)
			{
				obj->type = GRAVITY_INV;
				obj->texture = ft_strdup("pq.png");
			}
			else if (map->editor.flags & ED_BULLET)
			{
				obj->type = BULLET;
				obj->texture = ft_strdup("Ammo_box_icon.png");
			}
			else if (map->editor.flags & ED_BOX)
			{
				obj->type = BOX;
				obj->texture = map->editor.settings.texture;
			}
			obj->pos_rotz_only.x = pos.x;
			obj->pos_rotz_only.y = pos.y;
			obj->pos_rotz_only.z = map->editor.settings.object.z;
			obj->width = map->editor.settings.object.width;
			obj->height = map->editor.settings.object.width;
			obj->light_coef = 0.5;
			obj->poly = NULL;
			add_existing_object(&map->objects, obj);
		}
	}
}

static void	ed_action(t_win *win, t_map *map)
{
	if ((win->winui->mouse.clicked & UI_MOUSE_RIGHT || map->editor.flags & ED_SELECTION) && !win->winui->ui.on_mouse_button)
		ed_selection(win, map);
	else if (map->editor.flags & ED_PLAYER  && !win->winui->ui.on_mouse_button)
		ed_place_player(win, map);
	else if (map->editor.flags & ED_DOOR && !win->winui->ui.on_mouse_button)
		ed_place_door_event(win, map);
	else if ((map->editor.flags & ED_HEAL ||
				map->editor.flags & ED_SHIELD ||
				map->editor.flags & ED_GRAVITY ||
				map->editor.flags & ED_BULLET ||
				map->editor.flags & ED_BOX) && !win->winui->ui.on_mouse_button)
		ed_place_item(win , map);
	else if (map->editor.flags & ED_PLACE && !win->winui->ui.on_mouse_button)
		ed_place_poly(win, map);
	if (map->editor.flags & ED_MODE_CHANGED)
	{
		map->editor.flags ^= ED_MODE_CHANGED;
		if (map->editor.flags & ED_SELECTION || map->editor.flags & ED_PLACE)
			SDL_SetCursor(map->editor.cursor[CURSOR_SELECTING]);
		else
			SDL_SetCursor(map->editor.cursor[CURSOR_DEFAULT]);
	}
}

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
	map->player.pos.z = 0;	
}

int 		ed_event(t_win *win, t_map *map)
{
	const Uint8	*state;
	int			vel;

	state = SDL_GetKeyboardState(NULL);
	if (map->player.pos.z != 0)
		ed_move_player_z(map);
	if (!win->winui->ui.clicked_button)
	{
		vel = 3;
		if (state[SDL_SCANCODE_LSHIFT])
			vel += 4;
		if (state[SDL_SCANCODE_A])
			map->editor.pos.x -= vel;
		if (state[SDL_SCANCODE_D])
			map->editor.pos.x += vel;
		if (state[SDL_SCANCODE_W])
			map->editor.pos.y -= vel;
		if (state[SDL_SCANCODE_S])
			map->editor.pos.y += vel;
	}
	if (state[SDL_SCANCODE_DELETE] && (map->editor.selected_poly || map->editor.selected_mob || map->editor.selected_obj))
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
	if(win->winui->event.type == SDL_MOUSEWHEEL)
	{
		map->editor.unit += win->winui->event.wheel.y * 0.01;
		win->winui->event.wheel.y = 0;
	}
	ed_action(win, map);
	return (1);
}
