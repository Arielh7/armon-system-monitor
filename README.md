# ARMON — System Monitor v0.1

A terminal-based system monitor written in C. Displays real-time CPU, RAM, disk usage and system uptime by reading directly from the Linux kernel interfaces.

![C](https://img.shields.io/badge/language-C-blue)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)

## Features

- **CPU usage** — read from `/proc/stat` (delta between two samples)
- **RAM usage** — read from `/proc/meminfo` (`MemTotal` + `MemAvailable`)
- **Disk usage** — read via `statvfs()` on the root filesystem
- **System uptime** — read from `/proc/uptime`, formatted as `Xh YYm ZZs`
- **Live refresh** — updates every second
- **Clean terminal UI** — uses the alternate screen buffer so it doesn't pollute scrollback

## Requirements

- Linux (native, WSL, or any Linux environment)
- GCC (or any C11-compatible compiler)
- `make`

## Build

```bash
git clone <repo-url>
cd armon
make
```

## Run

```bash
make run
```

Or directly:

```bash
./build/armon
```

Press `Ctrl+C` to exit cleanly.

## Example Output

```
ARMON — System Monitor v0.1
===========================

CPU Usage:      3.42%
RAM Usage:      0.47 GB / 3.73 GB (12.6%)
Disk Usage:     45.23 GB / 250.00 GB (18.1%)
System Uptime:  0h 39m 58s
```

## Project Structure

```
armon/
├── Makefile
├── README.md
├── .gitignore
├── include/
│   ├── dashboard.h     # SystemInfo struct + public API
│   └── render.h        # Rendering API
└── src/
    ├── main.c          # Entry point + main loop
    ├── dashboard.c     # Reads system metrics from /proc and statvfs
    └── render.c        # Prints the dashboard to the terminal
```

### Architecture

The project is split into three responsibilities:

- **`dashboard.c`** — collects raw system data. Knows nothing about how it's displayed.
- **`render.c`** — formats and prints data. Knows nothing about where the data comes from.
- **`main.c`** — coordinates: sets up the terminal, runs the main loop, handles signals.

This separation makes it easy to swap the rendering layer (e.g. add colors, bars, or export to JSON) without touching the data collection code.

## How It Works

| Metric | Source | Method |
|--------|--------|--------|
| CPU    | `/proc/stat` | Reads cumulative jiffies twice and computes the delta |
| RAM    | `/proc/meminfo` | Parses `MemTotal` and `MemAvailable` lines |
| Disk   | `statvfs()` | Reads block counts from the root filesystem |
| Uptime | `/proc/uptime` | Reads seconds since boot |

## License

MIT