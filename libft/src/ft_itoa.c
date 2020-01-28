/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_itoa.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/08 17:00:49 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/27 16:45:49 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <stdio.h>

static size_t	ft_nblen(int n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char		*ft_itoa(int n)
{
	char	*str;
	int		i;

	i = ft_nblen(n) + (n < 0 ? 1 : 0);
	if (!(str = ft_strnew(i)))
		return (0);
	if (n == -2147483648)
	{
		str[i--] = '8';
		n = -214748364;
	}
	if (n < 0)
	{
		n *= -1;
		str[0] = '-';
	}
	while (n)
	{
		str[--i] = n % 10 + '0';
		n = (n - n % 10) / 10;
	}
	return (str);
}