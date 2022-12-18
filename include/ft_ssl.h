/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 18:41:47 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/17 19:46:03 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SSL_H_
# define _FT_SSL_H_

# include <stdbool.h>
# include "libft.h"

typedef enum
{
	MD5,
	SHA256,
	UNKNOWN
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
	SUCCESS
} t_error;

/* Parameters management */
t_command resolve_command_name(const char* name, const char** commands_supported);
int parse_arguments(int argc, char **argv, t_parameters* opt);
void debug_arguments(t_parameters* opt);

#endif
