/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ed_export_sounds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: widrye <widrye@student.le-101.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 19:35:55 by widrye            #+#    #+#             */
/*   Updated: 2020/03/08 12:37:14 by widrye           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		ed_write_sounds(int fd, char *name)
{
	static char	tmp[BUFSIZ];
	int			fd_sound;
	int			n;
	int			bytes;

	fd_sound = open(ft_strjoin("sounds/", name), O_RDONLY);
	if (fd_sound < 0)
		return (0);
	ft_putchar_fd('\n', fd);
	ft_putendl_fd(name, fd);
	bytes = 0;
	while ((n = read(fd_sound, tmp, sizeof(tmp))))
		bytes += n;
	ft_putendl_fd(ft_itoa(bytes), fd);
	fd_sound = open(ft_strjoin("sounds/", name), O_RDONLY);
	while ((n = read(fd_sound, tmp, sizeof(tmp))))
		write(fd, tmp, n);
	return (1);
}

int		ed_export_sounds(int fd)
{
	if (!ed_write_sounds(fd, "shotGun.mp3"))
		return (0);
	if (!ed_write_sounds(fd, "reload.mp3"))
		return (0);
	if (!ed_write_sounds(fd, "healsound.mp3"))
		return (0);
	if (!ed_write_sounds(fd, "shieldsound.mp3"))
		return (0);
	if (!ed_write_sounds(fd, "slotsound.mp3"))
		return (0);
	if (!ed_write_sounds(fd, "damageSound.mp3"))
		return (0);
	if (!ed_write_sounds(fd, "map_editor.wav"))
		return (0);
	if (!ed_write_sounds(fd, "Remember_chill.mp3"))
		return (0);
	if (!ed_write_sounds(fd, "doomMenu.wav"))
		return (0);
	return (1);
}
