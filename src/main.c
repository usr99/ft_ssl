/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 18:40:04 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/18 19:33:32 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "ft_ssl.h"
#include "ft_printf.h"

static void print_usage(t_command* commands, const char** flags)
{
	int i;

	ft_putstr_fd("\nCommands:\n", STDERR_FILENO);
	for (i = 0; i < CMD_COUNT; i++)
		ft_putendl_fd(commands[i].name, STDERR_FILENO);

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
	static t_command COMMANDS[] = {
		{ .name = "md5", .function = &md5 },
		{ .name = "sha256", .function = &sha256 }
	};

	static const char* FLAGS[] = {
		"-p", "-q", "-r", "-s",
		NULL
	};

	t_parameters params = {0};
	bool error = false;
	
	if (argc > 1)
	{
		t_command* cmd = resolve_command_name(argv[1], COMMANDS);
		if (!cmd)
		{
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

		if (params.print_stdin || (!params.strings && !params.files))
		{
			if (!error && process_stdin(&params, cmd) != SUCCESS)
				error = true;
		}
		if (!error && process_strings(&params, cmd) != SUCCESS)
			error = true;
		if (!error && process_files(&params, cmd) != SUCCESS)
			error = true;
	}
	else
	{
		ft_putendl_fd("usage: ft_ssl command [flags] [file/string]", STDERR_FILENO);
		error = true;
	}

	ft_lstclear(&params.files, NULL);
	ft_lstclear(&params.strings, NULL);
	return (!error ? EXIT_SUCCESS : EXIT_FAILURE);
}
