/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_2dintpushback.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 11:44:16 by gmonacho          #+#    #+#             */
/*   Updated: 2020/05/10 22:20:03 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <stdio.h>

int		**ft_2dintpushback(int **tab, int size_i, int size_j, int *line)
{
	int	**ntab;
	int	i;
	int j;

	if (!(ntab = (int**)ft_memalloc(sizeof(int*) * (size_i + 1))))
		return (NULL);
	i = 0;
	while (i < size_i)
	{
		j = 0;
		if (!(ntab[i] = (int*)ft_memalloc(sizeof(int) * size_j)))
			return (NULL);
		while (j < size_j)
		{
			ntab[i][j] = tab[i][j];
			j++;
		}
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
