/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 18:41:47 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/18 19:40:27 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SSL_H_
# define _FT_SSL_H_

# include <stdbool.h>
# include "libft.h"

# define CMD_COUNT 2

typedef struct
{
	uint8_t* digest;
	uint8_t size;
} t_hash;

typedef struct
{
	const char* name;
	t_hash* (*function)(const char*);
} t_command;

typedef struct
{
	t_list* strings;
	t_list* files;

	bool print_stdin;
	bool quiet;
	bool reversed_output;
} t_parameters;

typedef enum
{
	OOM = -1,
	SUCCESS,
	READ_FAILURE
} t_error;

/* Parameters management */
t_command* resolve_command_name(const char* name, t_command* commands_supported);
int parse_arguments(int argc, char **argv, t_parameters* opt);
void debug_arguments(t_parameters* opt);

/* Inputs and Ouputs */
int process_stdin(t_parameters* opt, t_command* cmd);
int process_strings(t_parameters* opt, t_command* cmd);
int process_files(t_parameters* opt, t_command* cmd);

/* Hashing algorithms */
t_hash* md5(const char* src);
t_hash* sha256(const char* src);

#endif
