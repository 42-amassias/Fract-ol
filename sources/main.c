/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 15:23:11 by amassias          #+#    #+#             */
/*   Updated: 2023/12/16 05:28:44 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "opencl.h"
#include "mlx_wrapper.h"

#include <X11/Xlib.h>

const char	*get_type_as_str(
				t_cl_arg_type type)
{
	size_t	i;

	i = 0;
	while (i < CL_TYPE_COUNT)
		if (g_cl_types[i++].internal_type == type)
			return (g_cl_types[i - 1].str_type);
	return ("Unknown");
}

int	change_param(
		t_kernel *kernel,
		cl_uint index)
{
	char			input_buffer[512 + 1];
	ssize_t			n;
	cl_int			error_code;
	t_kernel_arg	*arg;

	arg = &kernel->args[index];
	while (1)
	{
		ft_printf("%-15s %-15s = ", get_type_as_str(arg->type), arg->name);
		n = read(STDIN_FILENO, input_buffer, 512);
		if (n < 0)
			return (EXIT_FAILURE);
		if (n > 0)
			break ;
		ft_printf("\n");
	}
	if (input_buffer[n - 1] == '\n')
		--n;
	input_buffer[n] = '\0';
	if (arg->type == CL_ARG_TYPE__DOUBLE)
		*(cl_double *)arg->value = ft_atof(input_buffer);
	else if (arg->type == CL_ARG_TYPE__INT)
		*(cl_int *)arg->value = ft_atoi(input_buffer);
	else
		ft_memset(arg->value, 0, arg->size);
	error_code = clSetKernelArg(kernel->kernel, index + CL_KERNEL_PRIVTAE_ARG_COUNT, arg->size, arg->value);
	if (error_code != CL_SUCCESS)
		return (ft_printf("Failed to set kernel arg: %d\n", error_code), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	chose_new_kernel(
		t_cl *cl)
{
	static char	input_buffer[512 + 1] = {0};
	size_t		i;
	ssize_t		n;
	int			t;
	t_kernel	*kernel;

	ft_printf("Chose one of the available kernels :\n");
	i = 0;
	while (i < cl->kernel_count)
	{
		ft_printf("\t* %s\n", cl->kernels[i].name);
		++i;
	}
	t = 0;
	kernel = NULL;
	while (kernel == NULL)
	{
		if (t == 1)
			ft_printf("Unknown kernel name \"%s\".\n", input_buffer);
		ft_printf("> ");
		n = read(STDIN_FILENO, input_buffer, 512);
		if (n < 0)
			return (EXIT_FAILURE);
		if (n == 0)
			return (EXIT_SUCCESS);
		if (input_buffer[n - 1] == '\n')
			--n;
		else
			ft_putchar('\n');
		input_buffer[n] = '\0';
		i = 0;
		while (i < cl->kernel_count)
		{
			if (ft_strcmp(input_buffer, cl->kernels[i].name) == 0)
			{
				kernel = &cl->kernels[i];
				break ;
			}
			++i;
		}
		t = 1;
	}
	i = 0;
	clSetKernelArg(kernel->kernel, 0, sizeof(cl_mem), &cl->cl_screen);
	while (i < kernel->arg_count)
		if (change_param(kernel, i++))
			return (EXIT_FAILURE);
	cl->current_kernel = kernel;
	return (EXIT_SUCCESS);
}

int	command__print__kernels(
		char **tokens,
		t_cl *cl)
{
	size_t	i;

	if (tokens[0] != NULL)
	{
		ft_putstr("Error: Extra parameters for 'print kernels'.\n");
		return (EXIT_FAILURE);
	}
	i = 0;
	ft_putstr("Available kernels :\n");
	while (i < cl->kernel_count)
		ft_printf("\t* %s\n", cl->kernels[i++].name);
	return (EXIT_SUCCESS);
}

int	command__print__current(
		char **tokens,
		t_cl *cl)
{
	const char	*kernel_name;

	if (tokens[0] != NULL)
	{
		ft_putstr("Error: Extra parameters for 'print current'.\n");
		return (EXIT_FAILURE);
	}
	kernel_name = "None";
	if (cl->current_kernel != NULL)
		kernel_name = cl->current_kernel->name;
	ft_printf("Current kernel : \"%s\".\n", kernel_name);
	return (EXIT_SUCCESS);
}

int	command__print__params(
		char **tokens,
		t_cl *cl)
{
	size_t	i;

	if (tokens[0] != NULL)
	{
		ft_putstr("Error: Extra parameters for 'print params'.\n");
		return (EXIT_FAILURE);
	}
	if (cl->current_kernel == NULL)
	{
		ft_putstr("No kernel is currently selected.\n");
		return (EXIT_SUCCESS);
	}
	ft_putstr("Current kernel's arguments :\n");
	i = 0;
	while (i < cl->current_kernel->arg_count)
	{
		ft_printf("\t* %-15s %-15s = ",
			get_type_as_str(cl->current_kernel->args[i].type),
			cl->current_kernel->args[i].name);
		if (cl->current_kernel->args[i].type == CL_ARG_TYPE__DOUBLE)
			printf("%lf", *(cl_double *)cl->current_kernel->args[i].value);
		else if (cl->current_kernel->args[i].type == CL_ARG_TYPE__INT)
			printf("%d", *(cl_int *)cl->current_kernel->args[i].value);
		else
			printf("[Cannot display]");
		fflush(stdout);
		ft_putchar('\n');
		++i;
	}
	return (EXIT_SUCCESS);
}

int	command__print(
		char **tokens,
		t_cl *cl)
{
	int	return_code;

	if (tokens[0] == NULL)
	{
		ft_putstr("Error: Missing parameters !\nTry 'help print' for more information.\n");
		return (EXIT_FAILURE);
	}
	if (ft_strcmp(tokens[0], "kernels") == 0)
		return_code = command__print__kernels(tokens + 1, cl);
	else if (ft_strcmp(tokens[0], "current") == 0)
		return_code = command__print__current(tokens + 1, cl);
	else if (ft_strcmp(tokens[0], "params") == 0)
		return_code = command__print__params(tokens + 1, cl);
	else
	{
		ft_printf("Unknown sub command \"%s\" for 'print'.\n", tokens[0]);
		return_code = EXIT_FAILURE;
	}
	return (return_code);
}

int	command__set__kernel__name(
		char **tokens,
		t_cl *cl)
{
	size_t	i;

	if (tokens[1] != NULL)
	{
		ft_putstr("Error: Extra parameters for 'set kernel [KERNEL_NAME]'.\n");
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < cl->kernel_count)
	{
		if (ft_strcmp(tokens[0], cl->kernels[i].name) == 0)
			break ;
		++i;
	}
	if (i == cl->kernel_count)
	{
		ft_printf("Unknown kernel \"%s\".\n", tokens[0]);
		return (EXIT_FAILURE);
	}
	cl->current_kernel = &cl->kernels[i];
	return (EXIT_SUCCESS);
}

int	command__set__kernel(
		char **tokens,
		t_cl *cl)
{
	if (tokens[0] == NULL)
	{
		ft_putstr("Error: Error: Missing parameters !\nTry 'help set kernel' for more information.\n");
		return (EXIT_FAILURE);
	}
	return (command__set__kernel__name(tokens, cl));
}

int	command__set__param__name(
		char **tokens,
		t_cl *cl)
{
	size_t	i;

	if (tokens[1] != NULL)
	{
		ft_putstr("Error: Extra parameters for 'set param [PARAMETER_NAME]'.\n");
		return (EXIT_FAILURE);
	}
	if (cl->current_kernel == NULL)
	{
		ft_putstr("No kernel is currently selected.\n");
		return (EXIT_SUCCESS);
	}
	i = 0;
	while (i < cl->current_kernel->arg_count)
	{
		if (ft_strcmp(tokens[0], cl->current_kernel->args[i].name) == 0)
			break ;
		++i;
	}
	if (i == cl->current_kernel->arg_count)
	{
		ft_printf("Unknown parameter name \"%s\".\n", tokens[0]);
		return (EXIT_FAILURE);
	}
	if (change_param(cl->current_kernel, i))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	command__set__param(
		char **tokens,
		t_cl *cl)
{
	if (tokens[0] == NULL)
	{
		ft_putstr("Error: Error: Missing parameters !\nTry 'help set param' for more information.\n");
		return (EXIT_FAILURE);
	}
	return (command__set__param__name(tokens, cl));
}

int	command__set(
		char **tokens,
		t_cl *cl)
{
	int	return_code;

	if (tokens[0] == NULL)
	{
		ft_putstr("Error: Missing parameters !\nTry 'help set' for more information.\n");
		return (EXIT_FAILURE);
	}
	if (ft_strcmp(tokens[0], "kernel") == 0)
		return_code = command__set__kernel(tokens + 1, cl);
	else if (ft_strcmp(tokens[0], "param") == 0)
		return_code = command__set__param(tokens + 1, cl);
	else
	{
		ft_printf("Unknown sub command \"%s\" for 'set'.\n", tokens[0]);
		return_code = EXIT_FAILURE;
	}
	return (EXIT_SUCCESS);
}

int	command__quit(
		char **tokens,
		t_fractol *fractol)
{
	if (tokens[0] != NULL)
	{
		ft_putstr("Error: Extra parameters for 'quit'.\n");
		return (EXIT_FAILURE);
	}
	ft_putstr("Goodbye !\n");
	mlx_loop_end(fractol->mlx.mlx);
	return (EXIT_SUCCESS);
}

int	parse_command(
		const char *input,
		t_fractol *fractol)
{
	char	**tokens;
	char	**tmp;
	int		return_code;

	tokens = ft_split(input, ' ');
	if (tokens == NULL)
		return (EXIT_FAILURE);
	if (ft_strcmp(tokens[0], "p") == 0 || ft_strcmp(tokens[0], "print") == 0)
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

int	handle_keys(
		int keycode,
		t_fractol *fractol)
{
	static char	input_buffer[512 + 1] = {0};
	ssize_t		n;

	if (keycode == 'c')
	{
		ft_putstr("> ");
		n = read(STDIN_FILENO, input_buffer, 512);
		if (n < 0)
		{
			ft_putstr("Failed to read standard input !\n");
			return (EXIT_SUCCESS);
		}
		if (n == 0)
		{
			ft_putchar('\n');
			return (EXIT_SUCCESS);
		}
		if (input_buffer[n - 1] == '\n')
			--n;
		else
			ft_putchar('\n');
		input_buffer[n] = '\0';
		if (parse_command(input_buffer, fractol))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	else if (keycode == 'q')
		mlx_loop_end(fractol->mlx.mlx);
	return (EXIT_SUCCESS);
}

int	handle_mouse(
		int button,
		int x,
		int y,
		t_fractol *fractol)
{
	if (fractol->cl.current_kernel == NULL)
		return (EXIT_SUCCESS);
	if (button == 0)
		return (EXIT_SUCCESS);
	if (button == 1)
	{
		*(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__DX - CL_KERNEL_PRIVTAE_ARG_COUNT].value
			+= ((double)WIDTH / (double)HEIGHT)
			* (((double)(4 * x) / (double)(WIDTH - 1)) - 2.)
			/ *(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value;
		*(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__DY - CL_KERNEL_PRIVTAE_ARG_COUNT].value
			+= (((double)(4 * y) / (double)(HEIGHT - 1)) - 2.)
			/ *(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value;
		clSetKernelArg(fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__DX, sizeof(cl_double), fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__DX - CL_KERNEL_PRIVTAE_ARG_COUNT].value);
		clSetKernelArg(fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__DY, sizeof(cl_double), fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__DY - CL_KERNEL_PRIVTAE_ARG_COUNT].value);
	}
	else if (button == 4)
		*(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value *= 1.1;
	else if (button == 5)
		*(cl_double *)fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value /= 1.1;
	if (button == 4 || button == 5)
		clSetKernelArg(fractol->cl.current_kernel->kernel, KERNEL_ARG_INDEX__ZOOM, sizeof(cl_double), fractol->cl.current_kernel->args[KERNEL_ARG_INDEX__ZOOM - CL_KERNEL_PRIVTAE_ARG_COUNT].value);
	return (0);
}

int	kill_program(
			int code,
			const char *message,
			t_fractol *fractol)
{
	fractol->error.code = code;
	fractol->error.message = message;
	mlx_loop_end(fractol->mlx.mlx);
	return (code);
}

int update(
		t_fractol *fractol)
{
	(void) fractol;
	return (EXIT_SUCCESS);
}


int	render(
		t_fractol *fractol)
{
	static const size_t	global_work_size[] = {WIDTH, HEIGHT};
	cl_int				error_code;

	if (fractol->cl.current_kernel == NULL)
		return (EXIT_SUCCESS);
	error_code = clEnqueueNDRangeKernel(
			fractol->cl.command_queue, fractol->cl.current_kernel->kernel, 2,
			NULL, global_work_size, NULL,
			0, NULL, NULL);
	if (error_code != CL_SUCCESS)
		return (ft_printf("Kernel failed (%d) :(\n", error_code), mlx_loop_end(fractol->mlx.mlx), EXIT_FAILURE);
	error_code = clEnqueueReadBuffer(
			fractol->cl.command_queue,
			fractol->cl.cl_screen, CL_TRUE,
			0, WIDTH * HEIGHT * sizeof(int), fractol->mlx.screen,
			0, NULL, NULL);
	if (error_code != CL_SUCCESS)
		return (ft_printf("Read buffer failed\n"), mlx_loop_end(fractol->mlx.mlx), EXIT_FAILURE);
	mlx_put_image_to_window(
		fractol->mlx.mlx, fractol->mlx.window,
		fractol->mlx.img, 0, 0);
	return (0);
}

void	full_cleanup(
			t_fractol *fractol)
{
	cleanup_opencl(&fractol->cl);
	cleanup_mlx(&fractol->mlx);
}

void	create_handlers(
			t_handlers *handlers,
			t_fractol *fractol)
{
	handlers->update = (int (*)(void *))update;
	handlers->context.update = fractol;
	handlers->render = (int (*)(void *))render;
	handlers->context.render = fractol;
	handlers->keyboard = (int (*)(int, void *))handle_keys;
	handlers->context.keyboard = fractol;
	handlers->mouse = (int (*)(int, int, int, void *))handle_mouse;
	handlers->context.mouse = fractol;
}

int	main(void)
{
	static t_fractol	fractol = {0};
	static t_handlers	handlers = {0};

	fractol.params.dx = -0.744567;
	fractol.params.dy = 0.121201;
	fractol.params.zoom = 1.;
	create_handlers(&handlers, &fractol);
	if (init_mlx(&fractol.mlx, WIDTH, HEIGHT, &handlers))
		return (EXIT_FAILURE);
	if (init_opencl(&fractol.cl, fractol.mlx.screen, WIDTH * HEIGHT))
		return (full_cleanup(&fractol), EXIT_FAILURE);
	if (prime_private_kernel_fields(&fractol.cl, WIDTH, HEIGHT))
		return (full_cleanup(&fractol), EXIT_FAILURE);
	mlx_loop(fractol.mlx.mlx);
	full_cleanup(&fractol);
	return (fractol.error.code);
}
