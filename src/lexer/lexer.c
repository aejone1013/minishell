/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 16:11:13 by jaoh              #+#    #+#             */
/*   Updated: 2025/01/18 16:31:24 by jaoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* Will still return NULL if error,
* since we dont do any explicit pointer access,
* we have no need to check if NULL
*/
t_token	*lex_strtok(char *str, t_ctx *ctx)
{
	t_token			*new;
	t_token_type	type;
	int				len;

	new = NULL;
	type = lex_get_type(str);
	len = lex_get_len(str, type);
	new = tok_create(str, len, type, ctx);
	return (new);
}

t_token	*lexer(t_ctx *ctx, char *line)
{
	t_token	*token;
	t_token	*tmp;
	int		i;

	i = 0;
	token = NULL;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			tmp = lex_strtok(&(line[i]), ctx);
			if (tmp == NULL)
			{
				tok_free(token);
				return (NULL);
			}
			tok_add_back(&(token), tmp);
			i += ft_strlen(tmp->value);
		}
		else
			i++;
	}
	return (token);
}
