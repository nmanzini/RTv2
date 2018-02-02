/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_3v_subtract.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/23 11:11:25 by mpauw             #+#    #+#             */
/*   Updated: 2018/02/02 11:21:46 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_3v	ft_3v_subtract(t_3v v1, t_3v v2)
{
	t_3v	v_sub;
	int		i;

	i = 0;
	while (i < 3)
	{
		(v_sub.v)[i] = (v1.v)[i] - (v2.v)[i];
		i++;
	}
	return (v_sub);
}
