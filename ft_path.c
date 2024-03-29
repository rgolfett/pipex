/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgolfett <rgolfett@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:18:50 by rgolfett          #+#    #+#             */
/*   Updated: 2024/03/06 17:23:57 by rgolfett         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "ft_printf.h"

int	ft_access(char *str)
{
	if (str == NULL)
		return (-1);
	if (access(str, F_OK) == 0)
	{
		if (access(str, X_OK) == 0)
			return (0);
		return (ft_printf("%s : permission denied\n", str), -1);
	}
	return (1);
}

char	*ft_find_path(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			if (!envp[i][4])
				return (envp[i] + 4);
			return (envp[i] + 5);
		}
		i++;
	}
	return (NULL);
}

char	*ft_create_path_command(char *path, char *cmd)
{
	char	*str;
	char	*cpy;

	str = ft_strjoin(path, "/");
	cpy = str;
	str = ft_strjoin(str, cmd);
	free (cpy);
	return (str);
}

char	*ft_path(char *path, char *cmd)
{
	size_t		i;
	size_t		j;
	char		*str;

	i = 0;
	j = 0;
	if (cmd[0] != '\0' && ft_access(cmd) == 0)
		return (ft_strdup(cmd));
	if (cmd[0] == '\0' || (access(cmd, F_OK) == 0 && access(cmd, X_OK) != 0))
		return (NULL);
	while (path[i])
	{
		while (path[i] && path[i] != ':')
			i++;
		path[i] = '\0';
		str = ft_create_path_command(&path[j], cmd);
		if (ft_access(str) == -1)
			return (free(str), NULL);
		if (ft_access(str) == 0)
			return (str);
		free(str);
		j = ++i;
	}
	return (ft_printf("command not found : %s\n", cmd), NULL);
}

char	**ft_fill_av(char *path_cmd, char *cmd_arg)
{
	char	**argument;
	size_t	size;
	size_t	index;

	size = ft_count_args(cmd_arg);
	index = 1;
	argument = malloc (sizeof (char *) * (size + 2));
	if (!argument)
		return (NULL);
	argument[0] = path_cmd;
	while (index < (size + 1))
	{
		argument[index] = get_arg(cmd_arg, &cmd_arg);
		if (!argument[index])
			return (ft_free_arg(argument), NULL);
		index++;
	}
	argument[index] = NULL;
	return (argument);
}
