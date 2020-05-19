/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_event.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 17:33:50 by gmonacho          #+#    #+#             */
/*   Updated: 2020/05/19 12:05:27 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui_event.h"

void	ui_poll_event(SDL_Event *event)
{
	while (SDL_PollEvent(event))
		event = event;
}

void	ui_wait_event(SDL_Event *event)
{
	SDL_WaitEvent(event);
}
