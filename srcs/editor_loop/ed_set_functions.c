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

void		set_str_value(void *argument, char *button_output)
{
	if (*((char**)argument))
		ft_strdel((char**)argument);
	*((char**)argument) = ft_strdup(button_output);
}
