/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_appenders.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 20:45:29 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/26 10:39:45 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_append_separator(t_token_type type, char **line_ptr,
	t_token **token_list)
{
	t_token	*token;

	token = ft_new_token(NULL, type);
	if (!token)
		return (0);
	ft_token_list_add_back(token_list, token);
	(*line_ptr)++;
	if (type == T_DLESS || type == T_DGREAT || type == T_OR || type == T_AND)
		(*line_ptr)++;
	return (1);
}

int	ft_append_identifier(char **line_ptr, t_token **token_list, t_minishell *g_minishell)
{
	char	*tmp_line;
	char	*value;
	t_token	*token;
	size_t	i;

	tmp_line = *line_ptr;
	i = 0;
	while (tmp_line[i] && !ft_is_separator(tmp_line + i))
	{
		if (ft_is_quote(tmp_line[i]))
		{
			if (!ft_skip_quotes(tmp_line, &i))
				return (ft_print_quote_err(tmp_line[i], g_minishell), 0);
		}
		else
			i++;
	}
	value = ft_substr(tmp_line, 0, i);
	if (!value)
		return (0);
	token = ft_new_token(value, T_IDENTIFIER);
	if (!token)
		return (free(value), 0);
	*line_ptr += i;
	return (ft_token_list_add_back(token_list, token), 1);
}
