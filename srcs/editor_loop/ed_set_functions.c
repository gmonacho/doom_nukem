#include "doom_nukem.h"


void		set_int_value(void *argument, char *button_output)
{
	*((int*)argument) = ft_atoi(button_output);
}

void		set_float_value(void *argument, char *button_output)
{
	*((float*)argument) = ft_atoi(button_output);
}

void		set_coef_value(void *argument, char *button_output)
{
	*((float*)argument) = ft_atoi(button_output) / 100.f;
}

void			set_wall_z1(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[0].z = ft_atoi(button_output);
	poly->dots[1].z = ft_atoi(button_output);
}

void			set_wall_z2(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[2].z = ft_atoi(button_output);
	poly->dots[3].z = ft_atoi(button_output);
}


void			set_flat_z(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[0].z = ft_atoi(button_output);
	poly->dots[1].z = ft_atoi(button_output);
	poly->dots[2].z = ft_atoi(button_output);
	poly->dots[3].z = ft_atoi(button_output);
}

void			set_inclined_z1(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[0].z = ft_atoi(button_output);
	poly->dots[3].z = ft_atoi(button_output);
}

void			set_inclined_z2(void *argument, char *button_output)
{
	t_poly	*poly;

	poly = argument;
	poly->dots[1].z = ft_atoi(button_output);
	poly->dots[2].z = ft_atoi(button_output);
}

void		set_str_value(void *argument, char *button_output)
{
	if (*((char**)argument))
		ft_strdel((char**)argument);
	*((char**)argument) = ft_strdup(button_output);
}