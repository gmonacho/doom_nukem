#include "doom_nukem.h"

void	ed_set_buttons_wall(t_win *win, t_poly *selected)
{
	char	*tmp;

	ed_set_property(win, &(t_property){"b_property_1", "texture",
							&set_str_value, &selected->texture_name,
							selected->texture_name});
	tmp = ft_itoa(selected->dots[0].z);
	ed_set_property(win, &(t_property){"b_property_2", "z1",
							&set_wall_z1, selected, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->dots[2].z);
	ed_set_property(win, &(t_property){"b_property_3", "z2",
							&set_wall_z2, selected, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->light_coef * 100);
	ed_set_property(win, &(t_property){"b_property_4", "light %",
							&set_coef_value, &selected->light_coef, tmp});
	ft_strdel(&tmp);
	ed_clean_property(win, 5);
}

void	ed_set_buttons_flat(t_win *win, t_poly *selected)
{
	char	*tmp;

	ed_set_property(win, &(t_property){"b_property_1", "texture",
							&set_str_value,
							&selected->texture_name, selected->texture_name});
	tmp = ft_itoa(selected->dots[0].z);
	ed_set_property(win, &(t_property){"b_property_2", "z",
						&set_flat_z, selected, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->light_coef * 100);
	ed_set_property(win, &(t_property){"b_property_3", "light %",
							&set_coef_value, &selected->light_coef, tmp});
	ft_strdel(&tmp);
	ed_clean_property(win, 4);
}

void	ed_set_buttons_inclined(t_win *win, t_poly *selected)
{
	char	*tmp;

	ed_set_property(win, &(t_property){"b_property_1", "texture",
							&set_str_value, &selected->texture_name,
							selected->texture_name});
	tmp = ft_itoa(selected->dots[0].z);
	ed_set_property(win, &(t_property){"b_property_2", "z1",
							&set_inclined_z1, selected, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->dots[1].z);
	ed_set_property(win, &(t_property){"b_property_3", "z2",
							&set_inclined_z2, selected, tmp});
	ft_strdel(&tmp);
	tmp = ft_itoa(selected->light_coef * 100);
	ed_set_property(win, &(t_property){"b_property_4", "light %",
							&set_coef_value, &selected->light_coef, tmp});
	ft_strdel(&tmp);
	ed_clean_property(win, 5);
}
