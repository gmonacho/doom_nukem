
#include "libft.h"
#include <fcntl.h>
#include "data.h"
#include "ret_error.h"

void        add_sectors(t_sector **sectors, t_sector *new_sectors)
{
    new_sectors->next = *sectors;
    *sectors = new_sectors;
}

t_sector    *new_sector()
{   
    t_sector *sector;

    if (!(sector = (t_sector *)ft_memalloc(sizeof (t_sector))))
        exit(0);
    sector->floor_height = 0;
    sector->ceil_height = 0;
    sector->light_level = 0;
    sector->floor_texture = NULL;
    sector->ceil_texture = NULL;
    sector->lines = NULL;
    sector->next = NULL;
    return (sector);
}

int     count_line(int fd1)
{   
    int nb;
    char *line;

    nb = 0;
    while (get_next_line(fd1, &line) > 0)
    {   
        free(line);
        nb++;
    }
    return (nb);
}
char    **ft_fill_map(int fd, int fd1)
{
    char *line;
    char **tab;
    int i;
    int nb;

    i = 0;
    nb = count_line(fd1);
    if (!(tab = (char **)malloc(sizeof(char *) * (nb + 1))))
        return (NULL);
    while (get_next_line(fd, &line) > 0)
    {
        tab[i] = ft_strdup(line);
        //printf("tab[%d] = %s\n",i ,tab[i]);
        free(line);
        printf("antoine la grosse salope\n");
        i++;
    }
    tab[i] = NULL;
    return (tab);
}

int    ft_data_storing(int fd, int fd1)
{
    char **tab;
    int i;
    t_sector *sector;
    int y;
    int comptor;
    char tmp[5];

    i = 0;
    y = 0;
    comptor = 0;
    sector = NULL;
    tmp[4] = '\0';
    tab = ft_fill_map(fd, fd1);
    while (tab[i])
    {   
        if (ft_strstr(tab[i], "Sector"))
        {
            while ((ft_strchr(tab[i], '}') == NULL || ft_strchr(tab[i - 1], '}') == NULL))
            {   
                add_sectors(&sector, new_sector());
                if (ft_strstr(tab[i], "floorHeight"))
                    {   
                        y = 0;
                        comptor = 0;
                        while (tab[i][y])
                        {   
                            if (ft_isdigit(tab[i][y]))
                            {
                                tmp[comptor] = tab[i][y];
                                comptor++; 
                            }
                            //printf("tmp = %s\n",tmp);
                            y++;
                        }
                        //printf("tmp 2= %s\n",tmp);
                        sector->floor_height = ft_atoi(tmp);
                        //printf("floor = %d\n", sector->floor_height);
                    }
                i++;
            }
        }
        i++;
    }
    return (0);
}
