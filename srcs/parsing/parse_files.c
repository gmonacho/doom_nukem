/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widrye <widrye@student.le-101.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 12:29:01 by widrye            #+#    #+#             */
/*   Updated: 2020/03/08 12:57:02 by widrye           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int			write_tmp_file(int fd, char *texname, size_t bytes, char *dir)
{
	int		fd_tex;
	char	*tmp;
	int		n;

	if (!(tmp = (char *)malloc(sizeof(char) * bytes)))
		return (0);
	fd_tex = open(ft_strjoin(dir, texname),
	O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, 0777);
	n = read(fd, tmp, bytes);
	write(fd_tex, tmp, n);
	return (1);
}

int			create_tmp_files(int fd, char *dir)
{
	char		tmp[1];
	t_fdot_3d	index;
	char		texname[BUFSIZ];
	char		texlen[BUFSIZ];

	index.x = 0;
	index.y = 0;
	index.z = 0;
	ft_bzero(texname, BUFSIZ);
	ft_bzero(texlen, BUFSIZ);
	while ((read(fd, tmp, 1)) && (int)index.x < 3)
	{
		if (*tmp == '\n')
		{
			if ((int)index.x == 1)
			{
				texname[(int)index.y] = '\0';
				if (!ft_strcmp(texname, "Sounds") ||
				!ft_strcmp(texname, "Fonts") ||
				!ft_strcmp(texname, "###########"))
					return (1);
			}
			if ((int)index.x == 2)
			{
				texlen[(int)index.z] = '\0';
				return ((!write_tmp_file(fd, ft_strdup(texname),
				ft_atoi(texlen), dir) ||
				!create_tmp_files(fd, dir)) ? 0 : 1);
			}
			index.x++;
		}
		if ((int)index.x == 1 && *tmp != '\n')
			texname[(int)index.y++] = *tmp;
		if ((int)index.x == 2 && *tmp != '\n')
			texlen[(int)index.z++] = *tmp;
	}
	return (1);
}
