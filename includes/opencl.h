/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:14:09 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 17:54:47 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENCL_H
# define OPENCL_H

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

# define CL_USE_DEPRECATED_OPENCL_1_2_APIS
# include <CL/cl.h>

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

# define LOG_SIZE 4096

# define KERNEL_ARG_INDEX__BUFFER 0
# define KERNEL_ARG_INDEX__DX 1
# define KERNEL_ARG_INDEX__DY 2
# define KERNEL_ARG_INDEX__ZOOM 3

# define CL_TYPE_COUNT 10
# define CL_KERNEL_NEEDED_ARG_COUNT 4

/* ************************************************************************** */
/*                                                                            */
/* Enums                                                                      */
/*                                                                            */
/* ************************************************************************** */

enum e_cl_arg_type
{
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
	CL_ARG_TYPE_COUNT,
};

/* ************************************************************************** */
/*                                                                            */
/* Structures                                                                 */
/*                                                                            */
/* ************************************************************************** */

struct s_arg_info
{
	cl_uint							index;
	const char						*name;
	const char						*type;
	cl_kernel_arg_access_qualifier	qualifier;
};

struct s_cl_type
{
	const char			*str_type;
	enum e_cl_arg_type	internal_type;
	size_t				size;
};

struct s_kernel_arg
{
	const char			*name;
	size_t				size;
	enum e_cl_arg_type	type;
	void				*value;
};

struct s_kernel
{
	const char			*name;
	cl_kernel			kernel;
	cl_uint				arg_count;
	struct s_kernel_arg	*args;
	void				*_arg_values;
};

struct s_cl
{
	cl_platform_id		platform;
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			program;
	cl_mem				cl_screen;
	struct s_kernel		*current_kernel;
	size_t				kernel_count;
	struct s_kernel		*kernels;
	size_t				_kernel_names_size;
	char				*_kernel_names;
};

/* ************************************************************************** */
/*                                                                            */
/* Types                                                                      */
/*                                                                            */
/* ************************************************************************** */

typedef enum e_cl_arg_type	t_cl_arg_type;

typedef struct s_arg_info	t_arg_info;

typedef struct s_cl_type	t_cl_type;

typedef struct s_kernel_arg	t_kernel_arg;

typedef struct s_kernel		t_kernel;

typedef struct s_cl			t_cl;

/* ************************************************************************** */
/*                                                                            */
/* Global variables                                                           */
/*                                                                            */
/* ************************************************************************** */

extern const t_cl_type		g_cl_types[CL_TYPE_COUNT];
extern const t_arg_info		g_needed_kernel_args[CL_KERNEL_NEEDED_ARG_COUNT];

/* ************************************************************************** */
/*                                                                            */
/* Header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

int		init_opencl(
			t_cl *cl,
			char *screen_back_buffer,
			size_t pixel_count);

int		init_opencl_kernels(
			t_cl *cl);

int		build_kernels(
			t_cl *cl);

int		build_kernel(
			t_kernel *kernel);

int		get_param_size(
			const char *type,
			size_t *size,
			t_cl_arg_type *internal_type);

void	cleanup_kernels(
			t_cl *cl,
			size_t count);

// TODO: cleanup kernels
void	cleanup_opencl(
			t_cl *cl);

int		cl_get_kernel_info__int(
			cl_kernel kernel,
			cl_kernel_info info,
			void *value);

int		cl_get_kernel_info__str(
			cl_kernel kernel,
			cl_kernel_info info,
			void **string_ptr);

int		cl_get_kernel_arg_info__int(
			cl_kernel kernel,
			cl_uint index,
			cl_kernel_info info,
			void *value);

int		cl_get_kernel_arg_info__str(
			cl_kernel kernel,
			cl_uint index,
			cl_kernel_info info,
			void **string_ptr);

#endif