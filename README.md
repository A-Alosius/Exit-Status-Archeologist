# Exit Status Archeologist

A small C utility that runs a command, waits for it to finish, and reports how it terminated. It is intended to help inspect exit statuses, signal-based termination, and common exec failures such as command-not-found or permission issues.

## Build

From the project root, compile the program with:

```bash
gcc -Wall -Wextra -std=c11 -o runstat runstat.c
```

## Usage

Run the program with the command you want to inspect, followed by any arguments:

```bash
./runstat ls -l
./runstat /bin/true
./runstat /bin/false
./runstat does-not-exist
```

The program prints a summary such as:

- normal exit: status=0
- signal termination: signal=9, core=0
- command not found: status=2
- Permission denied: status=13

## Testing

This repository does not yet include an automated test suite. For now, use quick smoke tests to confirm the program behaves as expected:

```bash
./runstat ls
./runstat /bin/echo hello
./runstat /nonexistent/path
./runstat does-not-exist
```

As the project grows, add formal tests alongside the source and update this section with the exact commands to run them.
