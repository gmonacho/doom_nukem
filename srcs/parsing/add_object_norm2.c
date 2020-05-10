/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object_norm2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 18:33:57 by gal               #+#    #+#             */
/*   Updated: 2020/05/10 18:38:13 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		add_poly_object_norm2(t_object *object, char *type_str)
{
	if (!ft_strcmp(type_str, "TP"))
		object->type = TP;
	else if (!ft_strcmp(type_str, "ARMOR"))
		object->type = ARMOR;
	else if (!ft_strcmp(type_str, "GUN"))
		object->type = GUN;
	else if (!ft_strcmp(type_str, "BULLET"))
		object->type = BULLET;
	else if (!ft_strcmp(type_str, "GRAVITY_INV"))
		object->type = GRAVITY_INV;
	else if (!ft_strcmp(type_str, "LIGHT"))
		object->type = LIGHT;
	if (!ft_strcmp(type_str, "HEAL"))
		object->type = HEAL;
	return (0);
}
