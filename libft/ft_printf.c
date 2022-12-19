/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/02 12:52:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/19 01:06:31 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "mandatory.h"

int		ft_dprintf(int fd, const char* format, ...)
{
	va_list	ap;
	int		i;
	int		size;

	i = 0;
	size = 0;
	if (!format)
		return (-1);
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] != '%')
			ft_putchar_fd(format[i], fd);
		else if (format[i + 1] == '%')
			ft_putchar_fd(format[i++], fd);
		else
		{
			if (!convert_arg(fd, format + i + 1, ap, &i, &size))
				return (-1);
		}
		i++;
		size++;
	}
	va_end(ap);
	return (size);
}

int		ft_printf(const char *format, ...)
{
	va_list	ap;
	int		i;
	int		size;

	i = 0;
	size = 0;
	if (!format)
		return (-1);
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] != '%')
			ft_putchar_fd(format[i], STDOUT_FILENO);
		else if (format[i + 1] == '%')
			ft_putchar_fd(format[i++], STDOUT_FILENO);
		else
		{
			if (!convert_arg(STDOUT_FILENO, format + i + 1, ap, &i, &size))
				return (-1);
		}
		i++;
		size++;
	}
	va_end(ap);
	return (size);
}

int		convert_arg(int fd, const char *format, va_list ap, int *i, int *size)
{
	t_flags	flags;
	int		ret;

	flags.fd = fd;
	flags = read_flags(format, ap);
	flags.fd = fd;
	*i += flags.size;
	if (flags.type == '%')
	{
		if (flags.is_0_padded && !flags.is_left_adjusted)
			print("%", 1, flags, '0');
		else
			print("%", 1, flags, ' ');
		*size += get_nb_char_printed(1, flags.width) - 1;
	}
	else if (flags.type != 0)
	{
		if ((ret = print_arg(flags, ap)) == -1)
			return (0);
		*size += ret - 1;
	}
	return (1);
}

int		print_arg(t_flags flags, va_list ap)
{
	int	ret;

	if (flags.precision < -1)
		flags.precision = -1;
	if (ft_strchr("di", flags.type))
		ret = print_int(flags, ap);
	else if (ft_strchr("uxX", flags.type))
		ret = print_uint(flags, ap);
	else if (flags.type == 's')
		ret = print_str(flags, ap);
	else if (flags.type == 'p')
		ret = print_ptr(flags, ap);
	else if (flags.type == 'c')
		ret = print_char(flags, ap);
	else
		ret = 1;
	return (ret);
}
