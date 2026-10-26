# Philosophers

*This project has been created as part of the 42 curriculum by tomyc.*

**A dining philosophers simulation in C using POSIX threads and mutexes.**

Philosophers models concurrent workers sharing a limited set of resources. Each philosopher needs two neighbouring forks to eat, then sleeps and thinks before starting another cycle. The implementation explores thread lifecycles, resource locking, shared state, and timing through a command-line simulation.

The program lives in [`philo/`](philo/). Its [technical README](philo/README.md) explains the parameters, synchronization strategy, death detection, examples, and implementation limits.

## Implementation highlights

- One POSIX thread per philosopher, with one mutex per shared fork.
- A monitor running in the main thread to check elapsed time since each philosopher started their last meal.
- Separate mutexes for simulation state and status output.
- A consistent order for acquiring the two fork mutexes.
- An optional meal quota and a separate execution path for a single philosopher.
- Thread joining and resource cleanup when the simulation returns.

These mechanisms are documented from the source. No guarantee of freedom from deadlocks, data races, or starvation is claimed.

## Build and run

Use a Linux or Unix environment with a C compiler available as `cc`, Make, and POSIX threads support.

```sh
git clone https://github.com/tommysoeder/Philosophers.git
cd Philosophers/philo
make
./philo 5 800 200 200 5
```

This example requests five philosophers, an 800 ms death timeout, 200 ms eating and sleeping durations, and a quota of five meals per philosopher. Timing and scheduling affect the outcome; see [monitoring and termination](philo/README.md#death-detection-and-termination).

The existing Makefile uses `-Wall -Wextra -Werror` and does not explicitly pass `-pthread`. If your toolchain requires that option, use the [documented command-line override](philo/README.md#build) without editing the Makefile.

## Repository guide

| Location | Contents |
| --- | --- |
| [`philo/README.md`](philo/README.md) | Build instructions, CLI reference, and implementation walkthrough |
| [`philo/`](philo/) | C sources, shared header, and Makefile for the simulation |

## Attribution and AI usage

The project attribution is **tomyc**, as recorded in the original README.

- AI usage: used as a guided learning assistant to understand the subject,
  design the implementation steps, and review concepts before writing code.
