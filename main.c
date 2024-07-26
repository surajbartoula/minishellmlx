/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 01:28:41 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/26 11:13:21 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_minishell(char **env, t_minishell *g_minishell)
{
	ft_memset(g_minishell, 0, sizeof(t_minishell));
	g_minishell->environ = env;
	ft_init_envlst(g_minishell);
	g_minishell->stdin = dup(0);
	g_minishell->stdout = dup(1);
	tcgetattr(STDIN_FILENO, &g_sig_handler.original_term);
}

static void	ft_start_execution(t_minishell *g_minishell)
{
	signal(SIGQUIT, ft_sigquit_handler);
	ft_init_tree(g_minishell->ast, g_minishell);
	if (g_sig_handler.heredoc_sgint)
	{
		ft_clear_ast(&g_minishell->ast, g_minishell);
		g_sig_handler.heredoc_sgint = false;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &g_sig_handler.original_term);
	g_minishell->exit_s = ft_exec_node(g_minishell->ast, false, g_minishell);
	ft_clear_ast(&g_minishell->ast, g_minishell);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	g_minishell;

	((void)argc, (void)argv);
	ft_init_minishell(env, &g_minishell);
	while (1)
	{
		ft_init_signals();
		g_minishell.line = readline(PROMPT);
		if (!g_minishell.line)
			(ft_clean_ms(&g_minishell),
				ft_putstr_fd("exit\n", 1), exit(g_minishell.exit_s));
		if (g_minishell.line[0])
			add_history(g_minishell.line);
		g_minishell.tokens = ft_tokenize(&g_minishell);
		if (!g_minishell.tokens)
			continue ;
		g_minishell.ast = ft_parse(&g_minishell);
		if (g_minishell.parse_err.type)
		{
			ft_handle_parse_err(&g_minishell);
			continue ;
		}
		ft_start_execution(&g_minishell);
	}
	ft_garbage_collector(NULL, true);
	return (ft_clean_ms(&g_minishell), g_minishell.exit_s);
}
