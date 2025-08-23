/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlamhans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 03:45:14 by hlamhans          #+#    #+#             */
/*   Updated: 2025/08/22 04:58:30 by hlamhans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_usage(int fd, char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	write(fd, "Usage: ", 7);
	write(fd, s, len);
	write(fd, " n_philo[1->200] time_to_die time_to_eat ", 41);
	write(fd, "time_to_sleep [n_must_eat]\n", 28);
}

void	clean(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->last_meal_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	while (i < data->nb_of_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->philosophers);
	free(data->forks);
	free(data);
	free(philo);
}

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philo;

	if (ac != 5 && ac != 6)
	{
		print_usage(2, av[0]);
		return (1);
	}
	data = malloc(sizeof(t_data));
	if (!data)
	{
		write (2, "Error: Memory allocation failed for data\n", 42);
		return (1);
	}
	if (init_data(data, ac, av))
		return (1);
	philo = malloc(sizeof(t_philo) * data->nb_of_philo);
	if (!philo)
		return (1);
	init_mutexes(data);
	init_philosophers(data, philo);
	start_simulation(data, philo);
	clean(data, philo);
	return (0);
}
