/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 04:13:32 by amassias          #+#    #+#             */
/*   Updated: 2023/12/26 18:40:47 by amassias         ###   ########.fr       */
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
# include <stdbool.h>

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief The number of kernel arguments that the user won't be able to access.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-26
 */
# define KERNEL_HIDDEN_ARGUMENT_COUNT 3

/* ************************************************************************** */
/*                                                                            */
/* Enums                                                                      */
/*                                                                            */
/* ************************************************************************** */

/**
 * @enum e_kernel_argument_type
 * Each constant corresponds to a type defined by opencl. See table bellow.
 * @brief Defines all the types that a kernel parameter can accept.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
enum e_kernel_argument_type
{
	/**cl_char**/
	ARG_TYPE__CHAR,
	/**cl_short**/
	ARG_TYPE__SHORT,
	/**cl_int**/
	ARG_TYPE__INT,
	/**cl_long**/
	ARG_TYPE__LONG,
	/**cl_uchar**/
	ARG_TYPE__UCHAR,
	/**cl_ushort**/
	ARG_TYPE__USHORT,
	/**cl_uint**/
	ARG_TYPE__UINT,
	/**cl_ulong**/
	ARG_TYPE__ULONG,
	/**cl_float**/
	ARG_TYPE__FLOAT,
	/**cl_double**/
	ARG_TYPE__DOUBLE,
	/**The number of types supported**/
	_ARG_TYPE_COUNT
};

/**
 * @enum e_cl_code
 * See table bellow for code signification.
 * @brief Exit codes for this opencl wrapper.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
enum e_cl_code
{
	/**Success**/
	CL_CODE_SUCCESS,
	/**No OpenCL platform is available**/
	CL_ERR_NO_PLATFORM,
	/**No OpenCL device found for current platform**/
	CL_ERR_NO_DEVICE,
	/**Failed to initialize the OpenCL context**/
	CL_ERR_CONTEXT_INITIALIZATION_FAILURE,
	/**Failed to initialize a command queue for an OpenCL device**/
	CL_ERR_COMMAND_QUEUE_INITIALIZATION_FAILURE,
	CL_ERR_PROGRAM_INITIALIZATION_FAILURE,
	CL_ERR_PROGRAM_BUILD_FAILURE,
	CL_ERR_KERNEL_INITIALIZATION_FAILURE,
	CL_ERR_KERNEL_CREATION_FAILURE,
	CL_ERR_KERNEL_BUILD_FAILURE,
	CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_NAME,
	CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_TYPE,
	CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_TYPE_QUALIFIER,
	CL_ERR_KERNEL_INVALID_MANDATOTY_ARGUMENT,
	CL_ERR_KERNEL_MISSING_ARGUMENTS,
	CL_ERR_KERNEL_BUILD_ARGUMENT_INVALID_TYPE,
	CL_ERR_INFO_QUERY_FAILURE,
	CL_ERR_CANNOT_GET_KERNEL_INFO,
	CL_ERR_CANNOT_GET_KERNEL_ARG_INFO,
	CL_ERR_OUT_OF_MEMORY,
};

enum e_kma
{
	KMA__SCREEN,
	KMA__WIDTH,
	KMA__HEIGHT,
	KMA__DX,
	KMA__DY,
	KMA__ZOOM,
	_KMA_COUNT,
};

/* ************************************************************************** */
/*                                                                            */
/* Structures                                                                 */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Data relative to an opencl kernel argument type.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
struct s_kernel_argument_type_data
{
	/**
	 * @brief The internal associated type.
	 */
	enum e_kernel_argument_type	type;
	/**
	 * This string comes from <b>clGetKernelArgInfo</b> on
	 * <b>CL_KERNEL_ARG_TYPE_NAME</b>.
	 * @brief The string representation of this data type.
	 */
	const char					*litteral_name;
	/**
	 * @brief The size in bytes of this data type.
	 */
	size_t						size;
};

struct s_kma_data
{
	size_t							position;
	const char						*name;
	const char						*type;
	cl_kernel_arg_access_qualifier	type_qualifier;
};

/**
 * @struct s_kernel_argument
 * @brief All the data relative to an opencl kernel parameter.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
struct s_kernel_argument
{
	const char					*name;
	const char					*type_name;
	enum e_kernel_argument_type	type;
	size_t						size;
	void						*data;
	bool						need_update_on_device;
};

/**
 * @struct s_kernel
 * @brief All the data relative to an opencl kernel.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 */
struct s_kernel
{
	const char					*name;
	cl_kernel					cl;
	cl_uint						arg_count;
	struct s_kernel_argument	*args;
	void						*__int__args_data;
};

/**
 * @struct s_cl
 * @brief All the data relative to an opencl context.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
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
	char				*_kernel_names;
};

/* ************************************************************************** */
/*                                                                            */
/* Types                                                                      */
/*                                                                            */
/* ************************************************************************** */

/**
 * @typedef t_kernel_argument_type
 * @brief Type wrapper for `e_kernel_argument_type`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see e_kernel_argument_type
 */
typedef enum e_kernel_argument_type				t_kernel_argument_type;

/**
 * @typedef t_kma
 * @brief Type wrapper for `e_kma`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see e_kma
 */
typedef enum e_kma								t_kma;

/**
 * @typedef t_cl_code
 * @brief Type wrapper for `e_cl_code`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see e_cl_code
 */
typedef enum e_cl_code							t_cl_code;

/**
 * @typedef t_kernel_argument_type_data
 * @brief Type wrapper for `e_kernel_argument_type_data`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-26
 * @see e_kernel_argument_type_data
 */
typedef struct s_kernel_argument_type_data		t_kernel_argument_type_data;

/**
 * @typedef t_kma_data
 * @brief Type wrapper for `s_kma_data`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-26
 * @see s_kma_data
 */
typedef struct s_kma_data						t_kma_data;

/**
 * @typedef t_kernel_argument
 * @brief Type wrapper for `s_kernel_argument`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see s_kernel_argument
 */
typedef struct s_kernel_argument				t_kernel_argument;

/**
 * @typedef t_kernel
 * @brief Type wrapper for `s_kernel`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see s_kernel
 */
typedef struct s_kernel							t_kernel;

/**
 * @typedef t_cl
 * @brief Type wrapper for `s_cl`.
 * @author Antoine Massias (amassias@student.42lehavre.fr)
 * @date 2023-12-25
 * @see s_cl
 */
typedef struct s_cl								t_cl;

/* ************************************************************************** */
/*                                                                            */
/* Global variables                                                           */
/*                                                                            */
/* ************************************************************************** */

extern const t_kernel_argument_type_data		g_kernel_arg_types
[_ARG_TYPE_COUNT];

extern const t_kma_data							g_kernel_mandatory_args
[_KMA_COUNT];

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
 * @retval CL_ERR_NO_PLATFORM if no platform is found.
 * @retval CL_ERR_NO_DEVICE if no device if found.
 * @retval CL_ERR_CONTEXT_INITIALIZATION_FAILURE if the context has failed to
 * initialize.
 * @retval CL_ERR_COMMAND_QUEUE_INITIALIZATION_FAILURE if the command queue has
 * failed to initialize.
 * @retval CL_ERR_PROGRAM_INITIALIZATION_FAILURE if the program has failed to
 * initalize.
 * @retval CL_ERR_KERNEL_CREATION_FAILURE if <b>clCreateKernel</b> has failed.
 * @retval CL_ERR_CANNOT_GET_KERNEL_INFO if OpenCL failed to query a kernel
 * information.
 * @retval CL_ERR_CANNOT_GET_KERNEL_ARG_INFO if OpenCL failed to query a kernel
 * argument information.
 * @retval CL_ERR_KERNEL_MISSING_ARGUMENTS if a kernel is missing a mandatory
 * argument.
 * @retval CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_NAME if a kernel
 * mandatory argument has the wrong name.
 * @retval CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_TYPE if a kernel
 * mandatory argument has the wrong type.
 * @retval CL_ERR_KERNEL_BUILD_MANDATORY_ARGUMENT_INVALID_TYPE_QUALIFIER if a
 * kernel mandatory argument has the wrong type qualifier.
 * @retval CL_ERR_KERNEL_BUILD_ARGUMENT_INVALID_TYPE if a kernel argument 
 * @retval CL_ERR_OUT_OF_MEMORY if the host has ran out of memory.
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