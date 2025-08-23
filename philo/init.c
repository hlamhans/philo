/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlamhans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 03:45:33 by hlamhans          #+#    #+#             */
/*   Updated: 2025/08/22 04:09:20 by hlamhans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int ac, char **av)
{
	data->nb_of_philo = ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	if (ac == 6)
		data->nb_of_meals = ft_atol(av[5]);
	else
		data->nb_of_meals = -1;
	if (data->nb_of_philo == INT_MAX || data->time_to_eat == INT_MAX
		|| data->time_to_die == INT_MAX
		|| data->time_to_sleep == INT_MAX || data->nb_of_meals == INT_MAX
		|| data->nb_of_philo <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0 || (ac == 6
			&&data->nb_of_meals <= 0) || data->nb_of_philo > 200)
	{
		print_usage(2, av[0]);
		free(data);
		return (ERROR);
	}
	data->time_to_start = ft_gettime();
	data->stop = 0;
	data->number_philos_ate = 0;
	data->forks = NULL;
	return (SUCCESS);
}

int	init_philosophers(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < data->nb_of_philo)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].last_meal_time = ft_gettime();
		philo[i].meals_eaten = 0;
		philo[i].left_fork = &data->forks[i];
		philo[i].right_fork = &data->forks[(i + 1) % data->nb_of_philo];
		i++;
	}
	return (SUCCESS);
}

int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_of_philo);
	data->philosophers = malloc(sizeof(pthread_t) * data->nb_of_philo);
	while (i < data->nb_of_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->stop_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->meals_mutex, NULL);
	pthread_mutex_init(&data->last_meal_mutex, NULL);
	return (SUCCESS);
}

int	start_simulation(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	data->time_to_start = ft_gettime();
	while (i < philo->data->nb_of_philo)
	{
		pthread_create(&data->philosophers[i], NULL, philosopher_routine,
			&philo[i]);
		i++;
	}
	pthread_create(&data->monitor, NULL, monitor_routine, philo);
	i = 0;
	while (i < data->nb_of_philo)
	{
		pthread_join(data->philosophers[i], NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
	return (SUCCESS);
}
