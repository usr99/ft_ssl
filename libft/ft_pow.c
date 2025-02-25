/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 20:53:10 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/18 01:06:22 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_math.h"

int	ft_pow(int nb, int power)
{
	if (power > 0)
		return (nb * ft_pow(nb, power - 1));
	else if (power == 0)
		return (1);
	else
		return (0);
}

float	ft_fpow(float nb, int power)
{
	if (power > 0)
		return (nb * ft_fpow(nb, power - 1));
	else if (power == 0)
		return (1);
	else
		return (0);
}
