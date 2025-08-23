/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlamhans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 03:46:21 by hlamhans          #+#    #+#             */
/*   Updated: 2025/08/22 23:23:38 by hlamhans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *str)
{
	int		i;
	long	num;

	i = 0;
	num = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] == '-')
		return (write (2, "Error: Please use positive value\n", 34), INT_MAX);
	while (str[i] >= 48 && str[i] <= 57)
	{
		num = num * 10 + str[i++] - 48;
		if (num > INT_MAX)
			return (write(2, "Error: value is too big\n", 25), INT_MAX);
		if (num < 0)
			return (write(2, "Error: value is too big\n", 25), INT_MAX);
	}
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i])
		return (printf("Error: not valid integer\n"), INT_MAX);
	return (num);
}

size_t	ft_gettime(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
	{
		write(2, "Error: gettimeofday failed\n", 26);
		return (0);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_usleep(long time, t_philo *philo)
{
	long long	start;
	long long	elapsed;

	start = ft_gettime();
	while (1)
	{
		elapsed = ft_gettime() - start;
		if (elapsed >= time)
			break ;
		if (should_stop(philo))
			break ;
		if (time - elapsed > 5)
			usleep(500);
		else
			usleep(50);
	}
}

void	print_state(t_philo *philo, char *state)
{
	pthread_mutex_lock(&philo->data->stop_mutex);
	if (!philo->data->stop)
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%ld %d %s\n", ft_gettime() - philo->data->time_to_start,
			philo->id, state);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
	pthread_mutex_unlock(&philo->data->stop_mutex);
}

int	should_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->stop_mutex);
	stop = philo->data->stop;
	pthread_mutex_unlock(&philo->data->stop_mutex);
	return (stop);
}
