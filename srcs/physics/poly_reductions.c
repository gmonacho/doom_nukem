/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   poly_reductions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agiordan <agiordan@student.le-101.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 17:24:06 by agiordan          #+#    #+#             */
/*   Updated: 2020/03/05 18:45:10 by agiordan         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

//Fonction de debug a supprimer avant push

void	print_poly(t_poly *poly, int arg)
{
	int	i = -1;

	printf("Poly %d\n", poly->index);
	if (arg == 0)
	{
		while (++i < N_DOTS_POLY)
			printf("D%d (%f, %f, %f)\n", i, poly->dots[i].x, poly->dots[i].y, poly->dots[i].z);
	}
	else if (arg == 1)
	{
		while (++i < N_DOTS_POLY)
			printf("D%d (%f, %f, %f)\n", i, poly->dots_rotz_only[i].x, poly->dots_rotz_only[i].y, poly->dots_rotz_only[i].z);
	}
	else if (arg == 2)
	{
		while (++i < poly->n_dot)
			printf("D%d (%f, %f, %f)\n", i, poly->dots_new[i].x, poly->dots_new[i].y, poly->dots_new[i].z);
	}
	else
		while (++i < poly->n_proj)
			printf("D%d (%d, %d)\n", i, poly->dots_proj[i].x, poly->dots_proj[i].y);
	printf("\n");
}
