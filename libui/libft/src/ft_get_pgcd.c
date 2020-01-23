/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_get_pgcd.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/21 16:04:19 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 16:04:40 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/libft.h"

int		ft_get_pgcd(int a, int b)
{
	int	reste;

	if (b > a)
		ft_swap(&a, &b);
	while ((reste = a % b) != 0)
	{
		a = b;
		b = reste;
	}
	return (b);
}
