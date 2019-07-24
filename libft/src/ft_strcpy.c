/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strcpy.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 10:21:37 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/03 10:40:22 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../includes/libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	int		i;

	i = 0;
	if (src)
	{
		printf("strcpy\nsrc = %s dst = %s p = %p\n", src, dst, dst);
		while (src[i])
		{
			printf("i = %d dst[i] = %c src[i] = %c\n", i, dst[i], src[i]);
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
		printf("--ok--\n");
	}
	return (dst);
}
