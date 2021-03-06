/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_menu_control.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpauw <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 11:08:33 by mpauw             #+#    #+#             */
/*   Updated: 2018/04/13 17:25:33 by mpauw            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
/*
static void	print_instructions(t_event *event)
{
	int		i;
	int		y;
	t_menu	*menu;

	i = 0;
	menu = &(event->menu);
	y = SUB_MENU_Y + SUB_MARGIN;
	while (i < AMOUNT_INSTRUCTIONS)
	{
		mlx_string_put(event->mlx, event->win, menu->x + MENU_MARGIN,
			y, TEXT_DARK, (menu->strings).man[i]);
		i++;
		y += MENU_LINE;
	}
}
*/

static void	init_buttons(t_event *event, t_sub_m *main, t_menu *menu)
{
	t_sub_m	*button;
	int		i;
	int		id;

	i = 0;
	while (i < AMT_BUT)
	{
		id = init_sub_menu(menu, main->id);
		button = menu->sub_m[id];
		button->width = (main->width - 2 * SUB_MARGIN) / AMT_B_P_ROW;
		button->height = DEF_BUTTON_HEIGHT;
		button->x = main->x + SUB_MARGIN + (i % AMT_B_P_ROW) * button->width;
		button->y = SUB_MENU_Y - button->height * (AMT_B_ROW - i / AMT_B_P_ROW);
		button->type = MAIN_BUTTON;
		button->type_id = i;
		init_image(event->mlx, button->width, button->height, &(button->img));
		button->color = PRIMARY_LIGHT;
		button->color_selected = ALERT_COLOR;
		button->showing = 1;
		button->selected = (i == 0) ? 1 : 0;
		button->edge_thickness = 1;
		button->edge_color = TEXT_LIGHT;
		i++;
		add_child_id(main, button);
	}
}

static int	init_main_sub_menu(t_event *event, t_sub_m *main, t_menu *menu,
		int i)
{
	t_sub_m	*s;
	int		id;

	id = init_sub_menu(menu, main->id);
	s = menu->sub_m[id];
	s->x = main->x + SUB_MARGIN;
	s->y = SUB_MENU_Y;
	s->height = main->height - (SUB_MENU_Y + SUB_MARGIN * 2);
	if (s->height < 0)
		error(7);
	s->width = main->width - 2 * SUB_MARGIN;
	init_image(event->mlx, s->width, s->height, &(s->img));
	s->color = PRIMARY_DARK;
	s->color_selected = PRIMARY_DARK;
	s->type = SUB_MENU;
	s->type_id = i;
	s->tab_amount = 0;
	s->showing = 0;
	add_child_id(main, s);
	return (s->id);
}

static void	add_tab_buttons(t_event *event, t_sub_m *s, t_menu *menu)
{
	t_sub_m	*button;
	int		i;
	int		id;

	i = 0;
	while (i < s->tab_amount)
	{
		id = init_sub_menu(menu, s->id);
		button = menu->sub_m[id];
		button->width = TAB_BUTTON_WIDTH;
		button->height = DEF_BUTTON_HEIGHT;
		button->x = s->width / 2 - (s->tab_amount * button->width / 2) +
			i * button->width;
		button->y = s->height;
		button->type = TAB_BUTTON;
		button->type_id = i;
		button->sub_tab = -1;
		init_image(event->mlx, button->width, button->height, &(button->img));
		button->color = ALERT_COLOR;
		add_child_id(s, button);
		i++;
	}
}

void	add_sub_menu(t_event *event)
{
	t_menu	*menu;
	int		s_id;
	int		i;

	menu = &(event->menu);
	i = 0;
	init_buttons(event, menu->sub_m[0], menu);
	while (i < AMT_SUB_M) 
	{
		s_id = init_main_sub_menu(event, menu->sub_m[0], menu, i);
		if (i == OBJECT_MENU)
			add_object_menu(event, menu->sub_m[s_id], menu);
		if ((menu->sub_m[s_id])->tab_amount > 1)
			add_tab_buttons(event, menu->sub_m[s_id], menu);
		i++;
	}
}
