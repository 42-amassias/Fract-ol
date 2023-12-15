/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_wrapper.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:00:38 by amassias          #+#    #+#             */
/*   Updated: 2023/12/15 16:09:18 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_WRAPPER_H
# define MLX_WRAPPER_H

/* ************************************************************************** */
/*                                                                            */
/* Includes                                                                   */
/*                                                                            */
/* ************************************************************************** */

# include <mlx.h>

/* ************************************************************************** */
/*                                                                            */
/* Defines                                                                    */
/*                                                                            */
/* ************************************************************************** */

# define WINDOW_TITLE "fract-ol"

# define WINDOW_X_FONT "-*-*-r-normal-*-12-120-*-*-*-*-*-*"

/* ************************************************************************** */
/*                                                                            */
/* Structures                                                                 */
/*                                                                            */
/* ************************************************************************** */

struct	s_handlers
{
	int		(*update)(void *);
	int		(*render)(void *);
	int		(*mouse)(int, int, int, void *);
	int		(*keyboard)(int, void *);
	struct s_context
	{
		void	*update;
		void	*render;
		void	*mouse;
		void	*keyboard;
	}	context;
};

struct s_mlx
{
	void				*mlx;
	void				*window;
	void				*img;
	char				*screen;
	struct s_handlers	handlers;
};

/* ************************************************************************** */
/*                                                                            */
/* Types                                                                      */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_mlx		t_mlx;

typedef struct s_handlers	t_handlers;

/* ************************************************************************** */
/*                                                                            */
/* header protoypes                                                           */
/*                                                                            */
/* ************************************************************************** */

int		init_mlx(
			t_mlx *mlx,
			unsigned int width,
			unsigned int height,
			t_handlers *handlers);

void	cleanup_mlx(
			t_mlx *mlx);

#endif