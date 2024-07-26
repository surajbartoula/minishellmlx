/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean_ms.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 11:51:13 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/26 00:53:54 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_clear_envlst(t_minishell *g_minishell)
{
	t_env	*envlst;
	t_env	*envlst_tofree;

	envlst = g_minishell->envlst;
	while (envlst)
	{
		envlst_tofree = envlst;
		envlst = envlst->next;
		free(envlst_tofree);
	}
	g_minishell->envlst = NULL;
}

void	ft_clean_ms(t_minishell *g_minishell)
{
	ft_garbage_collector(NULL, true);
	ft_clear_ast(&g_minishell->ast, g_minishell);
	ft_clear_envlst(g_minishell);
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSANOW, &g_sig_handler.original_term);
}
