/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 15:23:11 by amassias          #+#    #+#             */
/*   Updated: 2023/12/13 20:38:58 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include "fractol.h"

typedef enum e_cl_arg_type {
	CL_ARG_TYPE__CHAR,
	CL_ARG_TYPE__SHORT,
	CL_ARG_TYPE__INT,
	CL_ARG_TYPE__LONG,
	CL_ARG_TYPE__UCHAR,
	CL_ARG_TYPE__USHORT,
	CL_ARG_TYPE__UINT,
	CL_ARG_TYPE__ULONG,
	CL_ARG_TYPE__FLOAT,
	CL_ARG_TYPE__DOUBLE,
	CL_ARG_TYPE_COUNT
}	t_cl_arg_type;

typedef struct s_cl_type {
	const char		*str_type;
	t_cl_arg_type	internal_type;
	size_t			size;
}	t_cl_type;

const t_cl_type	g_cl_types[] = {
	{"char", CL_ARG_TYPE__CHAR, sizeof(cl_char)},
	{"short", CL_ARG_TYPE__SHORT, sizeof(cl_short)},
	{"int", CL_ARG_TYPE__INT, sizeof(cl_int)},
	{"long", CL_ARG_TYPE__LONG, sizeof(cl_long)},
	{"uchar", CL_ARG_TYPE__UCHAR, sizeof(cl_uchar)},
	{"ushort", CL_ARG_TYPE__USHORT, sizeof(cl_ushort)},
	{"uint", CL_ARG_TYPE__UINT, sizeof(cl_uint)},
	{"ulong", CL_ARG_TYPE__ULONG, sizeof(cl_ulong)},
	{"float", CL_ARG_TYPE__FLOAT, sizeof(cl_float)},
	{"double", CL_ARG_TYPE__DOUBLE, sizeof(cl_double)},
};

void	cl_release_platform_list(
			cl_platform_id *platforms,
			cl_uint platform_count)
{
	while (platform_count--)
		free(platforms[platform_count]);
	free(platforms);
}

void	cl_release_device_list(
			cl_device_id *devices,
			cl_uint device_count)
{
	while (device_count--)
	{
		clReleaseDevice(devices[device_count]);
		free(devices[device_count]);
	}
	free(devices);
}

/**
 * @brief Queries 
 * @param id 
 * @return int 
 */
int	cl_get_platform_id(
		cl_platform_id *id)
{
	cl_platform_id	*platforms;
	cl_uint			platform_count;
	cl_uint			platform_index;
	size_t			platform_name_size;
	char			*platform_name;
	char			*option;

	if (clGetPlatformIDs(0, NULL, &platform_count) != CL_SUCCESS)
		return (EXIT_FAILURE);
	platforms = (cl_platform_id *) malloc(platform_count * sizeof(cl_platform_id));
	if (platforms == NULL)
		return (EXIT_FAILURE);
	if (clGetPlatformIDs(platform_count, platforms, NULL)  != CL_SUCCESS)
		return (cl_release_platform_list(platforms, 0), EXIT_FAILURE);
	if (platform_count <= 0)
		return (cl_release_platform_list(platforms, platform_count), EXIT_FAILURE);
	ft_printf("OpenCL platofrms available:\n");
	platform_index = 0;
	while (platform_index < platform_count)
	{
		if (clGetPlatformInfo(platforms[platform_index], CL_PLATFORM_NAME, 0, NULL, &platform_name_size) != CL_SUCCESS)
			return (cl_release_platform_list(platforms, platform_count), EXIT_FAILURE);
		platform_name = (char *) malloc(platform_name_size);
		if (platform_name == NULL)
			return (cl_release_platform_list(platforms, platform_count), EXIT_FAILURE);
		if (clGetPlatformInfo(platforms[platform_index], CL_PLATFORM_NAME, platform_name_size, platform_name, NULL) != CL_SUCCESS)
			return (free(platform_name), cl_release_platform_list(platforms, platform_count), EXIT_FAILURE);
		ft_printf("\t%d: %s\n", platform_index + 1, platform_name);
		free(platform_name);
		++platform_index;
	}
	if (platform_count > 1)
	{
		ft_printf("Chose a platform : ");
		while (1)
		{
			option = get_next_line(STDIN_FILENO);
			if (option != NULL)
			{
				platform_index = ft_atoi(option);
				free(option);
				if (platform_index > 0 && platform_index <= platform_count)
					break ;
			}
			ft_printf("Chose a platform : ");
		}
	}
	*id = platforms[platform_index - 1];
	platform_index = 0;
	while (platform_index < platform_count)
	{
		if (platforms[platform_index] != *id)
			free(platforms[platform_index]);
		++platform_index;
	}
	return (free(platforms), EXIT_SUCCESS);
}

char	*cl_get_device_info(
			cl_device_id device_id,
			cl_int info)
{
	size_t	buffer_size;
	char	*buffer;

	if (clGetDeviceInfo(device_id, info, 0, NULL, &buffer_size))
		return (NULL);
	buffer = (char *) malloc(buffer_size);
	if (buffer == NULL)
		return (NULL);
	if (clGetDeviceInfo(device_id, info, buffer_size, buffer, NULL))
		return (free(buffer), NULL);
	return (buffer);
}

int	cl_get_device_id(
		cl_platform_id platform_id,
		cl_device_id *id)
{
	cl_device_id	*devices;
	cl_uint			device_count;
	cl_uint			device_index;
	char			*buffer;
	char			*option;

	if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &device_count) != CL_SUCCESS)
		return (EXIT_FAILURE);
	devices = (cl_device_id *) malloc(device_count * sizeof(cl_device_id));
	if (devices == NULL)
		return (EXIT_FAILURE);
	if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, device_count, devices, NULL) != CL_SUCCESS)
		return (cl_release_device_list(devices, 0), EXIT_FAILURE);
	if (device_count <= 0)
		return (cl_release_device_list(devices, device_count), EXIT_FAILURE);
	ft_printf("OpenCL devices available:\n");
	device_index = 0;
	while (device_index < device_count)
	{
		buffer = cl_get_device_info(devices[device_index], CL_DEVICE_NAME);
		if (buffer == NULL)
			return (cl_release_device_list(devices, device_count), EXIT_FAILURE);
		ft_printf("\t%d: %s >> ", device_index + 1, buffer);
		free(buffer);
		buffer = cl_get_device_info(devices[device_index], CL_DEVICE_VERSION);
		ft_printf("%s\n", buffer);
		free(buffer);
		++device_index;
	}
	if (device_count > 1)
	{
		ft_printf("Chose a device : ");
		while (1)
		{
			option = get_next_line(STDIN_FILENO);
			if (option != NULL)
			{
				device_index = ft_atoi(option);
				free(option);
				if (device_index > 0 && device_index <= device_count)
					break ;
			}
			ft_printf("Chose a device : ");
		}
	}
	*id = devices[device_index - 1];
	device_index = 0;
	while (device_index < device_count)
	{
		if (devices[device_index] != *id)
			free(devices[device_index]);
		++device_index;
	}
	return (free(devices), EXIT_SUCCESS);
}

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

int	handle_keys(
		int keycode,
		t_data *data)
{
	if (keycode == 'q')
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

int loop(
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

void	cleanup_kernels(
			t_data *data,
			size_t count)
{
	while (count-- > 0)
	{
		while (data->cl.kernels[count].arg_count-- > 0)
			free((void *)data->cl.kernels[count].args[data->cl.kernels[count].arg_count].name);
		free((void *)data->cl.kernels[count].args);
		free((void *)data->cl.kernels[count].name);
		free((void *)data->cl.kernels[count]._arg_values);
		clReleaseKernel(data->cl.kernels[count].kernel);
	}
	free((void *)data->cl.kernels);
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

int	check_special_argument(
	t_kernel *kernel,
	cl_uint index,
	const char *_name,
	const char *_type,
	cl_kernel_arg_access_qualifier _qualifier)
{
	cl_kernel_arg_access_qualifier	qualifier;
	char							*name;
	size_t							size;
	char							type[512];

	clGetKernelArgInfo(kernel->kernel, index, CL_KERNEL_ARG_NAME, 0, NULL, &size);
	name = (char *) malloc(size * sizeof(char));
	if (name == NULL)
		return (EXIT_FAILURE);
	clGetKernelArgInfo(kernel->kernel, index, CL_KERNEL_ARG_NAME, size, name, NULL);
	if (ft_strcmp(name, _name) != 0)
		return (printf("Expected argument %u to be \"%s\" but got \"%s\".\n", index, _name, name), free(name), EXIT_FAILURE);
	clGetKernelArgInfo(kernel->kernel, index, CL_KERNEL_ARG_TYPE_NAME, 512, type, NULL);
	if (ft_strcmp(type, _type) != 0)
		return (printf("Expected argument %u's type to be \"%s\" but got \"%s\".\n", index, _type, type), free(name), EXIT_FAILURE);
	clGetKernelArgInfo(kernel->kernel, index, CL_KERNEL_ARG_ADDRESS_QUALIFIER, sizeof(cl_kernel_arg_access_qualifier), &qualifier, NULL);
	if (qualifier != _qualifier)
		return (printf("Expected argument %u's type qualifier to be \"%u\" but got \"%u\".\n", index, _qualifier, qualifier), free(name), EXIT_FAILURE);
	printf("\t> %-10s %-15s (type: %2d, size: %2zu)\n", type, name, -1, sizeof(void *));
	free(name);
	return (EXIT_SUCCESS);
}

int	build_kernel(
		t_kernel *kernel)
{
	size_t	i;
	size_t	size;
	size_t	arg_buffer_size;
	char	type[512];
	cl_int	error_code;

	printf("Building kernel \"%s\"\n", kernel->name);
	error_code = clGetKernelInfo(
		kernel->kernel, CL_KERNEL_NUM_ARGS,
		sizeof(cl_uint), &kernel->arg_count,
		NULL);
	if (error_code != CL_SUCCESS)
		return (printf("%d\n", error_code), EXIT_FAILURE);
	if (kernel->arg_count < 3)
		return (printf("Missing arguments\n"), EXIT_FAILURE);
	printf("\tArg count: %u\n", kernel->arg_count);
	kernel->arg_count -= 3;
	if (check_special_argument(kernel, 0, "screen", "int*", CL_KERNEL_ARG_ADDRESS_GLOBAL) != EXIT_SUCCESS
		|| check_special_argument(kernel, 1, "dx", "double", CL_KERNEL_ARG_ADDRESS_PRIVATE) != EXIT_SUCCESS
		|| check_special_argument(kernel, 2, "dy", "double", CL_KERNEL_ARG_ADDRESS_PRIVATE) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	kernel->args = (t_kernel_arg *) malloc(kernel->arg_count * sizeof(t_kernel_arg));
	if (kernel->args == NULL)
		return (printf("bite1\n"), EXIT_FAILURE);
	arg_buffer_size = 0;
	i = 0;
	while (i < kernel->arg_count)
	{
		clGetKernelArgInfo(kernel->kernel, i + 3, CL_KERNEL_ARG_NAME, 0, NULL, &size);
		kernel->args[i].name = (const char *) malloc(size * sizeof(char));
		if (kernel->args[i].name == NULL)
		{
			while (i-- > 0)
				free((void *)kernel->args[i].name);
			return (free(kernel->args), EXIT_FAILURE);
		}
		clGetKernelArgInfo(kernel->kernel, i + 3, CL_KERNEL_ARG_NAME, size, (void *)kernel->args[i].name, NULL);
		clGetKernelArgInfo(kernel->kernel, i + 3, CL_KERNEL_ARG_TYPE_NAME, 512, type, NULL);
		if (get_param_size(type, &kernel->args[i].size, &kernel->args[i].type) != EXIT_SUCCESS)
		{
			ft_printf("Unsuported type: \"%s\"\n", type);
			while (i-- > 0)
				free((void *)kernel->args[i].name);
			return (free(kernel->args), EXIT_FAILURE);
		}
		arg_buffer_size += kernel->args[i].size;
		printf("\t> %-10s %-15s (type: %2u, size: %2zu)\n", type, kernel->args[i].name, kernel->args[i].type, kernel->args[i].size);
		++i;
	}
	kernel->_arg_values = (char *) malloc(arg_buffer_size);
	if (kernel->_arg_values == NULL)
	{
		while (i-- > 0)
			free((void *)kernel->args[i].name);
		return (free(kernel->args), EXIT_FAILURE);
	}
	printf("\n");
	return (EXIT_SUCCESS);
}

int	build_kernels(
		t_data *data)
{
	size_t		i;
	cl_int		error_code;
	char		*name;

	name = data->cl._kernel_names;
	i = 0;
	while (i < data->cl.kernel_count)
	{
		data->cl.kernels[i].name = name;
		data->cl.kernels[i].kernel = clCreateKernel(
				data->cl.program,
				data->cl.kernels[i].name,
				&error_code);
		if (error_code != CL_SUCCESS)
			return (cleanup_kernels(data, i), EXIT_FAILURE);
		if (build_kernel(&data->cl.kernels[i]) != EXIT_SUCCESS)
			return (cleanup_kernels(data, i), EXIT_FAILURE);
		while (*name)
			++name;
		++name;
		++i;
	}
	data->cl.kernel = data->cl.kernels;
	return (EXIT_SUCCESS);
}

int	init_opencl_kernels(
		t_data *data)
{
	cl_int	error_code;
	size_t	program_size;
	char	*program;
	char	_log[LOG_SIZE + 1];

	error_code = read_file("kernels/fract-ol_kernels.cl", &program, &program_size);
	if (error_code != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	data->cl.program = clCreateProgramWithSource(data->cl.context, 1, (const char **)&program, NULL, &error_code);
	free(program);
	if (error_code != CL_SUCCESS)
		return (EXIT_FAILURE);
	error_code = clBuildProgram(data->cl.program, 1, &data->cl.device, "-cl-kernel-arg-info", NULL, NULL);
	if (error_code != CL_SUCCESS)
	{
		clGetProgramBuildInfo(data->cl.program, data->cl.device, CL_PROGRAM_BUILD_LOG, LOG_SIZE, _log, NULL);
		ft_printf("Error %d: Failed to build program executable!\nLog:\n%s\n", error_code, _log);
		return (EXIT_FAILURE);
	}
	clGetProgramInfo(data->cl.program, CL_PROGRAM_NUM_KERNELS, sizeof(size_t), &data->cl.kernel_count, NULL);
	if (data->cl.kernel_count == 0)
		return (ft_printf("No kernels.\n"), EXIT_FAILURE);
	clGetProgramInfo(data->cl.program, CL_PROGRAM_KERNEL_NAMES, 0, NULL, &data->cl._kernel_names_size);
	data->cl._kernel_names = (char *) ft_calloc(data->cl._kernel_names_size, sizeof(char));
	if (data->cl._kernel_names == NULL)
		return (EXIT_FAILURE);
	clGetProgramInfo(data->cl.program, CL_PROGRAM_KERNEL_NAMES, data->cl._kernel_names_size, data->cl._kernel_names, NULL);
	printf("Kernel names: %s\n", data->cl._kernel_names);
	parse_kernel_names(data->cl._kernel_names);
	data->cl.kernels = (t_kernel *) malloc(data->cl.kernel_count * sizeof(t_kernel));
	if (data->cl.kernels == NULL)
		return (EXIT_FAILURE);
	if (build_kernels(data) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	init_opencl(
		t_data *data)
{
	cl_int	error_code;

	if (cl_get_platform_id(&data->cl.platform) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (cl_get_device_id(data->cl.platform, &data->cl.device) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	data->cl.context = clCreateContext(NULL, 1, &data->cl.device, NULL, NULL, &error_code);
	if (error_code != CL_SUCCESS)
		return (EXIT_FAILURE);
	data->cl.command_queue = clCreateCommandQueue(data->cl.context, data->cl.device, 0, &error_code);
	if (error_code != CL_SUCCESS)
		return (EXIT_FAILURE);
	data->cl.cl_screen = clCreateBuffer(data->cl.context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, WIDTH * HEIGHT * sizeof(int), data->mlx.mlx_screen, &error_code);
	if (error_code != CL_SUCCESS)
		return (EXIT_FAILURE);
	if (init_opencl_kernels(data) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
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
	mlx_loop_hook(data.mlx.mlx, loop, &data);
	mlx_key_hook(data.mlx.window, handle_keys, &data);
	mlx_mouse_hook(data.mlx.window, handle_mouse, &data);
	mlx_loop(data.mlx.mlx);
	full_cleanup(&data);
	return (data.error.code);
}
