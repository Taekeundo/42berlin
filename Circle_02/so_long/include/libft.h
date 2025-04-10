/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkwak <tkwak@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:22:06 by tkwak             #+#    #+#             */
/*   Updated: 2023/11/01 10:35:01 by tkwak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>

/* Structure */
typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}	t_list;

/* part 1 @is */
int		ft_isalnum(int character);
int		ft_isalpha(int character);
int		ft_isascii(int character);
int		ft_isdigit(int character);
int		ft_isprint(int character);

/* part 1 @to */
int		ft_tolower(int character);
int		ft_toupper(int character);

/* part 1 @str */
size_t	ft_strlen(const char *str);
size_t	ft_strlcpy(char *dst, const char *src, size_t len);
size_t	ft_strlcat(char *dst, const char *src, size_t len);
char	*ft_strchr(const char *str, int character);
char	*ft_strrchr(const char *s, int c);
char	*ft_strdup(const char *str);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int		ft_strncmp(const char *s1, const char *s2, size_t len);

/* part 1 @mem */
void	*ft_memset(void *ptr, int value, size_t num);
void	*ft_memcpy(void *dst, const void *src, size_t num);
void	*ft_memccpy(void *dst, const void *src, int c, size_t num);
void	*ft_memmove(void *dst, const void *src, size_t num);
void	*ft_memchr(const void *ptr, int c, size_t num);
int		ft_memcmp(const void *s1, const void *s2, size_t num);

/* part 1 @etc */
void	ft_bzero(void *ptr, size_t num);
void	*ft_calloc(size_t num, size_t size);
int		ft_atoi(const char *str);

/* part 2 @all */
char	*ft_substr(char const *str, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

/* Bonus (list) */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void*));
int		ft_lstsize(t_list *lst);

#endif
