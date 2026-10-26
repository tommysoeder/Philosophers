# Philosophers: implementation and usage

*This project has been created as part of the 42 curriculum by tomyc.*

This C implementation of the dining philosophers problem uses POSIX threads (`pthread`) and mutexes to model philosophers sharing forks around a table. Each worker cycles through eating, sleeping, and thinking while the main thread monitors the simulation.

For a short project overview, see the [repository README](../README.md).

## Build

Requirements: a Linux or Unix environment, a C compiler available as `cc`, Make, and POSIX threads headers and library support.

From the repository root:

```sh
cd philo
make
```

The executable is `philo` in this directory. The existing [Makefile](Makefile) uses `cc` with `-Wall -Wextra -Werror`; it does not explicitly pass `-pthread`. On toolchains that require that option, supply it for compilation and linking through the existing `CFLAGS` variable:

```sh
make re CFLAGS="-Wall -Wextra -Werror -pthread"
```

This command overrides build flags without changing the Makefile.

| Command | Effect |
| --- | --- |
| `make` | Build the executable |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and the executable |
| `make re` | Clean and rebuild |

## Command-line parameters

Run from `philo/`:

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Parameter | Meaning |
| --- | --- |
| `number_of_philosophers` | Number of philosopher threads and shared forks |
| `time_to_die` | Milliseconds allowed since the start of the last meal, or the common simulation start before the first meal |
| `time_to_eat` | Requested eating duration in milliseconds; both forks are held during this phase |
| `time_to_sleep` | Requested sleeping duration in milliseconds after eating |
| `number_of_times_each_philosopher_must_eat` | Optional per-philosopher meal quota checked by workers and the monitor |

[Argument parsing](parse.c) accepts exactly four required values and one optional value. Every value must contain decimal digits only and be between `1` and `INT_MAX`, inclusive. Zero, signs, spaces within an argument, non-digit characters, and overflow are rejected with a usage message and exit status `1`.

Without a meal quota, there is no fixed iteration limit: the simulation continues until the monitor detects a death or the process is interrupted.

## Examples

Start a simulation without a meal quota; use `Ctrl+C` to interrupt it if it keeps running:

```sh
./philo 5 800 200 200
```

Request five meals per philosopher:

```sh
./philo 5 800 200 200 5
```

Exercise the single-philosopher path. With only one fork, the philosopher cannot eat and waits for the monitor to detect the death timeout:

```sh
./philo 1 800 200 200
```

Exercise argument validation. A zero duration is invalid:

```sh
./philo 5 0 200 200
```

These are usage examples, not recorded test results. Scheduling and machine load affect event order and timing; the meal-quota example is a request, not a guarantee of survival until completion.

## Threads and synchronization

### Thread lifecycle

[`init.c`](init.c) allocates the philosopher and fork arrays and initializes the mutexes. Philosopher IDs start at `1`. Each philosopher references fork `i` and fork `(i + 1) % number_of_philosophers`, forming a ring.

[`simulation.c`](simulation.c) sets a common start timestamp and initializes every `last_meal` timestamp before creating one worker thread per philosopher. Threads begin as they are created; there is no start barrier. After creating the workers, the main thread calls `monitor_simulation()` and later joins the workers with `pthread_join`. [`cleanup.c`](cleanup.c) then destroys the mutexes and frees the arrays.

### Mutex responsibilities

| Mutex | Use in the implementation |
| --- | --- |
| One mutex per fork | Exclusive ownership of that fork while a philosopher holds it |
| `state_mutex` | Access to the shared stop flag, last-meal timestamps, and meal counters during the simulation |
| `print_mutex` | Serialization of status-line construction and output |

[`actions.c`](actions.c) acquires the lower-address fork mutex first, then the higher-address fork mutex. This gives workers a consistent fork acquisition order. Fork acquisition uses blocking `pthread_mutex_lock` calls.

Even-numbered philosophers request a 1 ms initial delay. For an odd number of philosophers, the thinking phase also requests a delay of `time_to_eat * 9 / 10` milliseconds, using integer arithmetic. These scheduling choices do not establish fairness or prove the absence of deadlocks or data races.

### Worker cycle

In [`routine.c`](routine.c), a worker acquires both forks, updates `last_meal` at the start of eating, prints its eating status, and waits for `time_to_eat`. It then increments its meal counter, releases both forks, sleeps, and thinks before checking whether to begin another cycle.

The single-philosopher path acquires the only fork once and waits for the stop flag, then releases it. It does not attempt to lock the same fork a second time.

## Death detection and termination

The monitor in [`monitor.c`](monitor.c) scans philosophers in array order. It reads each `last_meal` timestamp under `state_mutex` and tests:

```text
current_time_ms - last_meal >= time_to_die
```

On a detected death, it sets the shared stop flag, prints `died`, and returns. After each death scan, it checks whether every philosopher has reached the optional meal quota. If so, it sets the stop flag and returns without a separate completion message. Otherwise, it requests a 100 microsecond sleep before scanning again.

Each worker checks its own quota before beginning a new cycle. The monitor still checks all philosophers for death, including those that have reached their quota, and performs death checks before the global quota check. With some timings, a philosopher that has finished its quota can therefore be reported dead while others are still eating.

[`time.c`](time.c) uses `gettimeofday` for millisecond timestamps. Its `precise_sleep` helper checks elapsed time and the stop flag between `usleep(100)` calls. These are polling intervals, not timing guarantees: scheduling and wall-clock adjustments can affect the observed timing.

Shutdown is cooperative. Sleeps poll the stop flag, but a worker blocked acquiring a fork does not poll it until the lock call returns. The meal counter is incremented after the eating wait returns, including if that wait ended early because the simulation stopped.

## Output

Status lines have this format:

```text
timestamp_ms philosopher_id status
```

The timestamp is elapsed milliseconds since the common simulation start. Status messages are `has taken a fork`, `is eating`, `is sleeping`, `is thinking`, and `died`.

[`print.c`](print.c) holds `print_mutex` while formatting and writing each line. Regular status output is suppressed when its stop check observes that the simulation has stopped; the death message is allowed through.

## Scope and validation

This documentation describes the current source and its synchronization mechanisms. It makes no claim that the program is free of deadlocks, data races, or starvation, and provides no real-time scheduling guarantee. Usage examples are not a substitute for dedicated concurrency and timing tests.

## Source guide

| Files | Responsibility |
| --- | --- |
| [`main.c`](main.c), [`parse.c`](parse.c) | Program entry and argument validation |
| [`philo.h`](philo.h), [`init.c`](init.c), [`cleanup.c`](cleanup.c) | Shared structures, allocation, and resource lifecycle |
| [`simulation.c`](simulation.c), [`routine.c`](routine.c), [`actions.c`](actions.c) | Thread creation, worker behaviour, and fork operations |
| [`monitor.c`](monitor.c), [`state.c`](state.c) | Death and meal-quota checks, shared stop state |
| [`time.c`](time.c), [`print.c`](print.c) | Timing helpers and status output |

## Resources

- `pthread_create`, `pthread_join`, and pthread mutex documentation.
- `gettimeofday` manual page.
- 42 subject: Philosophers.

## Attribution and AI usage

The project attribution is **tomyc**, as recorded in the original README.

- AI usage: used as a guided learning assistant to understand the subject,
  design the implementation steps, and review concepts before writing code.
