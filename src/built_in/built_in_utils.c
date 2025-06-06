/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pandemonium <pandemonium@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:20:21 by pandemonium       #+#    #+#             */
/*   Updated: 2025/06/02 20:25:02 by pandemonium      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int is_valid_env(char *exec)
{
    int i = 0;

    if (!exec || (!ft_isalpha(exec[0]) && exec[0] != '_'))
        return (0);
    while (exec[i] && exec[i] != '=')
    {
        if (!ft_isalnum(exec[i]) && exec[i] != '_')
            return (0);
        i++;
    }
    return (1);
}
int ft_get_env(char **env, char *to_check)
{
    int i;

    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], to_check, ft_strlen(to_check)) == 0)
            return (1);
        i++;
    }
    return (0);
}
// ========================
// ======= UNSET ENV ======
// ========================
char    **unset_env(char *unset_env, char **env)
{
    char **new_env;
    char **split;
    int len;
    int i;
    int j;

    i = 0;
    j = 0;
    len = 0;
    while (env[len])
        len++;
    new_env = ft_calloc(len, sizeof(char *));
    if (new_env == NULL)
        return (NULL);
    while (env[i])
    {
        split = ft_split(env[i], '=');
        if (ft_strncmp(split[0], unset_env, ft_strlen(split[0])))
            new_env[j++] = ft_strdup(env[i]);
        ft_free_tab((void **)split);
        i++;
    }
    ft_free_tab((void **)env);
    return (new_env);
}


char **put_env(t_shell **shell, int *i, char **env)
{
    int len;
    int j;
    char **new_env;

    len = 0;
    j = 0;
    while (env[len])
        len++;
    new_env = malloc((len + 2) * sizeof(char *));
    if (!new_env)
        return (NULL);
    while (env[j])
    {
        new_env[j] = ft_strdup(env[j]);
        j++;
    }
    new_env[j] = ft_strdup((*shell)->exec->cmd[*i + 1]);
    new_env[j + 1] = NULL;
    ft_free_tab((void **)(env));
    return (new_env);
}
// ========================
// ======= RESET ENV ======
// ========================
char **set_env(t_shell **shell, int *i, char *split, char **env)
{
    int j;
    char **new_env;

    j = 0;
    new_env = init_env(env);
    if (!split[0] || !new_env)
        return (NULL);
    while (env[j])
    {
        if (ft_strncmp(env[j], split, ft_strlen(split)) == 0)
        {
            free(new_env[j]);
            new_env[j] = ft_strdup((*shell)->exec->cmd[*i + 1]);
        }
        else
            new_env[j] = ft_strdup(env[j]);
        j++;
    }
    ft_free_tab((void **)(env));
    return (new_env);
}