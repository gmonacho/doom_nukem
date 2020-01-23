/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ui_draw_text_line_return.c                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <gmonacho@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/21 17:35:58 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 17:36:00 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ui_texture.h"
#include "ui_error.h"
#include "libft.h"

static char		*jump_n_words(const char *str, int n)
{
	int			i;
	int			i_words;

	i_words = 0;
	i = 0;
	if (str)
	{
		while (str[i] && i_words < n)
		{
			if (str[i] != ' ')
			{
				i_words++;
				while (str[i] && str[i] != ' ')
					i++;
			}
			while (str[i] && str[i] == ' ')
				i++;
		}
		if (str[i])
			return (ft_strdup(&str[i]));
	}
	return (NULL);
}

int				ui_draw_text_line_return(SDL_Renderer *rend,
								t_text_line_kit *text)
{
	int				n_rem_words;
	int				n_words;
	int				jumped;
	t_text_line_kit	text_tmp;

	if (text)
	{
		n_rem_words = 0;
		n_words = ft_count_words(text->str);
		text_tmp = *text;
		while (text_tmp.str && jumped != -1 && n_rem_words < n_words)
		{
			jumped = draw_one_line_return(rend, &text_tmp,
									n_rem_words, n_words);
			text_tmp.pos.y += text_tmp.height;
			text_tmp.str = jump_n_words(text_tmp.str, jumped);
			n_rem_words += jumped;
		}
	}
	return (1);
}
