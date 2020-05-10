/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_2dputint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 12:35:00 by gmonacho          #+#    #+#             */
/*   Updated: 2020/05/10 22:20:11 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <stdio.h>

void	ft_2dputint(int **tab, int ilen, int jlen)
{
	int		i;
	int		j;

	if (tab)
	{
		i = 0;
		while (i < ilen)
		{
			j = 0;
			while (j < jlen)
			{
				ft_putnbr(tab[i][j++]);
				ft_putchar(' ');
			}
			ft_putchar('\n');
			i++;
		}
	}
}
