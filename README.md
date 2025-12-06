# Ping-Pong with POSIX Signals

Implementation of the classic "ping-pong" synchronization scheme between two processes using POSIX signals.

## Build & Run

```bash
# Build
make

# Run
make run

# Or directly
./ping-pong

# Cleanup
make clean
```

## Description

How it works:
- Two processes are in one of two states: **Ready** or **Sleep**
- When process A is in Ready state, it sends a signal to process B (which is in Sleep)
- After sending the signal, process A transitions to Sleep, and process B transitions to Ready
- Process B performs the same action in the opposite direction
- The cycle repeats a specified number of times

## Solution

The program uses:
- `fork()` to create two processes (parent and child)
- `SIGUSR1` as the synchronization signal
- `sigaction()` to set up the signal handler
- `pause()` to wait for a signal (Sleep state)
- `kill()` to send a signal to another process

### Algorithm

1. Parent process creates a child via `fork()`
2. Both processes set up a handler for `SIGUSR1`
3. Parent starts first: prints "PING" and sends a signal to the child
4. Child receives the signal, prints "PONG" and sends a signal to the parent
5. Steps 3-4 repeat 10 times
