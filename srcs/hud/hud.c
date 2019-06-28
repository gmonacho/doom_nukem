#include "doom_nukem.h"

void 	hud_ammo(t_win *win)
{
	SDL_Texture 	*ammo;
	SDL_Texture		*text;
	TTF_Font		*police;
	
	ammo = load_texture(win->rend, "textures/ammo.png");
	police = TTF_OpenFont("TTF/arial.ttf", 65);
	text = generate_text(win->rend, police, " = 30 ", (SDL_Color){255, 255, 255, 50});
	SDL_RenderCopy(win->rend, text, NULL, &(SDL_Rect){(60), (730), 50, 50});
	SDL_RenderCopy(win->rend, ammo, NULL, &(SDL_Rect){(10), (710), 75, 75});
}	
void 	hud_health(t_win *win, t_player *player)
{	
	SDL_Texture		*healthBar;
	SDL_Texture		*healthPixel;
	int 			i;
	float			hpPosx;

	i = 0;
	hpPosx = 857;
	healthBar = load_texture(win->rend, "textures/healthBar.png");
	healthPixel = load_texture(win->rend, "textures/healthPixel.png");
	SDL_RenderCopy(win->rend, healthBar, NULL, (&(SDL_Rect){(win->w * 0.76), (win->h * 0.8875), (win->w * 0.2), (win->h * 0.0875)}));
	while (i++ < player->currentHp)
	{
		SDL_RenderCopy(win->rend, healthPixel, NULL, (&(SDL_Rect){(hpPosx), (win->h * 0.92), (win->w * 0.006), (win->h * 0.02)}));
		hpPosx += 0.9;
	}
}

void 	hud_armor(t_win *win, t_player *player)
{
	SDL_Texture		*armorBar;
	SDL_Texture		*armorPixel;
	int 			i;
	float			armorPosx;

	i = 0;
	armorPosx = 858;
	armorBar = load_texture(win->rend, "textures/shieldBar.png");
	armorPixel = load_texture(win->rend, "textures/shieldPixel.png");
	SDL_RenderCopy(win->rend, armorBar, NULL,(&(SDL_Rect){(win->w * 0.76), (win->h * 0.7875), (win->w * 0.2), (win->h * 0.0875)}));
	while (i < player->currentArmor)
	{
		SDL_RenderCopy(win->rend, armorPixel, NULL, (&(SDL_Rect){(armorPosx), (win->h * 0.8225), (win->w * 0.006), (win->h * 0.02)}));
		armorPosx += 0.9;
		i++;
	}
}

void	hud(t_win *win, t_player *player)
{
	hud_health(win, player);
	hud_armor(win, player);
	hud_ammo(win);
}