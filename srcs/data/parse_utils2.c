#include "libft.h"
#include <fcntl.h>
#include "data.h"
#include "ret_error.h"

t_linedef	*new_void_linedef(void)
{
	t_linedef	*line;

	if (!(line = (t_linedef *)ft_memalloc(sizeof(t_linedef))))
		exit(0);
	return (line);
}
