/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_light_value.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 18:31:47 by mpauw             #+#    #+#             */
/*   Updated: 2018/04/11 13:20:35 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static double		light_reaches(t_3v dir, t_list *objects, int src_id)
{
	double		t_value;
	t_list		*o_lst;
	t_object	*obj;
	t_3v		dir_a;
	t_source	*rel_src;

	o_lst = objects;
	while (o_lst && o_lst->content)
	{
		obj = (t_object *)o_lst->content;
		dir_a = get_dir(dir, obj->rotation);
		rel_src = get_source(src_id, obj->rel_lights);
		t_value = obj->f(obj, dir_a, rel_src->origin);
		if (t_value > 0.001 && t_value < 0.99999)
			return (0);
		o_lst = o_lst->next;
	}
	return (1);
}

static t_3v			get_pixel_value(t_pixel *p, t_scene *scene, t_list *sources,
		t_object *obj)
{
	t_list		*s_lst;
	t_source	*src;
	t_3v		dir;
	t_3v		color;
	t_intensity	intensity;

	s_lst = sources;
	color = ft_init_3v(((obj->color).v)[0] * obj->ambient * scene->ambient,
			((obj->color).v)[1] * obj->ambient * scene->ambient,
			((obj->color).v)[2] * obj->ambient * scene->ambient);
	while (s_lst && s_lst->content)
	{
		src = (t_source *)s_lst->content;
		dir = ft_3v_subtract(p->point[0], (src->origin));
		if (light_reaches(dir, scene->objects, src->id) > 0.01)
		{
			intensity = get_intensity(p, 0, dir, scene->camera);
			update_color(intensity, &color, obj, *src);
		}
		s_lst = s_lst->next;
	}
	return (color);
}

void				*get_light_value(void *arg)
{
	t_event		*event;
	t_pixel		*pixel;
	int			i;
	int			j;

	event = (t_event *)arg;
	i = 0;
	while (i < (event->scene).height)
	{
		j = 0;
		while (j < (event->scene).width)
		{
			pixel = &((event->p_array)[j + i * (event->scene).width]);
			j++;
			if (!pixel->vis_obj[0])
				continue ;
			pixel->color = get_pixel_value(pixel, 0, (event->scene).lights,
					pixel->vis_obj[0]);
			((int *)(((t_event *)event)->img).img_arr)
				[j + (event->scene).width * i] = get_color(pixel->color);
		}
		i++;
	}
	return (NULL);
}
