#include "doom_nukem.h"

t_button	*new_button(const t_frect ratio, SDL_Texture *texture, Uint32 button_flags)
{
	t_button	*button;

	if (!(button = (t_button*)ft_memalloc(sizeof(t_button))))
		return (ret_null_perror("button allocation failed in new_button"));
	button->ratio = ratio;
	button->texture = texture;
	button->flags = button_flags;
	button->text = NULL;
	button->data = NULL;
	button->next = NULL;
	return (button);
}

void		add_button(t_button **buttons, t_button *new_button)
{
	new_button->next = *buttons;
	*buttons = new_button;
}

void	remove_button(t_button **button, t_button *button_del)
{
	t_button	*b;
	t_button	*tmp;

	b = *button;
	if (b != button_del)
	{
		while (b && b->next != button_del)
			b = b->next;
	}
	if (b)
	{
		tmp = b->next;
		b->next = tmp->next;
		free_button(tmp);
	}
}

void	free_button(t_button *button)
{
	if (button)
	{
		SDL_DestroyTexture(button->texture);
		free(button);
	}
	button = NULL;
}

void		free_buttons(t_button **buttons)
{
	t_button *tmp_to_next;
	t_button *b;

	b = *buttons;
	while (b)
	{
		tmp_to_next = b->next;
		free_button(b);
		b = tmp_to_next;
	}
	*buttons = NULL;
}


int			get_nb_buttons(t_button **buttons)
{
	t_button	*b;
	int			nb_buttons;

	b = *buttons;
	nb_buttons = 0;
	while (b)
	{
		b = b->next;
		nb_buttons++;
	}
	return (nb_buttons);
}

t_button		*get_button_by_flags(t_button **buttons, Uint32 flags)
{
	t_button	*b;

	b = *buttons;
	while (b)
	{
		if (b->flags & flags)
			return (b);
		b = b->next;
	}
	return (NULL);
}

t_text_entry	*new_text_entry(char *name, int max_size, void *variable, Uint8 flags)
{
	t_text_entry 	*text_entry;

	if (!(text_entry = (t_text_entry*)ft_memalloc(sizeof(t_text_entry))))
		return (ret_null_error("text_entry allocation failed in new_text_entry"));
	text_entry->name = name;
	text_entry->max_size = max_size;
	text_entry->variable = variable;
	text_entry->flags = flags;
	return (text_entry);
}

t_simple_button	*new_simple_button(char *name, SDL_bool clicked)
{
	t_simple_button	*simple_button;

	if (!(simple_button = (t_simple_button*)ft_memalloc(sizeof(t_simple_button))))
		return (ret_null_error("simple_button allocation failed in new_simple_button"));
	simple_button->name = name;
	simple_button->clicked = clicked;
	return (simple_button);
}