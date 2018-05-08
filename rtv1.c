/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/03 08:05:39 by mpauw             #+#    #+#             */
/*   Updated: 2018/05/08 17:15:31 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		error(int err)
{
	if (err == 0)
		ft_putstr_fd("Error: Error reading file\n", 2);
	else if (err == 1)
		ft_putstr_fd("Error: Error in memory allocation\n", 2);
	else if (err == 2)
		ft_putstr_fd("Error: NULL returned\n", 2);
	else if (err == 3)
		ft_putstr_fd("Error: Values must be properly tabulated.\n", 2);
	else if (err == 4)
		ft_putstr_fd("Error: Color values must be between 0 and 1.\n", 2);
	else if (err == 5)
		ft_putstr_fd("Error: There is a vector of size 0. Check if there are\
				lights with the same coordinates as objects.\n", 2);
	else if (err == 6)
		ft_putstr_fd("Error: You are trying to divide by zero.\n", 2);
	else if (err == 6)
		ft_putstr_fd("Error: Negative value for height.\n", 2);
	exit(1);
}

void		s_error(const char *s)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

static void	usage(void)
{
	ft_putstr("You must enter a valid file name\n");
	exit(0);
}

static void	run_calc(t_event *event)
{
//	light_inside(&(event->scene));
	get_s_values((void *)event);
	init_light_values((void *)event);
	turn_on_lights(event);
	mlx_put_image_to_window(event->mlx, event->win,
		(event->img).img_ptr, 0, 0);
	init_loop(event);
}

int			main(int argc, char **argv)
{
	int			fd;
	t_scene		scene;
	t_event		event;

	if (argc != 2)
		usage();
	if ((fd = open(argv[1], O_RDONLY)) == -1)
		error(0);
	set_scene(fd, &scene);
	if (!(scene.cam_set))
		error(0);
	set_fixed_values(&scene);
	event = init_window(scene);
	run_calc(&event);
}
