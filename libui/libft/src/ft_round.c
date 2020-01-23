/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_round.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gmonacho <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/01/25 16:56:53 by gmonacho     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/25 16:59:49 by gmonacho    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

int		ft_round(double nb)
{
	int		rounded;

	rounded = nb;
	if (nb - (double)rounded > 0.5)
		return (rounded + 1);
	else
		return (rounded);
}
