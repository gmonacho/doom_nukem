/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/03 13:48:37 by gmonacho          #+#    #+#             */
/*   Updated: 2020/05/10 22:21:52 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

int		ft_isprint(int c)
{
	if (c > 32 && c <= 126)
		return (1);
	else
		return (0);
}
