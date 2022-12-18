/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 18:52:35 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/17 19:46:10 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "libft.h"

static bool push_new_node(t_list** alst, char* str)
{
	t_list* new = ft_lstnew(str);
	if (!new)
		return false;
	ft_lstadd_back(alst, new);
	return true;
}

t_command resolve_command_name(const char* name, const char** commands_supported)
{
	int i;
	size_t namelen = ft_strlen(name);
	for (i = 0; commands_supported[i]; i++)
	{
		size_t cmdlen = ft_strlen(commands_supported[i]);
		if (namelen == cmdlen && ft_strncmp(name, commands_supported[i], namelen) == 0)
			return (t_command)i;
	}
	return UNKNOWN;
}

int parse_arguments(int argc, char **argv, t_parameters* opt)
{
	bool expect_filename = false;
	bool is_string_arg = false;

	int i = 2;
	while (i < argc)
	{
		if (is_string_arg)
		{
			if (!push_new_node(&opt->strings, argv[i]))
				return OOM;
			is_string_arg = false;
		}
		else if (expect_filename)
		{
			if (!push_new_node(&opt->files, argv[i]))
				return OOM;
		}
		else
		{
			if (*argv[i] == '-')
			{
				if (ft_strlen(argv[i] + 1) != 1)
					return i;

				switch (argv[i][1])
				{
					case 'p':
						opt->print_stdin = true;
						break;
					case 'q':
						opt->quiet = true;
						break;
					case 'r':
						opt->reversed_output = true;
						break;
					case 's':
						is_string_arg = true;
						break;
					default:
						return i;
				}
			}
			else
			{
				expect_filename = true;
				continue ;
			}
		}
		i++;
	}
	return SUCCESS;
}

void debug_arguments(t_parameters* opt)
{
	ft_printf("print stdin:\t%s\n", (opt->print_stdin ? "true" : "false"));
	ft_printf("quiet output:\t%s\n", (opt->quiet ? "true" : "false"));
	ft_printf("reverse output:\t%s\n\n", (opt->reversed_output ? "true" : "false"));
	
	t_list* node;
	ft_printf("strings:\n");
	for (node = opt->strings; node; node = node->next)
		ft_printf("%s\n", (char*)node->content);

	ft_printf("\nfiles:\n");
	for (node = opt->files; node; node = node->next)
		ft_printf("%s\n", (char*)node->content);
}
