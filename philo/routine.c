/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlamhans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 03:46:09 by hlamhans          #+#    #+#             */
/*   Updated: 2025/08/22 04:44:03 by hlamhans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_helper(t_philo *philo, pthread_mutex_t *first_fork,
		pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(first_fork);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_state(philo, "has taken a fork");
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->data->last_meal_mutex);
	philo->last_meal_time = ft_gettime();
	pthread_mutex_unlock(&philo->data->last_meal_mutex);
	ft_usleep(philo->data->time_to_eat, philo);
	pthread_mutex_lock(&philo->data->meals_mutex);
	philo->meals_eaten++;
	if (philo->data->nb_of_meals != -1
		&& philo->meals_eaten == philo->data->nb_of_meals)
		philo->data->number_philos_ate++;
	pthread_mutex_unlock(&philo->data->meals_mutex);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
	usleep(100);
}

int	eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(500);
	pthread_mutex_lock(&philo->data->stop_mutex);
	if (philo->data->stop)
	{
		pthread_mutex_unlock(&philo->data->stop_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->stop_mutex);
	if (philo->right_fork < philo->left_fork)
		eat_helper(philo, philo->right_fork, philo->left_fork);
	else
		eat_helper(philo, philo->left_fork, philo->right_fork);
	return (0);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (!philo || !philo->data)
		return (NULL);
	if (philo->data->nb_of_philo == 1)
	{
		print_state(philo, "has taken a fork");
		ft_usleep(philo->data->time_to_die, philo);
		return (NULL);
	}
	pthread_mutex_lock(&philo->data->last_meal_mutex);
	philo->last_meal_time = ft_gettime();
	pthread_mutex_unlock(&philo->data->last_meal_mutex);
	while (!should_stop(philo))
	{
		if (eat(philo))
			break ;
		print_state(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep, philo);
		print_state(philo, "is thinking");
	}
	return (NULL);
}
