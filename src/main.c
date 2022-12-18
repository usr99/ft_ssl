/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 18:40:04 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/17 19:46:17 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ft_ssl.h"
#include "ft_printf.h"

static void print_usage(const char** commands, const char** flags)
{
	int i;

	ft_putstr_fd("\nCommands:\n", STDERR_FILENO);
	for (i = 0; commands[i]; i++)
		ft_putendl_fd(commands[i], STDERR_FILENO);

	ft_putstr_fd("\nFlags:\n", STDERR_FILENO);
	for (i = 0; flags[i]; i++)
	{
		ft_putstr_fd(flags[i], STDERR_FILENO);
		ft_putchar_fd(' ', STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDERR_FILENO);
}

int main(int argc, char** argv)
{
	static const char* COMMANDS[] = {
		"md5",
		"sha256",
		NULL
	};

	static const char* FLAGS[] = {
		"-p", "-q", "-r", "-s",
		NULL
	};

	t_parameters params = {0};
	bool error = false;
	
	if (argc > 1)
	{
		t_command cmd = resolve_command_name(argv[1], COMMANDS);
		if (cmd == UNKNOWN)
		{
			// had to edit ft_printf to write on stderr because dprintf is not allowed :/
			ft_printf("ft_ssl: %s: invalid command\n", argv[1]);
			print_usage(COMMANDS, FLAGS);
			error = true;
		}

		int ret = parse_arguments(argc, argv, &params);
		if (ret != SUCCESS)
		{
			if (ret == OOM)
				ft_putendl_fd("ft_ssl: out of memory", STDERR_FILENO);
			else
			{
				ft_printf("ft_ssl: %s: invalid flag\n", argv[ret]);
				print_usage(COMMANDS, FLAGS);
			}
			error = true;
		}
	}
	else
	{
		ft_putendl_fd("usage: ft_ssl command [flags] [file/string]", STDERR_FILENO);
		error = true;
	}
	
	debug_arguments(&params);

	ft_lstclear(&params.files, NULL);
	ft_lstclear(&params.strings, NULL);
	return (!error ? EXIT_SUCCESS : EXIT_FAILURE);
}
