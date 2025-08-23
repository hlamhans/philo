/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlamhans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 03:45:46 by hlamhans          #+#    #+#             */
/*   Updated: 2025/08/22 04:14:16 by hlamhans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_philo_death(t_philo *philo, int i)
{
	long	current_time;
	long	last_meal_time;

	pthread_mutex_lock(&philo->data->last_meal_mutex);
	current_time = ft_gettime();
	last_meal_time = philo[i].last_meal_time;
	pthread_mutex_unlock(&philo->data->last_meal_mutex);
	if (current_time - last_meal_time > philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->stop_mutex);
		if (!philo->data->stop)
		{
			philo->data->stop = 1;
			printf("%ld %d died\n", current_time - philo->data->time_to_start,
				philo[i].id);
		}
		pthread_mutex_unlock(&philo->data->stop_mutex);
	}
}

void	check_meals_complete(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->meals_mutex);
	if (philo->data->nb_of_meals != -1
		&& philo->data->number_philos_ate >= philo->data->nb_of_philo)
	{
		pthread_mutex_lock(&philo->data->stop_mutex);
		philo->data->stop = 1;
		pthread_mutex_unlock(&philo->data->stop_mutex);
	}
	pthread_mutex_unlock(&philo->data->meals_mutex);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	if (!philo || !philo->data)
		return (NULL);
	while (!should_stop(philo))
	{
		i = 0;
		while (i < philo->data->nb_of_philo && !should_stop(philo))
		{
			check_philo_death(philo, i);
			if (should_stop(philo))
				break ;
			check_meals_complete(philo);
			usleep(500);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
