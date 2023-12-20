/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:13:02 by amassias          #+#    #+#             */
/*   Updated: 2023/12/20 02:39:28 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

int	parse_command(
		const char *input,
		t_fractol *fractol)
{
	char	**tokens;
	char	**tmp;
	int		return_code;

	tokens = ft_split(input, ' ');
	if (tokens == NULL)
		(void)0;
	else if (tokens[0] == NULL)
		return (free(tokens), EXIT_SUCCESS);
	else if (ft_strcmp(tokens[0], "p") == 0 || ft_strcmp(tokens[0], "print") == 0)
		return_code = command__print(tokens + 1, &fractol->cl);
	else if (ft_strcmp(tokens[0], "s") == 0 || ft_strcmp(tokens[0], "set") == 0)
		return_code = command__set(tokens + 1, &fractol->cl);
	else if (ft_strcmp(tokens[0], "q") == 0 || ft_strcmp(tokens[0], "quit") == 0)
		return_code = command__quit(tokens + 1, fractol);
	// else if (ft_strcmp(tokens[0], "r") == 0 || ft_strcmp(tokens[0], "reload") == 0)
	// 	return_code = command__reload(tokens + 1, fractol);
	else
	{
		ft_printf("Unknown command \"%s\".\n", tokens[0]);
		return_code = EXIT_FAILURE;
	}
	tmp = tokens;
	while (*tmp)
		free(*tmp++);
	free(tokens);
	return (return_code);
}
