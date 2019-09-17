#include "doom_nukem.h"

// static int  update_simple(t_win *win, t_button *b, t_button_state state)
// {
//     SDL_Texture *text;
//     SDL_Texture *bg;
//     int         w;
//     int         h;
//     t_simple_button *data;
    
//     data = (t_simple_button*)b->data;
//     if (!(text = generate_text(win->rend, win->font, data->name,  (SDL_Color){200, 200, 200, 255})))
//         return (ret_error("text generation failed in update_simple"));
//     if (state == BUTTON_STATE_NONE)
//         bg = win->ed_texture.button;
//     else if (state == BUTTON_STATE_ON_MOUSE)
//         bg = win->ed_texture.on_mouse_button;
//     else if (state == BUTTON_STATE_CLICKED)
//         bg = win->ed_texture.clicked_button;
//     SDL_QueryTexture(bg, NULL, NULL, &w, &h);
//     if (!(b->texture = blit_text(win->rend, bg, text, &(SDL_Rect){w / 6, h / 6, w - w / 6 * 2, h - h / 6 * 2})))
//         return (ret_error("blit text failed in update simple"));
// }

void	remove_link_sector_button(t_win *win, t_button **buttons, t_sector *sector)
{
	t_button		*b;
	t_button		*tmp;
	t_simple_button	*data;

	b = *buttons;
	data = (t_simple_button*)b->data;
	while (b && data->link != sector)
	{
		b->ratio.x -= 1.0 / MAX_SECTORS;
		b = b->next;
		data = (t_simple_button*)b->data;
	}
	if (b)
	{
		if (b != *buttons)
		{
			tmp = *buttons;
			while (tmp->next != b)
				tmp = tmp->next;
			tmp->next = b->next;
		}
		else
			*buttons = (*buttons)->next;
		free_button(&b);
		b = *buttons;
		while (b->next)
			b = b->next;
		b->ratio.x -= 1.0 / MAX_SECTORS;
		update_ui_rect(win);
	}
}

int     update_text_entry_texture(t_win *win, t_button *button, const char *text)
{
	SDL_Texture		*texture;
	int				w;
	int				h;
	t_text_entry	*data;

	data = button->data;
	SDL_QueryTexture(button->texture, NULL, NULL, &w, &h);
	SDL_DestroyTexture(button->texture);
	if (!(texture = generate_text(win->rend, win->font.ui, data->name, (SDL_Color){200, 200, 200, 255})))
		return (ret_error("text generation failed in update_text_entry_texture"));
	if (!(button->texture = blit_text(win->rend, win->text_entry_texture, texture, &(SDL_Rect){w * 0.03,
																							h * 0.2,
																							w * 0.24,
																							h * 0.7})))
		return (ret_error("blit_text failed in update_text_entry_texture"));
	SDL_DestroyTexture(texture);
	if (!text)
		text = "_";
	if (!(texture = generate_text(win->rend, win->font.ui,
			text, (SDL_Color){225, 225, 225, 255})))
		return (ret_error("text generation 2 failed in update_text_entry_texture"));
	if (!(button->texture = blit_text(win->rend, button->texture, texture, &(SDL_Rect){w * 0.45,
																					h * 0.2,
																					w * 0.4,
																					h * 0.7})))
		return (ret_error("blit_text failed in update_text_entry_texture"));
	SDL_DestroyTexture(texture);
	return (1);
}

static int  update_simple(t_win *win, t_button *b, t_button_state state)
{
    SDL_Texture     *text;
    SDL_Texture     *bg;
    int             w;
    int             h;
    char            *button_text;
    t_sector        *link;
    t_simple_button *data;

    bg = NULL;
    data = (t_simple_button*)b->data;
    if (data->flags & SIMPLE_BUTTON_NAME)
    {
        link = (t_sector*)data->link;
        button_text = link->name;
    }
    else
        button_text = data->name;
    if (!(text = generate_text(win->rend, win->font.ui, button_text, (SDL_Color){200, 200, 200, 255})))
        return (ret_error("text generation failed in update_simple"));
    if (state == BUTTON_STATE_NONE)
        bg = win->ed_texture.button;
    else if (state == BUTTON_STATE_ON_MOUSE)
        bg = win->ed_texture.on_mouse_button;
    else if (state == BUTTON_STATE_CLICKED)
        bg = win->ed_texture.clicked_button;
    if (SDL_QueryTexture(bg, NULL, NULL, &w, &h) < 0)
        printf("tamere : %s\n", SDL_GetError());
    if (!(b->texture = blit_text(win->rend, bg, text, &(SDL_Rect){w / 6, h / 6, w - w / 6 * 2, h - h / 6 * 2})))
        return (ret_error("blit text failed in update simple"));
    return (1);
}

int     update_button(t_win *win, t_button *b, t_button_state state)
{
    if (b->flags & BUTTON_SIMPLE)
	{
		if (!update_simple(win, b, state))
			return (ret_error("update_button : update_simple failed"));
	}
	// else if (b->flags & BUTTON_TEXT_ENTRY)
	// {
	// 	t_data = (t_text_entry*)b->data;
	// 	if (!update_text_entry_texture(win, b, (char*)t_data->variable))
	// 		return (ret_error("update_button : update_text_entry failed"));
	// }
	return (1);
}