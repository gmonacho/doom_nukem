#include "doom_nukem.h"


void		resolve_ui_left_press(t_win *win, t_map_editor *map)
{
	if (win->selected_frame->flags & FRAME_INFO)
	{
		if (win->selected_button)
		{
			if (win->selected_button->flags & BUTTON_TEXT_ENTRY)
			{
				if (win->mouse->x > win->selected_button->rect.x + win->selected_button->rect.w / 3)
				{
					printf("start text_input\n");
					map->flags = MAP_TEXT_EDITING;
					SDL_StartTextInput();
					SDL_SetTextInputRect(&(SDL_Rect){win->selected_button->rect.x + win->selected_button->rect.w / 3,
														win->selected_button->rect.y,
														win->selected_button->rect.w - win->selected_button->rect.w / 3,
														win->selected_button->rect.h});
				}
			}
		}
	}
}