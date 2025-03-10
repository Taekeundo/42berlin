/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkwak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 21:07:59 by tkwak             #+#    #+#             */
/*   Updated: 2023/02/19 16:33:09 by tkwak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	[F]
	[ Definition ]
	Comparing two strings(Param(1):"s1" & Param(2):"s2") one char by one char.
	Return the value (*s1 - *s2).
*/
int	ft_strcmp(char *s1, char *s2)
{
	while ((*s1 == *s2) && *s1 && *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}
