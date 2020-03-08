
#include "doom_nukem.h"

int		write_tmp_file(int fd, char *texname, size_t bytes, char *dir)
{
	int			fd_tex;
	char		*tmp;
	int			n;

	if (!(tmp = (char *)malloc(sizeof(char) * bytes)))
		return (0);
	fd_tex = open(ft_strjoin(dir, texname), O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0777);
	n = read(fd, tmp, bytes);
	write(fd_tex, tmp, n);
	return (1);
}

int		create_tmp_files(int fd, char *dir)
{
	size_t	n;
	char	tmp[1];
	int		j;
	int		k;
	int		i;
	char texname[BUFSIZ];
	char texlen[BUFSIZ];
	int ret;
	
	j = 0;
	i = 0;
	k = 0;
	ret = 0;
	ft_bzero(texname, BUFSIZ);
	ft_bzero(texlen, BUFSIZ);
	while ((n = read(fd, tmp, 1)) && i < 3)
	{
		if (*tmp == '\n')
		{
			if (i == 1)
			{
				texname[j] = '\0';
				if (!ft_strcmp(texname, "Sounds") || !ft_strcmp(texname, "###########"))
					return (1);
			}
			if (i == 2)
			{
				texlen[k] = '\0';
				return ((!write_tmp_file(fd, ft_strdup(texname), ft_atoi(texlen), dir) ||
					!create_tmp_files(fd, dir)) ? 0 : 1);
			}
			i++;
		}
		if (i == 1 && *tmp != '\n')
			texname[j++] = *tmp;
		if (i == 2 && *tmp != '\n')
			texlen[k++] = *tmp;
	}
	return (1);
}
