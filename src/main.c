#include "SDL.h"
#include "ret_error.h"
#include "display.h"
#include "loop.h"
#include <fcntl.h>

int     main(int argc, char **argv)
{
    t_win win;
    int fd;
    int fd1;

    argc = 1;
    if ((((fd = open(argv[1] , O_RDONLY)) <= 0) || ((fd1 = open(argv[1], O_RDONLY)) <= 0)))
        return (-1);
    ft_data_storing(fd, fd1);
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return (ret_error(SDL_GetError()));
    if (!(create_window(&win, "doom_nukem", (SDL_Rect){200, 200, 2000, 1200}, SDL_WINDOW_SHOWN)))
        return (0);
    SDL_SetRenderDrawColor(win.rend, 255, 255, 255, 255);
    //editor_loop(&win);
    //game_loop(&win);
    SDL_DestroyWindow(win.ptr);
    SDL_DestroyRenderer(win.rend);
    SDL_Quit();
    return (0);
}