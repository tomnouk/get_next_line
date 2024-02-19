/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anoukmournard <anoukmournard@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 15:41:26 by anoukmourna       #+#    #+#             */
/*   Updated: 2023/12/20 13:55:45 by anoukmourna      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* Libère la mémoire allouée à un pointeur et le met à NULL */
void	free_ptr(char *ptr)
{
    if (ptr)
    {
        free(ptr);
        ptr = NULL;
    }
}

/* Lit une ligne à partir d'un descripteur de fichier */
char	*get_next_line(int fd)
{
    char		*line;
    static char	*remaining_lines_buffer = NULL ;
    char		*read_buffer;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    read_buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (!read_buffer)
        return (NULL);
    if (read(fd, read_buffer, 0) < 0)
    {
        free(read_buffer);
        return (NULL);
    }
    if (!remaining_lines_buffer)
        remaining_lines_buffer = ft_strdup("");
    if (read_and_process_file(fd, &read_buffer, &remaining_lines_buffer, &line) == 0 && *line == '\0')
    {
        free_ptr(line);
        return (NULL);
    }
    return (line);
}

/* Lit le fichier et traite les données lues */
int	read_and_process_file(int fd, char **read_buffer, char **remaining_lines_buffer, char **line)
{
    int		read_bytes;
    char	*temp;

    read_bytes = 1;
    while (!ft_strchr(*remaining_lines_buffer, '\n') && read_bytes)
    {
        read_bytes = read(fd, *read_buffer, BUFFER_SIZE);
        (*read_buffer)[read_bytes] = '\0';
        temp = *remaining_lines_buffer;
        *remaining_lines_buffer = ft_strjoin(*remaining_lines_buffer, *read_buffer);
        free_ptr(temp);
    }
    free_ptr(*read_buffer);
    extract_line(line, remaining_lines_buffer);
    return (read_bytes);
}

/* Extrait une ligne du buffer de lignes restantes */
char	*extract_line(char **line, char **remaining_lines_buffer)
{
    char	*temp;
    int		newline_position;

    newline_position = 0;
    temp = *remaining_lines_buffer;
    while ((*remaining_lines_buffer)[newline_position] != '\n' &&
        (*remaining_lines_buffer)[newline_position] != '\0')
        newline_position++;
    if (ft_strchr(*remaining_lines_buffer, '\n'))
    {
        *line = ft_substr(*remaining_lines_buffer, 0, newline_position + 1);
        *remaining_lines_buffer = ft_strdup(*remaining_lines_buffer + newline_position + 1);
    }
    else
    {
        *line = ft_strdup(temp);
        *remaining_lines_buffer = NULL;
    }
    free_ptr(temp);
    return (*line);
}
