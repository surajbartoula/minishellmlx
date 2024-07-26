/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbartoul <sbartoul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 01:32:13 by abelayad          #+#    #+#             */
/*   Updated: 2024/07/26 12:50:13 by sbartoul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <limits.h>
# include "libft.h"
# include "tokenizing.h"
# include "parsing.h"

int	sig_pipe[2];

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_err_msg
{
	ERRMSG_CMD_NOT_FOUND,
	ERRMSG_NO_SUCH_FILE,
	ERRMSG_PERM_DENIED,
	ERRMSG_AMBIGUOUS,
	ERRMSG_TOO_MANY_ARGS,
	ERRMSG_NUMERIC_REQUI
}	t_err_msg;

typedef enum e_err_no
{
	ENO_SUCCESS,
	ENO_GENERAL,
	ENO_CANT_EXEC = 126,
	ENO_NOT_FOUND,
	ENO_EXEC_255 = 255
}	t_err_no;

typedef enum e_ast_direction
{
	TD_LEFT,
	TD_RIGHT
}	t_ast_direction;

typedef struct s_err
{
	t_err_no	no;
	t_err_msg	msg;
	char		*cause;
}	t_err;

typedef struct s_path
{
	t_err	err;
	char	*path;
}	t_path;

typedef struct s_minishell
{
	char			*line;
	t_token			*tokens;
	t_token			*curr_token;
	int				exit_s;
	t_parse_err		parse_err;
	int				stdin;
	int				stdout;
	char			**environ;
	t_node			*ast;
	t_env			*envlst;
}					t_minishell;

typedef struct
{
	t_minishell *minishell;
}	sigaction_context_t;

typedef struct	s_signal_handler
{
	bool			heredoc_sgint;
	bool			signint_child;
	struct 	termios	original_term;
	bool			cleanup_needed;
}	t_signal_handler;

t_signal_handler g_sig_handler;

/* ****************************   BUILTINS   ****************************** */
// * cd.c ********************************************************************
int		ft_cd(char *path, t_minishell *g_minishell);

// * echo.c ******************************************************************
int		ft_echo(char **args);

// * env_utils.c *************************************************************
char	*ft_get_envlst_val(char *key, t_minishell *g_minishell);
bool	ft_env_entry_exists(char *key, t_minishell *g_minishell);
void	ft_update_envlst(char *key, char *value, bool create, t_minishell *g_minishell);

// * env.c *******************************************************************
char	*ft_extract_key(char *str);
char	*ft_extract_value(char *str);
void	ft_init_envlst(t_minishell *g_minishell);
int		ft_env(t_minishell *g_minishell);

// * exit.c ******************************************************************
void	ft_exit(char **args, t_minishell *g_minishell);

// * export.c ****************************************************************
int		ft_check_key(char *str);
int		ft_export(char **argv, t_minishell *g_minishell);

// * pwd.c *******************************************************************
int		ft_pwd(void);

// * unset.c *****************************************************************
int		ft_unset(char **args, t_minishell *g_minishell);

/* ****************************   CLEANING   ****************************** */
// * ft_clean_ms.c ***********************************************************
void	ft_clean_ms(t_minishell *g_minishell);

/* ******************************   EXEC   ******************************** */
// * error_msg.c *************************************************************
int		ft_err_msg(t_err err);

// * exec_builtin.c **********************************************************
int		ft_exec_builtin(char **args, t_minishell *g_minishell);
bool	ft_is_builtin(char *arg);

// * exec_redirect.c *********************************************************
int		ft_out(t_io_node *io_list, int *status);
int		ft_in(t_io_node *io_list, int *status);
int		ft_append(t_io_node *io_list, int *status);

// * exec_utils.c ************************************************************
void	*ft_garbage_collector(void *ptr, bool clean);
bool	ft_is_delimiter(char *delimiter, char *str);

// * exec.c ******************************************************************
int		ft_get_exit_status(int status);
int		ft_exec_node(t_node *tree, bool piped, t_minishell *g_minishell);

// * exist_check.c ***********************************************************
t_err	ft_check_exec(char *file, bool cmd);
t_err	ft_check_read(char *file);
t_err	ft_check_write(char *file);

// * ft_exec_simple_cmd.c ****************************************************
int		ft_check_redirection(t_node *node);
void	ft_reset_stds(bool piped, t_minishell *g_minishell);
int		ft_exec_simple_cmd(t_node *node, bool piped, t_minishell *g_minishell);

// * ft_get_path.c ***********************************************************
t_path	ft_get_path(char *cmd, t_minishell *g_minishell);

// * init_tree.c *************************************************************
void	ft_init_tree(t_node *node, t_minishell *g_minishell);
void	ft_heredoc(t_io_node *io, int p[2], t_minishell *g_minishell);

/* ****************************   EXPANDER   ****************************** */
// * ft_asterisker.c *********************************************************
bool	ft_match_star(char *pattern, char *str);

// * ft_clean_empty_strs.c ***************************************************
char	*ft_clean_empty_strs(char *str);

// * ft_expand_utils.c *******************************************************
bool	ft_is_valid_var_char(char c);
char	*ft_handle_normal_str(char *str, size_t *i);
char	*ft_handle_squotes(char *str, size_t *i);
char	*ft_handle_dquotes(char *str, size_t *i, t_minishell *g_minishell);

// * ft_expand.c *************************************************************
char	*ft_handle_dollar(char *str, size_t *i, t_minishell *g_minishell);
char	**ft_expand(char *str, t_minishell *g_minishell);

// * ft_expander_split.c *****************************************************
char	**ft_expander_split(char const *s);

// * ft_globber_utils.c ******************************************************
bool	ft_contains_asterisk(char *str);
size_t	ft_str_arr_len(char **str_arr);
size_t	ft_match_count(char *pattern);
char	**ft_join_str_arrs(char ***str_arrs);

// * ft_globber.c ************************************************************
char	**ft_globber(char **expanded);

// * ft_strip_quotes.c *******************************************************
char	*ft_strip_quotes(char *str);

// * heredoc_expander.c ******************************************************
void	ft_heredoc_expander(char *str, int fd, t_minishell *g_minishell);

/* ****************************   MINISHELL   ***************************** */
// * main_signals.c **********************************************************
void	ft_sigquit_handler(int num);
void	ft_init_signals();


t_token	*ft_tokenize(t_minishell *g_minishell);
t_node      *ft_parse(t_minishell *g_minishell);
void        ft_handle_parse_err(t_minishell *g_minishell);

// parser_utils.c
int         ft_curr_token_prec(t_minishell *g_minishell);
bool        ft_curr_token_is_binop(t_minishell *g_minishell);

void        ft_clear_ast(t_node **ast, t_minishell *g_minishell);

// parser_err.c
void        ft_set_parse_err(t_parse_err_type type, t_minishell *g_minishell);

// parser_helpers.c
bool        ft_get_io_list(t_io_node **io_list, t_minishell *g_minishell);
bool        ft_join_args(char **args, t_minishell *g_minishell);
t_node      *ft_get_simple_cmd(t_minishell *g_minishell);

void        ft_get_next_token(t_minishell *g_minishell);
t_node      *ft_term(t_minishell *g_minishell);
t_node      *ft_expression(int min_prec, t_minishell *g_minishell);

// parser.c;
t_node      *ft_combine(t_token_type op, t_node *left, t_node *right, t_minishell *g_minishell);

void	ft_print_quote_err(char c, t_minishell *g_minishell);

t_token	*ft_tokenization_handler(char *line, t_minishell *g_minishell);
int		ft_append_identifier(char **line_ptr, t_token **token_list, t_minishell *g_minishell);

#endif
