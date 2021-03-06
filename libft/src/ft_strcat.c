/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/03 10:46:51 by gmonacho          #+#    #+#             */
/*   Updated: 2020/05/17 20:07:56 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strcat(char *s1, const char *s2)
{
	int		i;

	if (!s2)
		return (s1);
	if (!s1)
		return ((char *)s2);
	i = 0;
	while (s1[i])
		i++;
	while (*s2)
		s1[i++] = *s2++;
	s1[i] = '\0';
	return (s1);
}
