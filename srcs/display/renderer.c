#include "doom_nukem.h"

void	clear_rend(SDL_Renderer *rend, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(rend, r, g, b, 255);
	SDL_RenderClear(rend);
}