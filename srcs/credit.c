#include "doom_nukem.h"

SDL_Texture    *define_credit(t_win *win)
{
    SDL_Texture *text;
    TTF_Font    *police;
    char *tmp;

    police = TTF_OpenFont("TTF/DooM.ttf", 65);
    tmp = ft_strdup("Aducimet Agiordan Gmomacho");
    text = generate_text(win->rend, police, tmp, (SDL_Color){255, 255, 255, 50});
    free(tmp);
    return (text);
}

void    print_credit(t_win *win)
{
    SDL_Event   event;
    SDL_bool    loop;
    
    loop = SDL_TRUE;
    if (win)
    {
        while (loop)
        {   
            clear_rend(win->rend, 197, 50, 250);
            SDL_PumpEvents();
			SDL_PollEvent(&event);
            SDL_RenderCopy(win->rend, define_credit(win), NULL, &(SDL_Rect){(500), (400), (250), (75)});
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
                loop = SDL_FALSE;
            SDL_RenderPresent(win->rend);
        }
    }
}