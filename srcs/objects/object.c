/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/07 22:12:26 by agiordan          #+#    #+#             */
/*   Updated: 2020/05/04 21:00:55 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	add_existing_object(t_object **objects, t_object *new_object)
{
	if (new_object)
	{
		new_object->next = *objects;
		*objects = new_object;
	}
}
