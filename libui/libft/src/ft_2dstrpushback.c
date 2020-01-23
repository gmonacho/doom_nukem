/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_2dstrpushback.c                               .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/21 16:05:08 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 16:06:06 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <stdio.h>

char	**ft_2dstrpushback(char **tab, int size_i, const char *line)
{
	char	**ntab;
	int		i;

	if (!(ntab = (char**)ft_memalloc(sizeof(char*) * (size_i + 2))))
		return (NULL);
	i = 0;
	if (tab)
	{
		while (i < size_i)
		{
			if (!(ntab[i] = ft_strdup(tab[i])))
				return (NULL);
			i++;
		}
	}
	if (!(ntab[i++] = ft_strdup(line)))
		return (NULL);
	ntab[i] = NULL;
	return (ntab);
}
