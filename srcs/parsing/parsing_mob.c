#include "doom_nukem.h"

void    add_mob(t_mob **mob)
{
    t_mob   *new_mob;

    if (!(new_mob = (t_mob *)ft_memalloc(sizeof(t_mob))))
        ft_putendl("error malloc in parsing_mob.c l.8");
    new_mob->next = *mob;
    *mob = new_mob;
    
}

void    fill_mob_data(t_mob **mob, char **tab, int i)
{   
    while (ft_strchr(tab[i], '}') == NULL || ft_strchr(tab[i - 1], '}') == NULL)
    {
        add_mob(mob);
        (*mob)->name = ft_strdup(tab[i]);
        while (!(ft_strchr(tab[i], '}')))
        {
            if (ft_strstr(tab[i], "nbmob = "))
                (*mob)->nmob = ft_atoi(ft_strrchr(tab[i], '=') + 1);
            if (ft_strstr(tab[i], "posx = "))
                (*mob)->pos.x = ft_atoi(ft_strrchr(tab[i], '=') + 1);
            if (ft_strstr(tab[i], "posy = "))
                (*mob)->pos.y = ft_atoi(ft_strrchr(tab[i], '=') + 1);
            if (ft_strstr(tab[i], "sector = "))
                (*mob)->sector = ft_atoi(ft_strrchr(tab[i], '=') + 1);
            if (ft_strstr(tab[i], "live = "))
                (*mob)->live = ft_atoi(ft_strrchr(tab[i], '=') + 1);
            i++;
        }
        i++;
    }
}