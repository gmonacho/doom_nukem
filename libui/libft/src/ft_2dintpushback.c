/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_2dintpushback.c                               .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/08 11:44:16 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 16:11:10 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <stdio.h>

static int	*get_int_tab_cpy(int **ddtab, int **ntab, int size_j, int i)
{
	int		*tab;
	int		j;

	j = 0;
	if (!(tab = (int*)ft_memalloc(sizeof(int) * size_j)))
		return (NULL);
	while (j < size_j)
	{
		ntab[i][j] = ddtab[i][j];
		j++;
	}
	return (tab);
}

int			**ft_2dintpushback(int **tab, int size_i, int size_j, int *line)
{
	int	**ntab;
	int	i;
	int j;

	if (!(ntab = (int**)ft_memalloc(sizeof(int*) * (size_i + 1))))
		return (NULL);
	i = 0;
	while (i < size_i)
	{
		if (!(ntab[i] = get_int_tab_cpy(tab, ntab, size_j, i)))
			return (NULL);
		i++;
	}
	if (!(ntab[i] = (int*)ft_memalloc(sizeof(int) * size_j)))
		return (NULL);
	j = 0;
	while (j < size_j)
	{
		ntab[i][j] = line[j];
		j++;
	}
	return (ntab);
}
