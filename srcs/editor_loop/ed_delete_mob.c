#include "doom_nukem.h"

static void	ed_delete_mob_link(t_mob **mobs,
								t_mob **mob,
								t_mob **tmp_prev,
								t_mob **tmp_next)
{
	*tmp_next = (*mob)->next;
	free(*mob);
	if (*tmp_prev)
		(*tmp_prev)->next = *tmp_next;
	else
		*mobs = *tmp_next;
	*mob = *tmp_next;
}

void		ed_delete_mob(t_mob **mobs, t_mob *mob)
{
	t_mob	*m;
	t_mob	*tmp_prev;
	t_mob	*tmp_next;

	if (mobs && mob)
	{
		tmp_prev = NULL;
		m = *mobs;
		while (m)
		{
			if (m == mob)
				ed_delete_mob_link(mobs, &m, &tmp_prev, &tmp_next);
			else
			{
				tmp_prev = m;
				m = m->next;
			}
		}
	}
}
