/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/08 13:38:46 by mpauw             #+#    #+#             */
/*   Updated: 2018/02/06 13:18:46 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_object	*get_visible_object(double *s_value, t_3v dir,
		t_list *objects)
{
	double		tmp;
	double		size;
	t_object	*obj;
	t_object	*visible_object;
	t_3v		tmp_dir;

	visible_object = NULL;
	while (objects && objects->content)
	{
		obj = (t_object *)objects->content;
		tmp_dir = get_dir(dir, obj->rotation);
		size = ft_get_3v_size(tmp_dir);
		if (size == 0)
			error(5);
		tmp = obj->f(obj, tmp_dir, (obj->rel_cam).origin);
		if (tmp > 0 && tmp < *s_value)
		{
			*s_value = tmp;
			visible_object = obj;
		}
		objects = objects->next;
	}
	return (visible_object);
}

static void		get_part_value(t_scene *scene, t_3v pixel, int *part_v)
{
	t_list		*tmp;
	t_source	cam;
	double		s_value;
	t_object	*obj;
	t_3v		point;

	(pixel.v)[0] = -1000.0;
	tmp = scene->objects;
	s_value = MAX_S_VALUE;
	cam = scene->camera;
	change_dir(&pixel, (scene->camera).rotation);
	obj = get_visible_object(&s_value, pixel, scene->objects);
	scene->objects = tmp;
	if (obj)
	{
		point = cam.origin;
		ft_3v_scalar(&pixel, s_value);
		(point.v)[0] += (pixel.v)[0];
		(point.v)[1] += (pixel.v)[1];
		(point.v)[2] += (pixel.v)[2];
		*part_v = (get_light_value(point, scene, scene->lights, obj));
	}
}

void			raytracer(t_event *event, t_scene *scene, int it)
{
	t_3v		pixel;
	int			i;
	int			j;
	int			pix_val;

	i = 0;
	while (i < scene->height)
	{
		j = 0;
		while (j < scene->width)
		{
			pix_val = BG_COLOR;
			if (it == 0 || i % (2 * scene->grain) != 0
					|| j % (2 * scene->grain) != 0)
			{
				(pixel.v)[1] = (double)(j - scene->width / 2.0);
				(pixel.v)[2] = (double)(scene->height / 2.0 - i);
				get_part_value(scene, pixel, &pix_val);
				fill_square(&(event->img), j + i * (event->img)->size_line_int,
					scene->grain, pix_val);
			}
			j += scene->grain;
		}
		i += scene->grain;
	}
}
