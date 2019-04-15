#include "SDL.h"
#include "ret_error.h"
#include "display.h"

int     main()
{
    t_win win;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return (ret_error(SDL_GetError()));
    if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 200, 1000, 1000}, SDL_WINDOW_SHOWN)))
        return (0);
    SDL_SetRenderDrawColor(win.rend, 255, 255, 255, 255);
    game_loop(&win);
    SDL_DestroyWindow(win.ptr);
    SDL_DestroyRenderer(win.rend);
    SDL_Quit();
    return (0);
}