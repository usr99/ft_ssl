/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 20:48:31 by mamartin          #+#    #+#             */
/*   Updated: 2022/12/17 20:51:06 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

char* md5(const char* src)
{
	(void)src;
	return "MD5-f1we86qg34s3sq845d";
}

char* sha256(const char* src)
{
	(void)src;
	return "SHA256-f1we86qg34s3sq845d";
}
