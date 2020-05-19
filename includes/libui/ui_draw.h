/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_draw.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gal <gal@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 18:17:07 by gmonacho          #+#    #+#             */
/*   Updated: 2020/05/19 09:27:42 by gal              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_DRAW_H
# define UI_DRAW_H

# include "ui_shape.h"
# include "SDL.h"

void	ui_set_draw_color(SDL_Renderer *rend, const SDL_Color *color);
void	ui_draw_point(SDL_Renderer *rend, const t_dot *dot);
void	ui_draw_line(SDL_Renderer *rend, const t_line *line);
void	ui_draw_rect(SDL_Renderer *rend, const t_rect *rect);

#endif
