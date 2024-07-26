/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 21:52:56 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/26 10:59:23 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_tokenize(t_minishell *g_minishell)
{
	char	*line;
	t_token	*token_list;

	line = g_minishell->line;
	token_list = ft_tokenization_handler(line, g_minishell);
	free(line);
	g_minishell->line = NULL;
	return (token_list);
}
