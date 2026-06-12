*This project has been created as part of the 42 curriculum by tomyc.*

# Philosophers

## Description

This project implements the classic dining philosophers problem using C threads and
mutexes. The goal is to coordinate philosophers sharing forks without data races,
deadlocks, or invalid memory usage.

## Instructions

Build the mandatory program:

```sh
cd philo
make
```

Run:

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Example:

```sh
./philo 5 800 200 200
```

## Resources

- `pthread_create`, `pthread_join`, and pthread mutex documentation.
- `gettimeofday` manual page.
- 42 subject: Philosophers.
- AI usage: used as a guided learning assistant to understand the subject,
  design the implementation steps, and review concepts before writing code.
