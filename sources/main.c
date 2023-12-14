/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 15:23:11 by amassias          #+#    #+#             */
/*   Updated: 2023/12/14 21:16:42 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "opencl.h"

#include <X11/Xlib.h>

cl_int	set_kernel_arg__int(
		cl_kernel kernel,
		cl_uint index,
		cl_int value)
{
	return (clSetKernelArg(kernel, index, sizeof(cl_int), &value));
}

cl_int	set_kernel_arg__double(
		cl_kernel kernel,
		cl_uint index,
		cl_double value)
{
	return (clSetKernelArg(kernel, index, sizeof(cl_double), &value));
}

void	change_parameter__int(
			int *value_ptr,
			const char *param_name,
			cl_kernel kernel,
			size_t kernel_index)
{
	char	*line;
	int		v;

	printf("New value for %s (current %d): ", param_name, *value_ptr);
	fflush(stdout);
	line = get_next_line(STDIN_FILENO);
	if (line == NULL)
		return ;
	v = ft_atoi(line);
	if (v > 0)
	{
		*value_ptr = v;
		set_kernel_arg__int(kernel, kernel_index, v);
	}
	free(line);
}

void	change_parameter__double(
			double *value_ptr,
			const char *param_name,
			cl_kernel kernel,
			size_t kernel_index)
{
	char	*line;
	double	v;

	printf("New value for %s (current %lf): ", param_name, *value_ptr);
	fflush(stdout);
	line = get_next_line(STDIN_FILENO);
	if (line == NULL)
		return ;
	v = ft_atoi(line);
	if (v > 0)
	{
		*value_ptr = v;
		set_kernel_arg__double(kernel, kernel_index, v);
	}
	free(line);
}

static char	*param_str_buffer = NULL;

char	*get_arg_as_string(
			const t_kernel_arg *arg)
{
	char	*v;

	v = NULL;
	if (arg->type == CL_ARG_TYPE__CHAR)
		v = ft_itoa(*(cl_char *)arg->value);
	else if (arg->type == CL_ARG_TYPE__SHORT)
		v = ft_itoa(*(cl_short *)arg->value);
	else if (arg->type == CL_ARG_TYPE__INT)
		v = ft_itoa(*(cl_int *)arg->value);
	else if (arg->type == CL_ARG_TYPE__LONG)
		v = ft_itoa(*(cl_long *)arg->value);
	else if (arg->type == CL_ARG_TYPE__UCHAR)
		v = ft_itoa(*(cl_uchar *)arg->value);
	else if (arg->type == CL_ARG_TYPE__USHORT)
		v = ft_itoa(*(cl_ushort *)arg->value);
	else if (arg->type == CL_ARG_TYPE__UINT)
		v = ft_itoa(*(cl_uint *)arg->value);
	else if (arg->type == CL_ARG_TYPE__ULONG)
		v = ft_itoa(*(cl_ulong *)arg->value);
	// else if (arg->type == CL_ARG_TYPE__FLOAT)
	// 	v = ft_ftoa(*(cl_float *)arg->value);
	// else if (arg->type == CL_ARG_TYPE__DOUBLE)
	// 	v = ft_ftoa(*(cl_double *)arg->value);
	return (v);
}

int	handle_keys(
		int keycode,
		t_data *data)
{
	if (keycode == 'p')
	{
		size_t			i;
		// size_t			len;
		char			*tmp;
		// char			*v;
		static size_t	size;
		char			*padd;

		if (param_str_buffer == NULL)
		{
			i = 0;
			size = 0;
			param_str_buffer = malloc(1);
			param_str_buffer[0] = '\0';
			while (i < data->cl.kernel->arg_count)
			{
				padd = ft_strdup("               ");
				ft_memcpy(padd, data->cl.kernel->args[i].name, ft_strlen(data->cl.kernel->args[i].name));
				tmp = ft_strcat(param_str_buffer, padd);
				free(padd);
				free(param_str_buffer);
				param_str_buffer = tmp;
				tmp = ft_strcat(param_str_buffer, " : ");
				free(param_str_buffer);
				param_str_buffer = tmp;
				padd = malloc(128);
				if (data->cl.kernel->args[i].type == CL_ARG_TYPE__DOUBLE)
					sprintf(padd, "%lf\n", *(cl_double *)data->cl.kernel->args[i].value);
				else if (data->cl.kernel->args[i].type == CL_ARG_TYPE__INT)
					sprintf(padd, "%d\n", *(cl_int *)data->cl.kernel->args[i].value);
				else
					sprintf(padd, "x\n");
				tmp = ft_strcat(param_str_buffer, padd);
				free(padd);
				free(param_str_buffer);
				param_str_buffer = tmp;
				++i;
			}
		}
	}
	else if (keycode == 'q')
		mlx_loop_end(data->mlx.mlx);
	else if (keycode == 'i')
		change_parameter__int(
			&data->params.max_itr, "MAX_ITR",
			data->cl.kernel->kernel, KERNEL_ARG_INDEX__MAX_ITR);
	else if (keycode == 'k')
		change_parameter__double(
			&data->params.k, "K",
			data->cl.kernel->kernel, KERNEL_ARG_INDEX__K);
	else if (keycode == 'x')
		change_parameter__double(
			&data->params.dx, "x",
			data->cl.kernel->kernel, KERNEL_ARG_INDEX__DX);
	else if (keycode == 'y')
		change_parameter__double(
			&data->params.dy, "y",
			data->cl.kernel->kernel, KERNEL_ARG_INDEX__DY);
	return (0);
}

int	handle_mouse(
		int button,
		int x,
		int y,
		t_data *data)
{
	if (button == 1)
	{
		data->params.dx += ((double)WIDTH / (double)HEIGHT)
			* (((double)(4 * x) / (double)(WIDTH - 1)) - 2.)
			/ data->params.zoom;
		data->params.dy += (((double)(4 * y) / (double)(HEIGHT - 1)) - 2.)
			/ data->params.zoom;
		set_kernel_arg__double(
			data->cl.kernel->kernel, KERNEL_ARG_INDEX__DX, data->params.dx);
		set_kernel_arg__double(
			data->cl.kernel->kernel, KERNEL_ARG_INDEX__DY, data->params.dy);
	}
	if (button == 4)
		data->params.zoom *= 1.1;
	if (button == 5)
		data->params.zoom /= 1.1;
	if (button == 4 || button == 5)
		set_kernel_arg__double(
			data->cl.kernel->kernel, KERNEL_ARG_INDEX__ZOOM, data->params.zoom);
	return (0);
}

int	kill_program(
			int code,
			const char *message,
			t_data *data)
{
	data->error.code = code;
	data->error.message = message;
	mlx_loop_end(data->mlx.mlx);
	return (code);
}

int	loop(
		t_data *data)
{
	static const size_t	global_work_size[] = {WIDTH, HEIGHT};
	cl_int				error_code;

	error_code = clEnqueueNDRangeKernel(
			data->cl.command_queue, data->cl.kernel->kernel, 2,
			NULL, global_work_size, NULL,
			0, NULL, NULL);
	if (error_code != CL_SUCCESS)
		return (kill_program(EXIT_FAILURE, ERROR__KERNEL_START, data));
	error_code = clEnqueueReadBuffer(
			data->cl.command_queue,
			data->cl.cl_screen, CL_TRUE,
			0, WIDTH * HEIGHT * sizeof(int), data->mlx.mlx_screen,
			0, NULL, NULL);
	if (error_code != CL_SUCCESS)
		return (kill_program(EXIT_FAILURE, ERROR__KERNEL_BUFFER_READ, data));
	mlx_put_image_to_window(
		data->mlx.mlx, data->mlx.window,
		data->mlx.img, 0, 0);
	if (param_str_buffer != NULL)
		mlx_string_put(data->mlx.mlx, data->mlx.window, 100, 100, 0xFFFFFF, param_str_buffer);
	return (0);
}

int	read_file(
		const char *file_path,
		char **file_buffer_ptr,
		size_t *file_size)
{
	FILE	*file;

	file = fopen(file_path, "rb");
	if (file == NULL)
		return (EXIT_FAILURE);
	fseek(file, 0, SEEK_END);
	*file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	*file_buffer_ptr = (char *) malloc(*file_size + 1);
	if (*file_buffer_ptr == NULL)
		return (EXIT_FAILURE);
	if (fread(*file_buffer_ptr, 1, *file_size, file) != *file_size)
		return (free(*file_buffer_ptr), EXIT_FAILURE);
	(*file_buffer_ptr)[*file_size] = '\0';
	fclose(file);
	return (EXIT_SUCCESS);
}

// TODO: cleanup ALL the loaded kernels.
void	cleanup_opencl(
			t_data *data)
{
	clReleaseMemObject(data->cl.cl_screen);
	cleanup_kernels(data, data->cl.kernel_count);
	clReleaseProgram(data->cl.program);
	clReleaseCommandQueue(data->cl.command_queue);
	clReleaseContext(data->cl.context);
	clReleaseDevice(data->cl.device);
	free(data->cl.platform);
}

void	cleanup_mlx(
			t_data *data)
{
	if (data->mlx.img != NULL)
		mlx_destroy_image(data->mlx.mlx, data->mlx.img);
	if (data->mlx.window != NULL)
		mlx_destroy_window(data->mlx.mlx, data->mlx.window);
	if (data->mlx.mlx != NULL)
		mlx_destroy_display(data->mlx.mlx);
	free(data->mlx.mlx);
}

void	full_cleanup(
			t_data *data)
{
	cleanup_opencl(data);
	cleanup_mlx(data);
}

int	prime_kernel_args(
		t_data *data)
{
	if (clSetKernelArg(data->cl.kernel->kernel, KERNEL_ARG_INDEX__BUFFER,
			sizeof(cl_mem), &data->cl.cl_screen) != CL_SUCCESS
		|| clSetKernelArg(data->cl.kernel->kernel, KERNEL_ARG_INDEX__MAX_ITR,
			sizeof(cl_int), &data->params.max_itr) != CL_SUCCESS
		|| clSetKernelArg(data->cl.kernel->kernel, KERNEL_ARG_INDEX__K,
			sizeof(cl_double), &data->params.k) != CL_SUCCESS
		|| clSetKernelArg(data->cl.kernel->kernel, KERNEL_ARG_INDEX__ZOOM,
			sizeof(cl_double), &data->params.zoom) != CL_SUCCESS
		|| clSetKernelArg(data->cl.kernel->kernel, KERNEL_ARG_INDEX__DX,
			sizeof(cl_double), &data->params.dx) != CL_SUCCESS
		|| clSetKernelArg(data->cl.kernel->kernel, KERNEL_ARG_INDEX__DY,
			sizeof(cl_double), &data->params.dy) != CL_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	parse_kernel_names(
			char *name_list)
{
	while (*name_list)
	{
		if (*name_list == ';')
			*name_list = '\0';
		++name_list;
	}
}

// `type` cannot be an empty string, so taking `type[len(type) - 2]` is ok !
int	is_type_ptr(
		const char *type)
{
	while (*type)
		++type;
	return (type[-1] == '*');
}

int	get_param_size(
		const char *type,
		size_t *size,
		t_cl_arg_type *internal_type)
{
	size_t	i;

	if (is_type_ptr(type))
		type = "pointer";
	i = 0;
	while (i < CL_ARG_TYPE_COUNT)
	{
		if (ft_strcmp(type, g_cl_types[i].str_type) == 0)
			break ;
		++i;
	}
	if (i == CL_ARG_TYPE_COUNT)
		return (EXIT_FAILURE);
	*size = g_cl_types[i].size;
	*internal_type = g_cl_types[i].internal_type;
	return (EXIT_SUCCESS);
}

int	get_kernel_arg_info(
		cl_kernel kernel,
		cl_uint index,
		cl_kernel_arg_info info,
		void **buffer_ptr)
{
	size_t	size;

	if (clGetKernelArgInfo(kernel, index, info, 0, NULL, &size) != CL_SUCCESS)
		return (EXIT_FAILURE);
	*buffer_ptr = (void *) malloc(size);
	if (*buffer_ptr == NULL)
		return (EXIT_FAILURE);
	if (clGetKernelArgInfo(kernel, index,
			info, size, *buffer_ptr, NULL) != CL_SUCCESS)
		return (free(*buffer_ptr), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	check_special_argument(
	const t_kernel *kernel,
	const t_arg_info *arg_info)
{
	cl_kernel_arg_access_qualifier	*qualifier;
	char							*name;
	char							*type;

	if (get_kernel_arg_info(kernel->kernel, arg_info->index, CL_KERNEL_ARG_NAME, (void **)&name) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (ft_strcmp(name, arg_info->name) != 0)
		return (printf("Expected argument %u to be \"%s\" but got \"%s\".\n", arg_info->index, arg_info->name, name), free(name), EXIT_FAILURE);
	if (get_kernel_arg_info(kernel->kernel, arg_info->index, CL_KERNEL_ARG_TYPE_NAME, (void **)&type) != EXIT_SUCCESS)
		return (free(name), EXIT_FAILURE);
	if (ft_strcmp(type, arg_info->type) != 0)
		return (printf("Expected argument %u's type to be \"%s\" but got \"%s\".\n", arg_info->index, arg_info->type, type), free(name), free(type), EXIT_FAILURE);
	if (get_kernel_arg_info(kernel->kernel, arg_info->index, CL_KERNEL_ARG_ADDRESS_QUALIFIER, (void **)&qualifier) != EXIT_SUCCESS)
		return (free(name), free(type), EXIT_FAILURE);
	if (*qualifier != arg_info->qualifier)
		return (printf("Expected argument %u's type qualifier to be \"%u\" but got \"%u\".\n", arg_info->index, arg_info->qualifier, *qualifier), free(name), free(type), free(qualifier), EXIT_FAILURE);
	printf("\t> %-10s %-15s\n", type, name);
	free(name);
	free(type);
	free(qualifier);
	return (EXIT_SUCCESS);
}

int	main(void)
{
	int				_;
	static t_data	data = {0};

	data.params.zoom = 1.;
	data.params.k = log(2);
	data.params.max_itr = 1024;
	data.params.dx = -0.744567;
	data.params.dy = 0.121201;
	data.mlx.mlx = mlx_init();
	data.mlx.window = mlx_new_window(data.mlx.mlx, WIDTH, HEIGHT, "fract-ol");
	data.mlx.img = mlx_new_image(data.mlx.mlx, WIDTH, HEIGHT);
	data.mlx.mlx_screen = mlx_get_data_addr(data.mlx.img, &_, &_, &_);
	if (init_opencl(&data) != EXIT_SUCCESS)
		return (cleanup_opencl(&data), EXIT_FAILURE);
	if (prime_kernel_args(&data) != EXIT_SUCCESS)
		return (cleanup_opencl(&data), EXIT_FAILURE);
	mlx_set_font(data.mlx.mlx, data.mlx.window, "-*-*-r-normal-*-12-120-*-*-*-*-*-*");
	mlx_loop_hook(data.mlx.mlx, loop, &data);
	mlx_key_hook(data.mlx.window, handle_keys, &data);
	mlx_mouse_hook(data.mlx.window, handle_mouse, &data);
	mlx_loop(data.mlx.mlx);
	full_cleanup(&data);
	return (data.error.code);
}
