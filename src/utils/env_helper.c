/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 16:42:57 by jaoh              #+#    #+#             */
/*   Updated: 2025/02/10 17:33:53 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_default_env(void)
{
	char	*id;
	char	*value;
	t_env	*def;

	id = env_get_id(DEF_ENV);
	value = env_get_value(DEF_ENV);
	def = env_create(id, value, ft_strdup(DEF_ENV));
	return (def);
}
