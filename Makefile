# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amassias <amassias@student.42lehavre.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/23 21:43:21 by amassias          #+#    #+#              #
#    Updated: 2023/12/14 21:35:08 by amassias         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC				:=	cc
NAME			:=	fractol
THIRD_PARTY_DIR	:=	third-party
SRC_DIR			:=	sources
INC_DIR			:=	includes
BIN_DIR			:=	binaries

# **************************************************************************** #
#                                                                              #
# Third party libraries                                                        #
#                                                                              #
# **************************************************************************** #

LIB_FT_PATH		:=	$(THIRD_PARTY_DIR)/libft-full
LIB_FT			:=	$(LIB_FT_PATH)/libftfull.a

LIB_MLX_PATH	:=	$(THIRD_PARTY_DIR)/minilibx-linux
LIB_MLX			:=	$(LIB_MLX_PATH)/libmlx.a

# **************************************************************************** #
#                                                                              #
# Compiler flags                                                               #
#                                                                              #
# **************************************************************************** #

CFLAGS			:=					\
	-I$(INC_DIR)					\
	-I$(LIB_FT_PATH)				\
	-I$(LIB_MLX_PATH)				\
	-DCL_TARGET_OPENCL_VERSION=210	\
	-Wall -Werror -Wextra -g
LFLAGS			:=					\
	-L$(LIB_FT_PATH)				\
	-L$(LIB_MLX_PATH)				\
	-lftfull -lOpenCL -lmlx -lXext -lX11 -lm -lz

# **************************************************************************** #
#                                                                              #
# Files                                                                        #
#                                                                              #
# **************************************************************************** #

FILES			:=		\
	main				\
	globals				\
	cl/build_kernel		\
	cl/build_kernels	\
	cl/cleanup_kernels	\
	cl/init_kernels		\
	cl/init_opencl		\
	cl/kernel_arg_info	\
	cl/kernel_info		\

SRCS			:=	$(addprefix $(SRC_DIR)/,$(addsuffix .c,$(FILES)))

OBJS			:=	$(addprefix $(BIN_DIR)/,$(addsuffix .o,$(FILES)))

# **************************************************************************** #
#                                                                              #
# Phony rules                                                                  #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re cleanlibs fcleanlibs

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

cleanlibs:
	$(MAKE) clean -C $(LIB_FT_PATH)
	$(MAKE) clean -C $(LIB_MLX_PATH)

fcleanlibs:
	$(MAKE) fclean -C $(LIB_FT_PATH)
	$(MAKE) clean -C $(LIB_MLX_PATH)

re:	fclean all

# **************************************************************************** #
#                                                                              #
# Binary rules                                                                 #
#                                                                              #
# **************************************************************************** #

$(NAME): $(LIB_FT) $(LIB_MLX) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)

$(LIB_FT):
	$(MAKE) -C $(LIB_FT_PATH)

$(LIB_MLX):
	$(MAKE) -C $(LIB_MLX_PATH)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS)