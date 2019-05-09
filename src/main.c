#include "SDL.h"
#include "SDL_ttf.h"
#include "ret_error.h"
#include "loop.h"
#include "display.h"
#include <fcntl.h>

int     main()
{
    t_win win;
    //if ((((fd = open(argv[1] , O_RDONLY)) <= 0) || ((fd1 = open(argv[1], O_RDONLY)) <= 0)))
    //    return (-1);
    //ft_data_storing(fd, fd1);
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() == -1)
        return (ret_error(SDL_GetError()));
    if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 200, 2000, 1200}, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)))
        return (0);
    SDL_SetRenderDrawColor(win.rend, 255, 255, 255, 255);
    editor_loop(&win);
    //game_loop(&win);
    SDL_DestroyWindow(win.ptr);
    SDL_DestroyRenderer(win.rend);
    SDL_Quit();
    return (0);
}