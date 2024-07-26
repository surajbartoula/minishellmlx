/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 21:50:17 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/26 00:53:37 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_set_parse_err(t_parse_err_type type, t_minishell *g_minishell)
{
	g_minishell->parse_err.type = type;
}

void	ft_handle_parse_err(t_minishell *g_minishell)
{
	t_parse_err_type	type;
	t_token_type		token_type;
	char				**types;

	types = (char *[]){"T_IDENTIFIER",
		"<", ">", "<<", ">>", "|", "(", ")", "&&", "||", "newline"};
	type = g_minishell->parse_err.type;
	(void)token_type;
	(void)types;
	if (type)
	{
		if (type == E_SYNTAX)
		{
			if (!g_minishell->curr_token)
				token_type = T_NL;
			else
				token_type = g_minishell->curr_token->type;
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(types[token_type], 2);
			ft_putstr_fd("'\n", 2);
			g_minishell->exit_s = 258;
		}
		ft_clear_ast(&g_minishell->ast, g_minishell);
		ft_bzero(&g_minishell->parse_err, sizeof(t_parse_err));
	}
}
