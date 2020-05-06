/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 14:19:07 by gal               #+#    #+#             */
/*   Updated: 2020/05/06 14:19:08 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "SDL.h"
#include "ui.h"
#include "ui_error.h"

int			ui_init(Uint32 sdl_flags)
{
	int		ret;

	if ((ret = SDL_Init(sdl_flags)) < 0)
		return (ui_ret_error("ui_init", SDL_GetError(), ret));
	if (TTF_Init() == -1)
		return (ui_ret_error("ui_init", TTF_GetError(), -1));
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	return (1);
}

void		ui_quit(Uint32 flags)
{
	TTF_Quit();
	SDL_QuitSubSystem(flags);
	SDL_Quit();
}
