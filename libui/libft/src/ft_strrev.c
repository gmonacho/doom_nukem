/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strrev.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/21 16:13:02 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 16:13:39 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strrev(const char *str)
{
	char	*str_rev;
	int		i;
	int		i_rev;

	if (str)
	{
		if (!(str_rev = (char*)ft_memalloc(
		sizeof(char) * (ft_strlen(str) + 1))))
			return (NULL);
		i = ft_strlen(str) - 1;
		i_rev = 0;
		while (i >= 0)
			str_rev[i_rev++] = str[i--];
		return (str_rev);
	}
	return (NULL);
}
