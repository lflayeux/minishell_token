/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:19:08 by lflayeux          #+#    #+#             */
/*   Updated: 2025/05/19 23:56:45 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// ==============================================
// ================= EXPANSION ==================
// ==============================================

// RETOURNE LE PID POUR $$
char	*get_pid(void)
{
	int		fd;
	char	*line;
	char	**split;
	char	*pid;

	fd = open("/proc/self/status", O_RDONLY);
	line = get_next_line(fd);
	pid = NULL;
	while (line)
	{
		if (ft_strncmp(line, "Pid:", 4) == 0)
		{
			split = ft_split(line, '\t');
			pid = ft_strdup(ft_substr(split[1], 0, ft_strlen(split[1]) - 1));
			ft_free_tab((void **)split);
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (pid);
}

int	ft_isdelim(char c, char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (delim[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*find_var(char *s, int index)
{
	int		i;
	char	*var;
	int		len;

	len = 0;
	while (s[index + len] && ft_isdelim(s[index + len], "|<> \'\"") != 1)
		len++;
	var = ft_calloc((len + 1), sizeof(char));
	if (!var)
		return (NULL);
	i = 0;
	while (s[index] && ft_isdelim(s[index], "|<> \'\"") != 1)
		var[i++] = s[index++];
	return (var);
}

// RETURN LE PATH POUR $ENV_VARIABLE OU NULL SI NON TROUVE
char	*check_dollar_env(char **token, int *i, char **env)
{
	int		j;
	char	**split;
	char	*path;

	j = 0;
	path = NULL;
	while (env[j])
	{
		split = ft_split(env[j], '=');
		if (ft_strcmp(split[0], find_var(*token, *i)) == 0)
		{
			path = ft_strdup(split[1]);
			// *i += ft_strlen(find_var(*token, *i)) + 1;
			*i += ft_strlen(find_var(*token, *i));
			ft_free_tab((void **)split);
			return (path);
		}
		j++;
		ft_free_tab((void **)split);
	}
	return (*i += ft_strlen(find_var(*token, *i)), ft_strdup(""));
}

// PERMET DE RECALCULER LA BONNE LONGUEURE EN CAS DE $
int	dollar_len(char **token, int *i, char **env)
{
	int		len;
	char	*env_path;

	len = 0;
	(void)env;
	(*i)++;
	if ((*token)[*i] == '$')
	{
		// printf("pid : %s\n", get_pid());
		(*i)++;
		return (ft_strlen(get_pid()) - 1);
	}
	//	else if (token[*i] == '$' &&  token[i + 1] == '?')
	else
	{
		env_path = check_dollar_env(token, i, env);
		if (env_path)
			len += ft_strlen(env_path);
		// else
		// 	len++;
	}
	return (len);
}

// PERMET DE RECALCULER LA BONNE LONGUEURE EN CAS DE '' OU ""
int	quotes_len(char **token, int *i, char **env)
{
	int	len;

	len = 0;
	if ((*token)[*i] == '\'')
	{
		(*i)++;
		while ((*token)[*i] != '\'' && (*token)[*i] != '\0')
		{
			(*i)++;
			len++;
		}
		(*i)++;
	}
	else if ((*token)[*i] == '"')
	{
		(*i)++;
		while ((*token)[*i] != '"' && (*token)[*i] != '\0')
		{
			if ((*token)[*i] == '$')
				len += dollar_len(token, i, env);
			else
			{
				// printf("%d i\n", *i);
				(*i)++;
				len++;
			}
		}
		(*i)++;
	}
	return (len);
}

// FONCTION GLOBALE POUR LE RECALCUL DES ' " et $
int	expansion_len(char **token, char **env)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while ((*token)[i])
	{
		while ((*token)[i] != '\0' && (*token)[i] != '\'' && (*token)[i] != '"'
			&& (*token)[i] != '$')
		{
			i++;
			len++;
		}
		// printf("%d iexp\n", i);
		if ((*token)[i] != '\'' || (*token)[i] != '"')
			len += quotes_len(token, &i, env);
		if ((*token)[i] == '$')
			len += dollar_len(token, &i, env);
	}
	return (len);
}

// ==============================================
// ================ END EXPANSION ===============
// ==============================================

// ==============================================
// =================== TESTS ====================
// ==============================================

const char	*get_token_name(int type)
{
	if (type == 0)
		return ("|");
	if (type == 1)
		return ("<");
	if (type == 2)
		return (">");
	if (type == 3)
		return ("<<");
	if (type == 4)
		return (">>");
	if (type == 5)
		return ("WORD");
	return ("error");
}

// ==============================================
// ================= FIN TESTS ==================
// ==============================================

void	execute_input(char *input, char **env)
{
	t_tok	*token;
	t_exec	*exec;
	int		i;
	int 	j;
	t_tok	*tmp;
	t_tok	*tmp2;
	int		past_len;
	int		new_len;
	t_tok	*tmp3;
	t_exec	*tmp4;

	(void)env;
	token = NULL;
	while (*input != '\0')
	{
		tokenize(&input, &token);
	}
	//============ TEST TOKENISATION ===========
	tmp = token;
	tmp2 = token;
	i = 0;
	while (tmp != NULL)
	{
		printf("token num %d\n", i++);
		printf("\ttype: %s\n", get_token_name(tmp->type));
		printf("\tword: %s\n", tmp->word);
		tmp = tmp->next;
	}
	//========== FIN TEST TOKNISATION=========
	//============ TEST EXPANSION MALLOC===========
	while (tmp2 != NULL)
	{
		if (tmp2->word != NULL)
		{
			past_len = ft_strlen(tmp2->word);
			new_len = expansion_len(&tmp2->word, env);
			printf("\n========\n ancien len: %d\n nveau len: %d\n=======\n",
				past_len, new_len);
		}
		tmp2 = tmp2->next;
	}
	//============ FIN TEST EXPANSION MALLOC===========
	//============ TEST EXPANSION WORD PARSING===========
	word_identification(&token, env);
	tmp3 = token;
	i = 0;
	while (tmp3 != NULL)
	{
		printf("token num %d\n", i++);
		printf("\ttype: %s\n", get_token_name(tmp3->type));
		printf("\tword: %s\n", tmp3->word);
		tmp3 = tmp3->next;
	}
	//============ FIN TEST EXPANSION WORD PARSING===========
	//============ TEST LST EXEC PIPE PROCESS===========
	exec = NULL;
	create_lst_exec(&exec, &token);
	tmp4 = exec;
	i = 0;
	while (tmp4 != NULL)
	{
		j = 0;
		printf("exec num %d\n", i++);
		printf("\tcommand:\n");
		while((tmp4->cmd)[j])
		{
			printf("\t\tcmd num %d: %s\n", j, (tmp4->cmd)[j]);
			j++;
		}
		printf("\tinfile:%d,   %s\n", tmp4->if_infile, tmp4->infile);
		printf("\toutfile:%d,   %s\n", tmp4->if_outfile, tmp4->outfile);
		tmp4 = tmp4->pipe_to;
	}
	//============ FIN TEST LST EXEC PIPE PROCESS===========

}

int	main(int argc, char **argv, char **env)
{
	char		*input;
	t_signal	signals;

	(void)argc;
	(void)argv;
	(void)env;
	set_signal(&signals);
	while (1)
	{
		input = readline(PROMPT);
		if (input == NULL || ft_strcmp(input, "exit") == 0)
			return (printf("exit"), 0);
		if (*input)
		{
			add_history(input);
			//			test_signals(signals, env);
			execute_input(input, env);
		}
		reset_signals(&signals);
	}
}
// ==============================================
// =================== TESTS ====================
// ==============================================

void	test_signals(t_signal signals, char **env)
{
	pid_t	pid;
	char	*caca[] = {"../fdfdemerde/fdf", "../test_fdf/test_maps/julia.fdf",
			NULL};
	int		status;

	//	char *caca[] = {"/bin/bash", NULL};
	pid = fork();
	if (pid == 0)
	{
		child_signals(&signals);
		execve(caca[0], caca, env);
	}
	else
	{
		parent_signals(&signals);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		{
			if (WCOREDUMP(status))
				printf("Core dump fdp\n");
		}
		status = 0;
	}
}
