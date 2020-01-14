#include "doom_nukem.h"

void    destroyTextures(SDL_Texture *texture1, SDL_Texture *texture2, SDL_Texture *texture3, SDL_Texture *texture4)
{
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(texture4);
}

int 	clickOnPos(SDL_Event event, t_frect *rect) //&(SDL_Rect){(win->w * 0.05), (win->h * 0.8125), (win->w * 0.25), (win->h * 0.0625)}
{
	int x;
    int y;

    x = event.motion.x;
    y = event.motion.y;
    if ((x >= rect->x && x <= (rect->x + rect->w)) && (y >= rect->y && y <= (rect->y + rect->h)))
    {   
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT))
            return (1);
    }
    return (0);
}

int    dead_moment(t_win *win, t_player *player, t_texHud *texHud, SDL_Event event)
 {  
    SDL_Texture *textM;
    SDL_Texture *textMB;
    SDL_Texture *textQ;
    SDL_Texture *textQB;
    
    textM = NULL;
    textQ = NULL;
    if (player->currentHp <= 0)
    {   
        textM = generate_text(win->rend, win->main_menu->police, "MENU", (SDL_Color){255, 0, 0, 50});
        textMB = generate_text(win->rend, win->main_menu->police, "MENU", (SDL_Color){0, 0, 0, 50});
        textQ = generate_text(win->rend, win->main_menu->police, "QUIT", (SDL_Color){255, 0, 0, 50});
        textQB = generate_text(win->rend, win->main_menu->police, "QUIT", (SDL_Color){0, 0, 0, 50});
        SDL_RenderCopy(win->rend, texHud->tex[12], NULL, &(SDL_Rect){(0), (0), (win->w), (win->h)});
        SDL_RenderCopy(win->rend, textMB, NULL, &(SDL_Rect){(win->w * 0.382), (win->h * 0.625), (win->w * 0.256), (win->h * 0.1325)});
		SDL_RenderCopy(win->rend, textM, NULL, &(SDL_Rect){(win->w *0.38), (win->h * 0.625), (win->w * 0.25), (win->h * 0.125)});
        SDL_RenderCopy(win->rend, textQB, NULL, &(SDL_Rect){(win->w * 0.382), (win->h * 0.75), (win->w * 0.256), (win->h * 0.1325)});
        SDL_RenderCopy(win->rend, textQ, NULL, &(SDL_Rect){(win->w *0.38), (win->h * 0.75), (win->w * 0.25), (win->h * 0.125)});
        SDL_SetRelativeMouseMode(SDL_FALSE);
        if (clickOnPos(event, &(t_frect){(win->w * 0.38), (win->h * 0.625), (win->w * 0.25), (win->h * 0.125)}) == 1)
            return (2);
        if (clickOnPos(event, &(t_frect){(win->w * 0.38), (win->h * 0.75), (win->w * 0.25), (win->h * 0.125)}) == 1)
            return (1);
        //destroyTextures(textM, textMB, textQ, textQB);
    }
	// event.type = SDL_FALSE;
    return (0);
}