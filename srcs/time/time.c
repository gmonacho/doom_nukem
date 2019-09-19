#include "doom_nukem.h"

void    start_cooldown(t_timer *timer, uint32_t time)
{   
    timer->save = SDL_GetTicks();
    timer->time = time;
}

int     test_timer(t_timer *timer)
{   
    if (SDL_GetTicks() > timer->save + timer->time)
    {   
        timer->save = SDL_GetTicks();
        return (1);
    }

    else
        return (0);
}

void    reload_cd(t_map *map)
{   
    int i;

    i = -1;
    if (test_timer(&(map->player.timers.shot_cd)) == 1 && map->player.timers.bullet_cd.index < 5)
		map->player.timers.bullet_cd.index++;
	if (test_timer(&(map->player.timers.animation_shot_cd)) == 1 && map->player.timers.animation_shot_cd.index < 5)
    {   
        if (map->player.bullet_drop)
        {   
            while (++i < map->player.len_bullet)    //Augmente chaque compteur
                (map->player.bullet_drop[i])++;
            if (map->player.bullet_drop[0] == 5)    //Si le premier arrive a la fin le supprime
                del_bullet(&map->player);
        }
    }
	if (test_timer(&(map->player.timers.reload_cd)) == 1 && map->player.timers.reload_cd.index < 4)
        map->player.timers.reload_cd.index++;
	if (map->player.timers.reload_cd.index == 4)
		map->player.inventory->weapon = 1;
    if (test_timer(&(map->player.timers.mouse)) == 1)
       map->player.timers.mouse.index = 0;


}

// void    init_cd(t_map *map)
// {
//     start_cooldown(&(map->player.timers.bullet_cd), 150);
// 	start_cooldown(&(map->player.timers.item_cd), 200);
// 	start_cooldown(&(map->player.timers.text_cd), 600);
// 	start_cooldown(&(map->player.timers.reload_cd), 600);
// 	start_cooldown(&(map->player.timers.animation_cd), 1000);
// 	start_cooldown(&(map->player.timers.shot_cd), 50);
// 	start_cooldown(&(map->player.timers.animation_shot_cd), 50);
//     map->player.timers.reload_cd.index = 4;
//     map->player.timers.bullet_cd.index = 5;
// 	define_line_shot(map->player);
// }