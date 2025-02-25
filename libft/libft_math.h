/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_math.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 22:41:14 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/18 01:05:41 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LIBFT_MATH_H_
#define _LIBFT_MATH_H_

int		ft_pow(int nb, int power);
float	ft_fpow(float nb, int power);
int		ft_sqrt(int nb);
float	ft_fsqrt(float nb);
int		ft_abs(int x);
float	ft_fabs(float x);

int		ft_strtol(const char* nptr, long* value);
int		ft_strtof(const char* nptr, double* value);

#endif
