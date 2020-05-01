#include "ui_texture.h"
#include "ui_error.h"
#include "libft.h"

int		get_dst_y(t_text_line_kit *text)
{
	if (text->alignment & TEXT_ALIGN_V_MIDDLE)
		return (text->pos.y - text->height / 2);
	else if (text->alignment & TEXT_ALIGN_V_TOP)
		return (text->pos.y - text->height);
	else
		return (text->pos.y);
}
