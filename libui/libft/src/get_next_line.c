/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/26 21:57:26 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 16:14:04 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/get_next_line.h"

static int		ft_findn(char *str)
{
	int	count;

	count = 0;
	while (str[count] != '\n' && str[count])
		count++;
	return ((str[count] ? count : -1));
}

static t_list	*ft_checkfd(t_list *first, int fd)
{
	t_list *file;

	file = first;
	while (file)
	{
		if (file->content_size == (size_t)fd)
			return (file);
		file = file->next;
	}
	file = ft_lstnew("\0", fd);
	ft_lstaddend(&first, file);
	return (file);
}

static void		ft_setline(t_list *file, char **line)
{
	char *tmp;

	if (ft_strchr(file->content, '\n'))
	{
		*line = ft_strsub(file->content, 0, ft_findn(file->content));
		tmp = ft_strsub(file->content, ft_findn(file->content) + 1,
		ft_strlen(file->content) - ft_findn(file->content));
		free(file->content);
		file->content = tmp;
	}
	else
	{
		*line = ft_strdup(file->content);
		free(file->content);
		file->content = "\0";
	}
}

int				get_next_line(const int fd, char **line)
{
	char			buff[BUFF_SIZE + 1];
	static t_list	*first;
	t_list			*file;
	int				ret;
	char			*tmp;

	if (fd < 0 || BUFF_SIZE < 0 || read(fd, buff, 0) == -1 || !line)
		return (-1);
	if (!first)
		first = ft_lstnew("\0", fd);
	file = ft_checkfd(first, fd);
	while (!ft_strchr(file->content, '\n') &&
		(ret = read(file->content_size, buff, BUFF_SIZE)) > 0)
	{
		buff[ret] = '\0';
		tmp = ft_strjoin(file->content, buff);
		free(file->content);
		file->content = tmp;
	}
	if (ft_strlen(file->content))
		ft_setline(file, line);
	else
		return (0);
	return (1);
}
