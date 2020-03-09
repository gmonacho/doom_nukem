#include "doom_nukem.h"

static void	ed_set_first_properties(t_win *win, t_mob *selected)
{
	char	*tmp;

	ed_set_property(win, &(t_property){"b_property_1", "texture",
							&set_str_value, &selected->texture,
							selected->texture});
	tmp = ft_itoa(selected->pos.z);
	ed_set_property(win, &(t_property){"b_property_2", "z",
							&set_float_value, &selected->pos.z, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->width);
	ed_set_property(win, &(t_property){"b_property_3", "width",
							&set_int_value, &selected->width, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->height);
	ed_set_property(win, &(t_property){"b_property_4", "height",
							&set_int_value, &selected->height, tmp});
	ft_strdel(&tmp);
}

void		ed_set_buttons_mob(t_win *win, t_mob *selected)
{
	char				*tmp;

	ed_set_first_properties(win, selected);
	tmp = ft_itoa(selected->health);
	ed_set_property(win, &(t_property){"b_property_5", "health",
							&set_int_value, &selected->health, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->damage);
	ed_set_property(win, &(t_property){"b_property_6", "damage",
							&set_int_value, &selected->damage, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->vel);
	ed_set_property(win, &(t_property){"b_property_7", "velocity",
							&set_int_value, &selected->vel, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->light_coef * 100);
	ed_set_property(win, &(t_property){"b_property_8", "light %",
							&set_coef_value, &selected->light_coef, tmp});
	ft_strdel(&tmp);
}
