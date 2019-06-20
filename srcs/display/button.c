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

void		free_buttons(t_button **buttons)
{
	t_button *tmp_to_next;
	t_button *b;

	b = *buttons;
	while (b)
	{
		tmp_to_next = b->next;
		SDL_DestroyTexture(b->texture);
		free(b);
		b = tmp_to_next;
	}
	*buttons = NULL;
}

t_text_entry	*new_text_entry(char *name, int max_size, void *variable, Uint8 flags)
{
	t_text_entry 	*text_entry;

	if (!(text_entry = (t_text_entry*)ft_memalloc(sizeof(t_text_entry))))
		return (ret_null_error("text_entry allocation failed in t_text_entry"));
	text_entry->name = name;
	text_entry->max_size = max_size;
	text_entry->variable = variable;
	text_entry->flags = flags;
	return (text_entry);	
}