# ARMON - System Monitor v0.3

A terminal-based system monitor written in C. Displays real-time system
metrics by reading directly from the Linux kernel interfaces.

![C](https://img.shields.io/badge/language-C-blue)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)

## Features

- **CPU usage** - read from `/proc/stat` (delta between two samples)
- **RAM usage** - read from `/proc/meminfo` (`MemTotal` + `MemAvailable`)
- **Swap usage** - read from `/proc/meminfo` (`SwapTotal` + `SwapFree`)
- **Disk usage** - read via `statvfs()` on the root filesystem
- **System uptime** - read from `/proc/uptime`, formatted as `Xh YYm ZZs`
- **Load average** - read from `/proc/loadavg`, with context based on core count
- **Host and kernel** - shown in the header
- **System health** - a global indicator (GOOD / WARN / CRITICAL) based on
  the worst metric
- **Top 5 processes by RAM** - name, PID and memory usage
- **Color-coded output** - green / yellow / red based on thresholds
- **Splash screen** - a brief startup screen when the program launches
- **Live refresh** - updates every second (configurable)
- **Clean terminal UI** - uses the alternate screen buffer so it doesn't
  pollute scrollback, restores the terminal cleanly on Ctrl+C, and uses
  an output buffer to avoid flicker between frames
- **Command-line flags** - `--help`, `--version`, `--oneshot`, `--interval`

## Requirements

- Linux (native, WSL, or any Linux environment)
- GCC (or any C11-compatible compiler)
- `make`

## Build

```bash
git clone https://github.com/Arielh7/armon-system-monitor.git
cd armon-system-monitor
make
```

## Install

To install ARMON so you can run `armon` from any directory:

```bash
make install
```

By default this installs to `~/.local/bin`. If that directory is not in
your `PATH`, add it to your shell configuration:

```bash
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

Now you can run `armon` from anywhere:

```bash
armon -o           # one-line summary
armon              # live dashboard
```

To install for all users (requires `sudo`):

```bash
sudo make install PREFIX=/usr/local
```

To uninstall:

```bash
make uninstall
```

To reinstall (useful after recompiling):

```bash
make reinstall
```

## Usage

```bash
armon [options]
```

### Options

| Flag                    | Description                                  |
|-------------------------|----------------------------------------------|
| `-h`, `--help`          | Show help message and exit                   |
| `-v`, `--version`       | Show version and exit                        |
| `-o`, `--oneshot`       | Print a one-line summary and exit            |
| `-i N`, `--interval N`  | Refresh every N seconds (default: 1)         |

### Examples

```bash
# Live dashboard, refresh every second
armon

# Live dashboard, refresh every 5 seconds
armon -i 5

# Print a one-line summary and exit (useful for scripts)
armon -o
# Output: CPU: 12.3%  RAM: 43.1%  Disk: 18.1%  Uptime: 4h 12m

# Show help
armon --help
```

Press `Ctrl+C` to exit the live dashboard.

## Example Output

```
ARMON - System Monitor v0.3

Host: your-hostname  |  Kernel: 6.x.x-your-kernel
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

System Health:  ● GOOD

 ▸ SYSTEM METRICS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CPU Usage:      3.42%
RAM Usage:      0.47 GB / 3.73 GB (12.6%)
Swap Usage:     0.00 GB / 1.00 GB (0.0%)
Disk Usage:     45.23 GB / 250.00 GB (18.1%)
System Uptime:  0h 39m 58s

 ▸ LOAD AVERAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Load Average:   0.42  0.51  0.60   (1m / 5m / 15m)
Load Status:    OK (4 cores)

 ▸ TOP PROCESSES (by RAM)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
   PROCESS                 PID      MEMORY
1. node                   6540    220.4 MB
2. node                   6495    133.9 MB
3. node                   6419    119.6 MB
4. node                   6501    114.4 MB
5. node                   6453     57.1 MB
```

## Project Structure

```
armon/
├── Makefile
├── README.md
├── LICENSE
├── .gitignore
├── include/
│   ├── dashboard.h    # SystemInfo struct + data collection API
│   ├── render.h       # Dashboard rendering API
│   ├── colors.h       # ANSI color codes and helpers
│   ├── health.h       # Global health status
│   ├── processes.h    # Process listing (top by RAM)
│   ├── splash.h       # Splash screen
│   ├── cli.h          # Command-line argument parsing
│   └── output.h       # Buffered output API
└── src/
    ├── main.c         # Entry point, main loop, signal handling
    ├── dashboard.c    # Reads system metrics from /proc and statvfs
    ├── render.c       # Prints the dashboard to the terminal
    ├── colors.c       # Color selection helpers
    ├── health.c       # Computes the overall health status
    ├── processes.c    # Reads and sorts processes from /proc
    ├── splash.c       # Startup splash screen
    ├── cli.c          # Command-line argument parsing
    └── output.c       # Buffered output implementation
```

### Architecture

The project is split into clear responsibilities:

- **`dashboard.c`** - collects raw system data. Knows nothing about how
  it's displayed.
- **`render.c`** - formats and prints data. Knows nothing about where the
  data comes from.
- **`main.c`** - coordinates: sets up the terminal, runs the main loop,
  handles signals.
- **`colors.c`** - decides which color to use for a given value.
- **`health.c`** - computes the overall health of the system based on the
  worst metric.
- **`processes.c`** - reads `/proc/[pid]/cmdline` and `/proc/[pid]/status`
  to list processes, sorted by RAM usage.
- **`splash.c`** - displays the startup screen.
- **`cli.c`** - parses command-line arguments.
- **`output.c`** - buffers the whole frame and writes it in a single call
  to avoid flicker.

This separation makes it easy to swap the rendering layer (e.g. add bars,
graphs, or export to JSON) without touching the data collection code.

## How It Works

| Metric         | Source               | Method                                              |
|----------------|----------------------|-----------------------------------------------------|
| CPU            | `/proc/stat`         | Reads cumulative jiffies twice and computes the delta |
| RAM            | `/proc/meminfo`      | Parses `MemTotal` and `MemAvailable` lines          |
| Swap           | `/proc/meminfo`      | Parses `SwapTotal` and `SwapFree` lines             |
| Disk           | `statvfs()`          | Reads block counts from the root filesystem         |
| Uptime         | `/proc/uptime`       | Reads seconds since boot                            |
| Load average   | `/proc/loadavg`      | Reads 1m / 5m / 15m averages                        |
| Hostname       | `gethostname()`      | POSIX call                                          |
| Kernel version | `uname()`            | POSIX call                                          |
| Processes      | `/proc/[pid]/*`      | Reads `cmdline` (name) and `status` (`VmRSS`)       |

### Health Status

The global health is computed by taking the **worst** status among all
metrics:

| Metric | Warn threshold | Critical threshold |
|--------|----------------|--------------------|
| CPU    | 50%            | 80%                |
| RAM    | 50%            | 80%                |
| Disk   | 80%            | 90%                |
| Load   | > cores        | > 2 × cores        |

The disk has higher thresholds because a full disk is more critical than
high CPU or RAM usage.

## License

MIT