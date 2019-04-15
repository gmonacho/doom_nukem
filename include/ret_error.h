#ifndef DOOM_NUKEM_H
# define DOOM_NUKEM_H

/*
** ================================== Error Management ==================================
*/

int			ret_error(const char *error_msg);
void		*ret_null_error(const char *error_msg);
int			ret_perror(const char *error_msg);
void		*ret_null_perror(const char *error_msg);

#endif