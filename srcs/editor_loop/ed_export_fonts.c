/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ed_export_fonts.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widrye <widrye@student.le-101.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 06:52:40 by widrye            #+#    #+#             */
/*   Updated: 2020/03/08 12:05:53 by widrye           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int     ed_write_fonts(int fd, char *name)
{
	static char tmp[BUFSIZ];
	int fd_font;
	int n;
	int	bytes;

	fd_font = open(ft_strjoin("TTF/", name), O_RDONLY);
	if (fd_font < 0)
		return (0);
	ft_putchar_fd('\n', fd);
	ft_putendl_fd(name, fd);
	bytes = 0;
	while ((n = read(fd_font, tmp, sizeof(tmp))))
		bytes += n;
	ft_putendl_fd(ft_itoa(bytes), fd);
	fd_font = open(ft_strjoin("TTF/", name), O_RDONLY);
	while ((n = read(fd_font, tmp, sizeof(tmp))))
		write(fd, tmp, n);
	return (1);
}

int     ed_export_fonts(int fd)
{
	if (!ed_write_fonts(fd, "arial.ttf"))
		return (0);
	if (!ed_write_fonts(fd, "DooM.ttf"))
		return (0);
	if (!ed_write_fonts(fd, "Raleway-Regular.ttf"))
		return (0);
    printf("DONE EXPORTING FONTS\n");
	return (1);
}
