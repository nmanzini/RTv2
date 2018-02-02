/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_light.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/30 15:53:34 by mpauw             #+#    #+#             */
/*   Updated: 2018/01/30 17:28:54 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_source	init_light(t_scene **scene)
{
	t_source	light;
	t_intensity	i;

	light.origin = ft_zero_3v();
	light.color = ft_zero_3v();
	i.diff = 10;
	i.spec = 10;
	light.intensity = i;
	(*scene)->amount_light++;
	light.id = (*scene)->amount_light;
	light.type = 1;
	return (light);
}

static void		set_values_source(t_source *src, char *s, char *value)
{
	double	*tmp;

	if (ft_strncmp(s, "origin", 6) == 0)
		update_vector(&(src->origin), value);
	else if (ft_strncmp(s, "color", 5) == 0)
		update_vector(&(src->color), value);
	else if (ft_strncmp(s, "intensity", 9) == 0)
	{
		if (!(tmp = (double *)malloc(2 * sizeof(double))))
			error(0);
		get_doubles_from_line(tmp, value, 2);
		(src->intensity).diff = tmp[0];
		(src->intensity).spec = tmp[1];
		free(tmp);
	}
}

void			add_light(t_scene **scene, int fd)
{
	t_source	light;
	char		*line;
	char		*s;
	char		*value;

	light = init_light(scene);
	while (get_next_line(fd, &line) == 1)
	{
		if (*(line) == '}')
			break ;
		if (ft_isalpha(*line) || !(s = ft_brace_content(line, '{', '}')))
			error(0);
		if (!(value = ft_brace_content(s, '{', '}')))
			error(0);
		set_values_source(&light, s, value);
		free(s);
		free(value);
		free(line);
	}
	free(line);
	ft_lstaddnewr(&((*scene)->lights), &light, sizeof(t_source));
}
