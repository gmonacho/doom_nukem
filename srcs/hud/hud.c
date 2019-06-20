#include "doom_nukem.h"

void 	hud(t_win *win)
{	
	SDL_Texture		*healthBar;
	SDL_Texture		*healthPixel;
	int 			i; 
	float			hpPosx;
	int 			currentHp;

	i = -1;
	hpPosx = 1.29;
	currentHp = 88;
	healthBar = load_texture(win->rend, "textures/healthBar.png");
	healthPixel = load_texture(win->rend, "textures/healthPixel.png");
	
	SDL_RenderCopy(win->rend, healthBar, NULL,(&(SDL_Rect){(win->w / 1.5), (win->h / 1.15), (win->w / 3.33), (win->h / 8)}));
	while (i++ < currentHp)
	{
		SDL_RenderCopy(win->rend, healthPixel, NULL,(&(SDL_Rect){(win->w / hpPosx), (win->h / 1.13), (win->w / 13.3), (win->h / 10.6)}));
		hpPosx -= 0.00151;
		printf("salut\n");
	}
}