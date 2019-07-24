#include "doom_nukem.h"

int     update_text_entry_texture(t_win *win, t_button *button, const char *text)
{
	SDL_Texture		*texture;
	int				w;
	int				h;
	t_text_entry	*data;

	data = button->data;
	SDL_QueryTexture(button->texture, NULL, NULL, &w, &h);
	SDL_DestroyTexture(button->texture);
	if (!(texture = generate_text(win->rend, win->font, data->name, (SDL_Color){200, 200, 200, 255})))
		return (ret_error("text generation failed in update_text_entry_texture"));
	if (!(button->texture = blit_text(win->rend, win->text_entry_texture, texture, &(SDL_Rect){w * 0.03,
																							h * 0.2,
																							w * 0.24,
																							h * 0.7})))
		return (ret_error("blit_text failed in update_text_entry_texture"));
	SDL_DestroyTexture(texture);
	if (text)
	{
		if (!(texture = generate_text(win->rend, win->font,
				text, (SDL_Color){225, 225, 225, 255})))
			return (ret_error("text generation 2 failed in update_text_entry_texture"));
		if (!(button->texture = blit_text(win->rend, button->texture, texture, &(SDL_Rect){w * 0.45,
																						h * 0.2,
																						w * 0.4,
																						h * 0.7})))
			return (ret_error("blit_text failed in update_text_entry_texture"));
		SDL_DestroyTexture(texture);
	}
	return (1);
}

static void	set_selected_portal_id(t_map_editor *map, int id)
{
	t_linedef		*l;

	l = NULL;
	if (map->selected_sector)
		l = map->selected_sector->lines;
	while (l)
	{
		if (l->flags & LINEDEF_SELECTED && l->gflags & PORTAL)
			l->id = id;
		l = l->next;
	}
}
 
int		fill_variable(t_win *win, t_map_editor *map, t_button *button, const void *result)
{
	t_text_entry	*data;
	int				*int_variable;
	char			*str_variable;

	data = button->data;
	if (win)
	{
		if (data->variable)
		{
			if (data->flags & TEXT_ENTRY_DIGITAL)
			{
				int_variable = data->variable;
				*int_variable = *((int*)result);
				printf("*int_variable = %d\n", *int_variable);
			}			
			else if (data->flags & TEXT_ENTRY_ALPHANUM)
			{
				str_variable = data->variable;
				ft_strcpy(str_variable, (char*)result);
			}
			// data->variable = NULL;
		}
		else
		{
			if (button->flags & BUTTON_ID)
				set_selected_portal_id(map, *((int*)result));
		}
		if (data->flags & TEXT_ENTRY_TMP)
		{
			if (!update_text_entry_texture(win, button, NULL))
				return (ret_error("update_text_entry_texture failed in fill_variable"));
		}
	}
	return (1);
}