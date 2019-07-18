#include "doom_nukem.h"

void 	hud_ammo(t_win *win, t_player *player, t_texHud *texHud)
{
	SDL_Texture		*text;
	char 			*tmp;
	char			*tmp2;
	
	tmp = ft_strjoin(ft_itoa(player->ammo), " | ");
	tmp2 = ft_strjoin(tmp, ft_itoa(player->magazine));
	text = generate_text(win->rend, texHud->police, tmp2, (SDL_Color){255, 255, 255, 50});
	SDL_RenderCopy(win->rend, text, NULL, &(SDL_Rect){(90), (win->h * 0.9125), (win->w * 0.07), (win->h * 0.0725)});
	SDL_RenderCopy(win->rend, texHud->tex[0], NULL, &(SDL_Rect){(win->w * 0.01), (win->h * 0.8875), (win->w * 0.075), (win->h * 0.09375)});
	free(tmp);
}

void 	hud_health(t_win *win, t_player *player, t_texHud *texHud)
{	
	int 			i;
	float			hpPosx;

	i = 0;
	hpPosx = 857;
	SDL_RenderCopy(win->rend, texHud->tex[1], NULL, (&(SDL_Rect){(win->w * 0.76), (win->h * 0.8875), (win->w * 0.2), (win->h * 0.0875)}));
	while (i++ < player->currentHp)
	{
		SDL_RenderCopy(win->rend, texHud->tex[2], NULL, (&(SDL_Rect){(hpPosx), (win->h * 0.92), (win->w * 0.006), (win->h * 0.02)}));
		hpPosx += 0.9;
	}
}

void 	hud_armor(t_win *win, t_player *player, t_texHud *texHud)
{
	int 			i;
	float			armorPosx;

	i = 0;
	armorPosx = 858;
	SDL_RenderCopy(win->rend, texHud->tex[3], NULL,(&(SDL_Rect){(win->w * 0.76), (win->h * 0.7875), (win->w * 0.2), (win->h * 0.0875)}));
	while (i < player->currentArmor)
	{
		SDL_RenderCopy(win->rend, texHud->tex[4], NULL, (&(SDL_Rect){(armorPosx), (win->h * 0.8225), (win->w * 0.006), (win->h * 0.02)}));
		armorPosx += 0.9;
		i++;
	}
}

void	hud(t_win *win, t_player *player, t_texHud *texHud)
{
	hud_health(win, player, texHud);
	hud_armor(win, player, texHud);
	hud_ammo(win, player, texHud);
    inventory_hud(win, texHud, player);
}