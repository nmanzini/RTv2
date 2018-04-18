/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_light_per_pixel.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/20 15:42:20 by mpauw             #+#    #+#             */
/*   Updated: 2018/04/18 11:39:13 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int		light_reaches(t_3v dir, t_list *objects, int fixed_val_id)
{
	double		t_value;
	int			reached;
	t_list		*o_lst;
	t_object	*obj;

	o_lst = objects;
	reached = 0;
	while (o_lst && o_lst->content)
	{
		obj = (t_object *)o_lst->content;
		t_value = obj->f(obj, dir, fixed_val_id);
		if (t_value > 0.001 && t_value < 0.99999)
			return (0);
		else if (t_value > 0.999999 && t_value < 1.000001)
			reached = 1;
		o_lst = o_lst->next;
	}
	return (reached);
}

static void		check_values(t_intensity *in, t_3v color, t_source l)
{
	if (in->diff > 1)
		in->diff = 1;
	if (in->spec > 1)
		in->spec = 1;
	if (color.v[0] > 1 || color.v[1] > 1 || color.v[2] > 1
			|| color.v[0] < 0 || color.v[1] < 0
			|| color.v[2] < 0 || ((l.color).v)[0] > 1
			|| ((l.color).v)[1] > 1 || ((l.color).v)[2] > 1
			|| ((l.color).v)[0] < 0 || ((l.color).v)[1] < 0
			|| ((l.color).v)[2] < 0)
		error(4);
}

static double	set_light_value(t_intensity in, t_pixel *p,
		t_source l, int i)
{
	t_3v		*c;
	t_3v		o;
	double		influence;

	o = p->obj_color[i];
	c = &(p->c_per_src[l.id]);
	influence = 1.0;
	while (i > 0)
	{
		influence *= ((p->vis_obj)[i - 1])->specular;
		i--;
	}
	in.diff = in.diff * (l.intensity).diff;
	in.spec = in.spec * (l.intensity).spec;
	check_values(&in, o, l);
	(c->v)[0] += influence * (1 - in.spec) * in.diff * ((l.color).v)[0]
		* o.v[0];
	(c->v)[1] += influence * (1 - in.spec) * in.diff * ((l.color).v)[1]
		* o.v[1];
	(c->v)[2] += influence * (1 - in.spec) * in.diff * ((l.color).v)[2]
		* o.v[2];
	(c->v)[0] += in.spec * ((l.color).v)[0];
	(c->v)[1] += in.spec * ((l.color).v)[1];
	(c->v)[2] += in.spec * ((l.color).v)[2];
	return ((c->v)[0] + (c->v)[1] + (c->v)[2]);
}

static int		inside_object(t_pixel *p, t_source src, t_cam cam, int amount)
{
	int	i;

	i = 0;
	(void)p;
	while (i < amount)
	{
		if (src.inside_obj[i] != cam.inside_obj[i])
			return (0);
		i++;
	}
	return (1);
}

static void		light_intensity(t_source src, t_pixel *p, t_scene *scene)
{
	t_3v			dir;
	t_intensity		in;
	int				r;
	double			total_value;

	r = 0;
	total_value = 0.0;
	while (r < scene->refl && p->vis_obj[r])
	{
		dir = ft_3v_subtract(p->point[r], src.origin);
		if (!inside_object(p, src, scene->camera, scene->amount_obj))
			break ;
		in.diff = 0;
		in.spec = 0;
		if (light_reaches(dir, scene->objects, src.id + scene->refl - 1) > 0.01)
			in = get_intensity(p, r, dir, scene->camera);
		total_value = set_light_value(in, p, src, r);
		r++;
	}
	if (total_value > scene->max_value)
		scene->max_value = total_value;
}

void		set_light_per_pixel(t_event *event, t_source src)
{
	t_pixel		*p;
	int			i;
	int			j;

	i = 0;
	while (i < (event->scene).height)
	{
		j = 0;
		while (j < (event->scene).width)
		{
			p = &((event->p_array)[j + i * (event->scene).width]);
			p->c_per_src[src.id] = ft_zero_3v();
			j++;
			if (!p->vis_obj[0])
				continue ;
			light_intensity(src, p, &(event->scene));
		}
		i++;
	}
}
