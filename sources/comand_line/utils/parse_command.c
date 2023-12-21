/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:13:02 by amassias          #+#    #+#             */
/*   Updated: 2023/12/21 03:52:05 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_command_line_internal.h"

int	parse_command(
		const char *input,
		t_fractol *fractol)
{
	const char	**tokens;
	const char	**tmp;
	int			return_code;

	tokens = (const char **)ft_split(input, ' ');
	if (tokens == NULL)
		(void)0;
	else if (tokens[0] == NULL)
		return (free(tokens), EXIT_SUCCESS);
	else if (ft_strcmp(*tokens, "p") == 0 || ft_strcmp(*tokens, "print") == 0)
		return_code = command__print(tokens + 1, &fractol->cl);
	else if (ft_strcmp(*tokens, "s") == 0 || ft_strcmp(*tokens, "set") == 0)
		return_code = command__set(tokens + 1, &fractol->cl);
	else if (ft_strcmp(*tokens, "q") == 0 || ft_strcmp(*tokens, "quit") == 0)
		return_code = command__quit(tokens + 1, fractol);
	else if (ft_strcmp(*tokens, "h") == 0 || ft_strcmp(*tokens, "help") == 0)
		return_code = command__help(tokens + 1);
	// else if (ft_strcmp(*tokens, "r") == 0 || ft_strcmp(*tokens, "reload") == 0)
	// 	return_code = command__reload(tokens + 1, fractol);
	else
	{
		ft_printf("Unknown command \"%s\".\n", tokens[0]);
		return_code = EXIT_FAILURE;
	}
	tmp = tokens;
	while (*tmp)
		free((char *)*tmp++);
	free((char **)tokens);
	return (return_code);
}
