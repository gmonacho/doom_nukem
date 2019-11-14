








static t_fdot_3d	begin_launch(t_player *player, t_calculs *calculs, t_cartesienne *ray)
{
	t_cartesienne	ray_tmp;
	t_linedef		*wall;
	t_sector		*sector;

	calculs->dist_sum = 0;
	sector = player->sector;
	ray_tmp = (t_cartesienne){0, 0, 0, ray->vx, ray->vy, ray->vz, ray->x, ray->y, NULL};
	wall = launch_ray_3d(calculs, &ray_tmp, sector);
	while (wall && wall->flags & PORTAL &&
			!(calculs->closest_2.z < wall->destline->sector->floor_height ||\
			calculs->closest_2.z > wall->destline->sector->ceil_height) &&\
			calculs->dist_sum < RENDER_DISTANCE)
	{
		ray_tmp.ox = calculs->closest_2.x;
		ray_tmp.oy = calculs->closest_2.y;
		ray_tmp.oz = calculs->closest_2.z;
		teleportation_ray(&ray_tmp, wall, wall->destline, &sector);
		rotate(&ray_tmp, create_matrice(wall->destline->angle - wall->angle +\
										(sign(wall->p2.x - wall->p1.x) ==\
										sign(wall->destline->p2.x - wall->destline->p1.x) ?\
										0 : M_PI)));
		wall = launch_ray_3d(calculs, &ray_tmp, sector);
	}
	return (calculs->closest_2);
}




int				raycasting_3d_static_rays(t_win *win, t_player *player)
{
	t_calculs	calculs;
	t_cartesienne	*ray;

	calculs.raycast = 1;
	ray = player->rays;
	// printf("Debut moteur\n");
	while (ray)
	{
		begin_launch(player, &calculs, ray);
		draw_point(win, &calculs, player, ray->x, ray->y);
		ray = ray->next;
	}
	return (0);
}