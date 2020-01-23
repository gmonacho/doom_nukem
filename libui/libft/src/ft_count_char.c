/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_count_char.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/21 16:11:31 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 16:11:47 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/libft.h"

int	ft_count_char(char *str, int c)
{
	int	i;
	int	nb_char;

	i = 0;
	nb_char = 0;
	while (str[i])
	{
		if (str[i] == c)
			nb_char++;
		i++;
	}
	return (nb_char);
}
