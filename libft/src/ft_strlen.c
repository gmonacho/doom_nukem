/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/02 21:08:00 by gmonacho          #+#    #+#             */
/*   Updated: 2020/05/10 22:24:51 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

size_t		ft_strlen(const char *s)
{
	size_t		len;

	len = 0;
	if (s)
	{
		while (s[len])
			len++;
	}
	return (len);
}
