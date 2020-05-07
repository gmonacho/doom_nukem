/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 16:06:45 by gal               #+#    #+#             */
/*   Updated: 2020/05/07 13:58:36 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		main_free_rays(t_cartesienne **rays)
{
	t_cartesienne	*l;
	t_cartesienne	*next;
	int				i;

	if (rays)
	{
		i = 0;
		while (rays[i])
		{
			l = rays[i];
			while (l)
			{
				next = l->next;
				free(l);
				l = next;
			}
			i++;
		}
	}

}

static void		main_free_inventory(t_inventory *inventory)
{
	int i;

	i = 0;
	while (i < 5)
	{
		if (inventory->item[i])
		{
			// if (inventory->item[i]->pos)
			// 	free(inventory->item[i]->pos);
			free(inventory->item[i]);
		}
		i++;
	}
}

static void		main_free_player(t_player *player)
{
	if (player->inventory)
	{
		main_free_inventory(player->inventory);
		free(player->inventory);
		player->inventory = NULL;
	}
	if (player->rays)
	{
		main_free_rays(player->rays);
		free(player->rays);
	}
	
	
}

static void		main_free_polys(t_poly **polys)
{
	t_poly	*p;
	t_poly	*next;

	if (polys)
	{
		p = *polys;
		while (p)
		{
			next = p->next;
			free(p);
			p = next;
		}
		*polys = NULL;
	}
}

static void		main_free_music(t_music *music)
{
	int i = 0;

	while (i < 7)
	{
		if (music->tmusic[i])
			Mix_FreeChunk(music->tmusic[i]);
		i++;
	}
}

static void		main_free_map(t_map *map)
{
	if (map)
	{
		main_free_player(&map->player);
		if (map->editor.export.path)
			ft_strdel(&map->editor.export.path);
		if (map->editor.settings.texture)
			ft_strdel(&map->editor.settings.texture);
		if (map->polys_save)
			main_free_polys(&map->polys_save);
		if (map->music)
		{
			main_free_music(map->music);
			free(map->music);
			map->music = NULL;
		}
		ed_delete_map(map);
	}
}

static void		main_free_texthud(t_texhud *texhud)
{
	int i;

	i = 0;
	while (i < 16)
		SDL_DestroyTexture(texhud->tex[i++]);
	i = 0;
	while (i < 6)
		SDL_DestroyTexture(texhud->tex_weapon[i++]);
	i = 0;
	while (i < 5)
		SDL_DestroyTexture(texhud->tex_reload[i++]);
	if (texhud->police)
		TTF_CloseFont(texhud->police);
}

static void		main_free_win(t_win *win)
{
	if (win)
	{
		if (win->pixels)
			free(win->pixels);
		if (win->texhud)
		{
			main_free_texthud(win->texhud);
			free(win->texhud);
		}
		if (win->threads)
			free(win->threads);
	}
}

void			main_free(t_win *win, t_map *map)
{
	main_free_win(win);
	main_free_map(map);
}