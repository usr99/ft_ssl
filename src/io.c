/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 20:34:17 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/19 16:26:48 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "ft_ssl.h"

static char* _read(int fd, int* fsize)
{
	const int READ_SIZE = 5000;

	int size = 0;
	char buffer[READ_SIZE] = {0};
	char *tmp = NULL;
	char *text = NULL;

	while ((*fsize = read(fd, buffer, READ_SIZE)) > 0)
	{
		int oldsize = size;
		size += *fsize * sizeof(char);
		text = malloc(size + 1);
		if (!text)
		{
			free(tmp);
			return NULL;
		}

		if (tmp)
		{
			ft_memcpy(text, tmp, oldsize);
			free(tmp);
		}
		ft_memcpy(text + oldsize, buffer, *fsize);
		tmp = text;
	}

	if (*fsize < 0)
	{
		free(text);
		return NULL;
	}

	if (!text)
	{
		text = ft_calloc(1, 1);
		*fsize = 0;
	}
	else
	{
		text[size] = '\0';
		*fsize = size;
	}
	return text;
}

static void trim(char* str)
{
	size_t last = ft_strlen(str) - 1;
	if (last && str[last] == '\n')
		str[last] = '\0';
}

static void output(t_hash* result, const char* src, const char* cmdname, bool quotes, bool stdin, t_parameters* opt)
{
	char q = (quotes ? '\"' : '\0');
	char s = (!stdin ? ' ' : '\0');
	bool reverse = (opt->reversed_output && !stdin);

	if (!opt->quiet && !reverse)
	{
		if (!stdin)
		{
			for (; *cmdname; cmdname++)
				ft_putchar(ft_toupper(*cmdname));
			ft_putchar(' ');
		}
		ft_printf("(%c%s%c)%c= ", q, src, q, s);
	}
	else if (opt->quiet && stdin && opt->print_stdin)
		ft_printf("%s\n", src);

	for (int i = 0; i < result->size; i++)
		ft_printf("%02x", result->digest[i]);

	if (!opt->quiet && reverse)
		ft_printf(" %c%s%c", q, src, q);
	ft_putchar('\n');
}

int process_stdin(t_parameters* opt, t_command* cmd)
{
	int size;
	char *text = _read(STDIN_FILENO, &size);
	if (!text)
	{
		if (size == 0)
			return OOM;
		else
			return READ_FAILURE;
	}

	t_hash *result = cmd->function(text, size);
	if (!result)
	{
		free(text);
		return OOM;
	}

	if (opt->print_stdin)
	{
		trim(text);
		output(result, text, cmd->name, true, true, opt);
	}
	else
		output(result, "stdin", cmd->name, false, true, opt);

	free(text);
	free(result->digest);
	free(result);
	return SUCCESS;
}

int process_strings(t_parameters* opt, t_command* cmd)
{
	t_list* str;
	for (str = opt->strings; str; str = str->next)
	{
		t_hash* result = cmd->function(str->content, ft_strlen(str->content));
		if (!result)
			return OOM;

		output(result, str->content, cmd->name, true, false, opt);
		free(result->digest);
		free(result);
	}
	return SUCCESS;
}

int process_files(t_parameters* opt, t_command* cmd)
{
	t_list* file;
	for (file = opt->files; file; file = file->next)
	{
		int fd = open(file->content, O_RDONLY);
		if (fd == -1)
		{
			ft_dprintf(STDERR_FILENO, "ft_ssl: %s: %s: %s\n", cmd->name, file->content, strerror(errno));
			continue ;
		}

		int size;
		char* text = _read(fd, &size);
		close(fd);
		if (!text)
		{
			if (size == 0)
				return OOM;
			else
				return READ_FAILURE;
		}

		t_hash* result = cmd->function(text, size);
		free(text);
		if (!result)
			return OOM;

		output(result, file->content, cmd->name, false, false, opt);
		free(result->digest);
		free(result);
	}
	return SUCCESS;
}
