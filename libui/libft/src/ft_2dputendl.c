/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_2dputendl.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/21 16:01:56 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 16:02:07 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../includes/libft.h"

void	ft_2dputendl(const char **str)
{
	int	i;

	i = 0;
	while (str[i])
		ft_putendl(str[i++]);
}
