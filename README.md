<div align="center">
    <h1>ggpkg</h1>
    <h5>The cross-platform package manager wrapper you never asked for (but needed 🚀)</h5>
</div>

<p align="center">
    <img src="https://img.shields.io/badge/Language-C%2B%2B-blue">
    <img src="https://img.shields.io/github/license/landiluigi746/ggpkg">
</p>

## 📖 Overview

**ggpkg** is a unified command-line interface for system package managers across different platforms written in C++23. It automatically detects your system's package manager and provides a consistent set of commands, making package management seamless whether you're on Windows, Ubuntu, Arch Linux, or other supported distributions.

### ✨ Key Features

- 🔍 **Automatic Detection** - Finds and configures your system's package manager automatically
- 🌐 **Cross-Platform** - Works on Windows (winget), Arch Linux (pacman), and Debian/Ubuntu (apt) (more coming soon...)
- 📦 **Package Mapping** - Translates package names between different package managers
- 🎨 **Interactive UI** - Beautiful terminal interface for browsing available packages
- ⚡ **Batch Operations** - Install or uninstall multiple packages with a single command

## 🚀 Quick Start

### Prerequisites

- Git
- CMake 3.16+
- C++23 compatible compiler
- vcpkg to install dependencies

### Building from Source

```bash
# Clone the repository
git clone https://github.com/landiluigi746/ggpkg.git
cd ggpkg

# Configure and build
cmake --preset release
cmake --build --preset release --parallel $(nproc)

# Install
# make sure you have the required rights
sudo cmake --install build-release
```

### First Time Setup

```bash
# Configure ggpkg to detect your package manager and download package database
ggpkg configure

# Test the configuration
ggpkg test
```

## 📋 Usage

### Basic Commands

```bash
# Configure package manager automatically
ggpkg configure
# or
ggpkg autoconfig

# Fetch latest ggpkg package database
ggpkg update-db

# Test current configuration
ggpkg test

# List available packages
ggpkg list
# or with interactive mode
ggpkg list -i # or --interactive

# Install packages
ggpkg install firefox git
# or
ggpkg add firefox git

# Update package manager repositories
ggpkg update

# Uninstall packages
ggpkg uninstall firefox
# or
ggpkg remove firefox

# Get help
ggpkg --help
```

## 📋 Supported Package Managers

| Platform | Package Manager | Status |
|----------|----------------|---------|
| Windows | winget | ✅ Supported |
| Arch Linux | pacman | ✅ Supported |
| Debian/Ubuntu | apt | ✅ Supported |
| Fedora | dnf | 🚧 Planned |
| macOS | brew | 🚧 Planned |

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

<p align="center">Drop a ⭐ if you like this project!</p>
