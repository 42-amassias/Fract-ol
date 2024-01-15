/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 22:51:57 by amassias          #+#    #+#             */
/*   Updated: 2024/01/12 17:35:14 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

#define STATUS_OK "\033[32m"
#define STATUS_ERR "\033[31m"
#define COLOR_RESET "\033[0m"

void	_show_prompt(
			int status)
{
	const char	*status_color;

	status_color = STATUS_OK;
	if (status != 0)
		status_color = STATUS_ERR;
	ft_printf("%sfractol"COLOR_RESET" > ", status_color);
}

void	*command_line_routine(
			t_fractol *fractol)
{
	char	*line;
	char	*end;
	bool	has_failed;
	bool	show_prompt;

	show_prompt = true;
	has_failed = false;
	while (fractol->alive)
	{
		if (show_prompt)
			_show_prompt(has_failed);
		line = get_next_line(STDIN_FILENO);
		show_prompt = line != NULL;
		if (!show_prompt)
			continue ;
		end = ft_strchr(line, '\n');
		if (end)
			*end = '\0';
		has_failed = parse_command(line, fractol) != 0;
		free(line);
	}
	fractol->alive = false;
	return (NULL);
}
