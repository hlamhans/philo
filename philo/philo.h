/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlamhans <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 03:45:57 by hlamhans          #+#    #+#             */
/*   Updated: 2025/08/22 04:40:36 by hlamhans         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1

typedef struct s_data
{
	long			nb_of_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			nb_of_meals;
	long			time_to_start;
	int				stop;
	int				number_philos_ate;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	last_meal_mutex;
	pthread_mutex_t	meals_mutex;
	pthread_t		*philosophers;
	pthread_t		monitor;
	pthread_mutex_t	*forks;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}					t_philo;

long				ft_atol(const char *str);
size_t				ft_gettime(void);
void				print_state(t_philo *philo, char *state);
int					start_simulation(t_data *data, t_philo *philo);
int					eat(t_philo *philo);
void				ft_usleep(long time, t_philo *philo);
void				*philosopher_routine(void *arg);
void				*monitor_routine(void *arg);
int					should_stop(t_philo *philo);
int					init_data(t_data *data, int ac, char **av);
int					init_mutexes(t_data *data);
int					init_philosophers(t_data *data, t_philo *philo);
void				*philosopher_routine(void *arg);
void				*monitor_routine(void *arg);
void				clean(t_data *data, t_philo *philo);
void				print_usage(int fd, char *s);

#endif
