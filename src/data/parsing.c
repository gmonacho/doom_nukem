
#include "data.h"
#include "libft.h"

void    ft_read_map(t_sector *data, int fd)
{   
    char *line;

    while (get_next_line(fd, line))
}

int main (int argc, char **argv)
{
    int fd;

    if (argc = 2)
    {
        if (fd = open(argv[1], O_RDONLY) <= 0)
            return (-1);
    }
    else
        ft_putendl("pas d'argument");
    return (0);
}