/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 04:13:32 by amassias          #+#    #+#             */
/*   Updated: 2023/12/25 07:05:29 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file opencl.h
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @copyright Copyright (c) 2023
 */

#ifndef OPENCL_H
# define OPENCL_H

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

# include <CL/cl.h>

/* ************************************************************************** */
/*                                                                            */
/* Enums                                                                      */
/*                                                                            */
/* ************************************************************************** */

/**
 * @enum e_parameter_type
 * Each constant corresponds to a type defined by opencl. See table bellow.
 * @brief Defines all the types that a kernel parameter can accept.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-11-25
 */
enum e_parameter_type
{
	/**cl_char**/
	PARAM_TYPE__CHAR,
	/**cl_short**/
	PARAM_TYPE__SHORT,
	/**cl_int**/
	PARAM_TYPE__INT,
	/**cl_long**/
	PARAM_TYPE__LONG,
	/**cl_uchar**/
	PARAM_TYPE__UCHAR,
	/**cl_ushort**/
	PARAM_TYPE__USHORT,
	/**cl_uint**/
	PARAM_TYPE__UINT,
	/**cl_ulong**/
	PARAM_TYPE__ULONG,
	/**cl_float**/
	PARAM_TYPE__FLOAT,
	/**cl_double**/
	PARAM_TYPE__DOUBLE,
	/**The number of types supported**/
	PARAM_TYPE_COUNT
};

/**
 * @enum e_cl_code
 * See table bellow for code signification.
 * @brief Exit codes for this opencl wrapper.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-11-25
 */
enum e_cl_code
{
	/**Success**/
	CL_CODE_SUCCESS,
	/**No OpenCL platform is available**/
	CL_CODE_NO_PLATFORM,
	/**No OpenCL device found for current platform**/
	CL_CODE_NO_DEVICE,
	/**Failed to initialize the OpenCL context**/
	CL_CODE_CONTEXT_INITIALIZATION_FAILURE,
	/**Failed to initialize a command queue for an OpenCL device**/
	CL_CODE_COMMAND_QUEUE_INITIALIZATION_FAILURE,
	CL_CODE_PROGRAM_INITIALIZATION_FAILURE,
	CL_CODE_PROGRAM_BUILD_FAILURE,
};

/* ************************************************************************** */
/*                                                                            */
/* Structures                                                                 */
/*                                                                            */
/* ************************************************************************** */

/**
 * @struct s_parameter
 * @brief All the data relative to an opencl kernel parameter.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-11-25
 */
struct s_parameter
{
	const char				*name;
	const char				*type_name;
	enum e_parameter_type	type;
	size_t					size;
	void					*data;
};

/**
 * @struct s_kernel
 * @brief All the data relative to an opencl kernel.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-11-25
 */
struct s_kernel
{
	const char			*name;
	cl_kernel			cl;
	size_t				parameter_count;
	struct s_parameter	*parameters;
	void				*__int__param_data;
};

/**
 * @struct s_cl
 * @brief All the data relative to an opencl context.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-11-25
 */
struct s_cl
{
	cl_platform_id		platform;
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			program;
	struct s_kernel		*active_kernel;
	size_t				kernel_count;
	struct s_kernel		*kernels;
	struct s_screen
	{
		unsigned int		width;
		unsigned int		height;
		cl_mem				device_buffer;
		void				*host_screen;
	}					screen;
};

/* ************************************************************************** */
/*                                                                            */
/* Types                                                                      */
/*                                                                            */
/* ************************************************************************** */

/**
 * @typedef t_parameter_type
 * @brief Type wrapper for `e_parameter_type`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see e_parameter_type
 */
typedef enum e_parameter_type	t_parameter_type;

/**
 * @typedef t_cl_code
 * @brief Type wrapper for `e_cl_code`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see e_cl_code
 */
typedef enum e_cl_code			t_cl_code;

/**
 * @typedef t_parameter
 * @brief Type wrapper for `s_parameter`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see s_parameter
 */
typedef struct s_parameter		t_parameter;

/**
 * @typedef t_kernel
 * @brief Type wrapper for `s_kernel`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see s_kernel
 */
typedef struct s_kernel			t_kernel;

/**
 * @typedef t_cl
 * @brief Type wrapper for `s_cl`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see s_cl
 */
typedef struct s_cl				t_cl;

/* ************************************************************************** */
/*                                                                            */
/* Header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

/**
 * If an error occurs, the opencl context will be freed.
 * @brief Initializes an opencl context.
 * @param cl A valid pointer to a non initialized opencl context.
 * @returns CL_CODE_SUCCESS on success, an error otherwise.
 * @retval CL_CODE_NO_PLATFORM if no platform is found.
 * @retval CL_CODE_NO_DEVICE if no device if found.
 * @retval CL_CODE_CONTEXT_INITIALIZATION_FAILURE if the context has failed to
 * initialize.
 * @retval CL_CODE_COMMAND_QUEUE_INITIALIZATION_FAILURE if the command queue has
 * failed to initialize.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
t_cl_code	opencl_init(
				t_cl *cl
				);

/**
 * @brief 
 * 
 * @param cl 
 * @param width 
 * @param height 
 * @param host_screen_buffer 
 * @return t_cl_code 
 */
t_cl_code	opencl_update_host_screen_buffer(
				t_cl *cl,
				unsigned int width,
				unsigned int height,
				void *host_screen_buffer
				);

t_cl_code	opencl_run_active_kernel(
				t_cl *cl
				);

t_cl_code	opencl_read_screen_buffer(
				t_cl *cl
				);

void		opencl_cleanup(
				t_cl *cl
				);

#endif