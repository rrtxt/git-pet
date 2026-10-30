# git-pet

A terminal-based pet and Git status visualizer built with C++20. The application maps the state of your local Git repository to the growth stage and emotional mood of an interactive terminal pet.

## Description

git-pet is a command-line tool that brings your local Git repositories to life by representing their state through a digital pet. By querying commit history and worktree status, the application determines the pet's growth stage and mood in real-time.

* **Growth Stages**: The pet progresses through stages (Egg, Baby, Teen, and Adult) based on the repository's total commit count.
* **Emotional Moods**: The pet responds dynamically to repository activity and conditions:
  * **Happy**: High activity (at least 10 commits today or 20 commits this week).
  * **Sad**: Low activity (5 or fewer commits this week).
  * **Angry**: The repository has active merge conflicts.
  * **Neutral**: Normal repository status.

The user interface features a double-column layout where the pet's pixel art and status are displayed on the left, while repository statistics, commit history, and branch lists are displayed on the right.

## Features

* **Terminal UI**: Built using the FTXUI library for interactive and responsive layout structures inside the command line.
* **Real-time Stats**: Displays total commits, weekly commits, daily commits, and metadata for the HEAD commit.
* **Interactive Navigation**: Supports tabs to easily switch views between Main Info, Commit Info, and Branch Info.
* **Custom Assets**: Renders pixel art animations by loading images (PNG) frame-by-frame using the stb_image libraries.
* **Extensible Configuration**: Supports local config (`pet.config`) in the repository and global pet configurations defined by the `GIT_PET_PATH` environment variable.

## Installation

### Quick install (Linux x86_64)

```bash
curl -fsSL https://raw.githubusercontent.com/rrtxt/git-pet/main/install.sh | bash
```

This downloads the latest release and:
* Installs the binary to `~/.local/bin/git-pet`.
* Installs the default pet assets to `~/.local/share/git-pet/cat/` (an existing `pet.toml` is kept).
* Adds `GIT_PET_PATH` and `~/.local/bin` to your shell config (`~/.bashrc`, `~/.zshrc`, or `~/.profile`, plus fish if installed).

Restart your shell, then run `git-pet` inside any Git repository.

The prebuilt binary links against the system libgit2, so make sure it is installed (e.g. `sudo apt-get install libgit2-1.7` on Ubuntu 24.04).

### Install a local build

If you built from source (see [How to build](#how-to-build)), run this from the repository root to install `build/git-pet` instead of the release:

```bash
./install.sh --local
```

### Options

```bash
./install.sh --uninstall          # remove the binary, assets and shell config
PREFIX=/usr/local ./install.sh    # install somewhere other than ~/.local (may need sudo)
```

### Windows

Run this in PowerShell to download and install the latest release:

```powershell
irm https://raw.githubusercontent.com/rrtxt/git-pet/main/install.ps1 | iex
```

This:
* Installs `git-pet.exe` and its bundled DLLs to `%LOCALAPPDATA%\Programs\git-pet`.
* Installs the default pet assets to `%LOCALAPPDATA%\Programs\git-pet\pets\cat\` (an existing `pet.toml` is kept).
* Sets the `GIT_PET_PATH` user environment variable and adds git-pet to your user `PATH`.

Open a new terminal, then run `git-pet` inside any Git repository.

Alternatively, download `git-pet-windows-x86_64.zip` from the [Releases](https://github.com/rrtxt/git-pet/releases) page, extract it and run the bundled script:

```powershell
cd git-pet
powershell -ExecutionPolicy Bypass -File .\install.ps1
```

Options:

```powershell
.\install.ps1 -Uninstall                       # remove git-pet, its assets and environment variables
.\install.ps1 -InstallDir "D:\Tools\git-pet"   # install somewhere else
```

## How to build

### Prerequisites

You need the following installed on your system:
* A C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
* CMake (version 3.20 or newer)
* libgit2 (development files)
* pkg-config (to find libgit2)

On Debian/Ubuntu:
```bash
sudo apt-get install cmake build-essential libgit2-dev pkg-config
```

On macOS (using Homebrew):
```bash
brew install cmake libgit2 pkg-config
```

### Build Instructions (CMake)

1. Configure the build directory:
   ```bash
   cmake -B build
   ```

2. Compile the executable:
   ```bash
   cmake --build build
   ```

3. Run the application:
   ```bash
   ./build/git-pet
   ```

### Makefile Build (Alternative)

A standard Makefile is also provided for quick compiles. Run:
```bash
make
make run
```

### Configuration and Assets Setup

To run git-pet with animations, you need to configure your pet assets:

1. Create a pet configuration folder structure containing `pet.toml` inside a directory named after your pet's ID within the `GIT_PET_PATH` directory (e.g., `GIT_PET_PATH/cat/pet.toml`).
   Example directory structure:
   ```
   GIT_PET_PATH/
     cat/
       pet.toml
       egg/
         egg1.png
         egg2.png
       baby/
         baby1.png
       teen/
         teen1.png
   ```

   Example `pet.toml` configuration:
   ```toml
   name = "Cat"
   stages = ["egg", "baby", "teen"]

   [egg]
   fps = 5

   [baby]
   fps = 8

   [teen]
   fps = 10
   ```

2. Point the `GIT_PET_PATH` environment variable to the parent folder of your pet configuration:
   ```bash
   export GIT_PET_PATH=/path/to/assets/parent
   ```
   For example, if your pet configuration folder is at `/usr/local/share/git-pet/cat`, set `GIT_PET_PATH` to `/usr/local/share/git-pet`.

3. Configure your local repository pet settings by creating or editing the `pet.config` file inside your repository's local `.git/` directory (e.g., `.git/pet.config`):
   ```ini
   id = "cat"
   name = "Milo"
   ```
   * **id**: Specifies the target pet subdirectory to load within the `GIT_PET_PATH` (e.g., searching for the pet configuration and frame assets inside `/path/to/assets/parent/cat/`).
   * **name**: Specifies the display name of your pet in the interface.

## Whats next

* **Additional Interaction**: Support petting, feeding, or training actions triggered via key bindings.
* **Conditional Evolution**: Diversify pet evolution paths based on commit characteristics (e.g., chore commits versus feature/fix commits).
* **Self-Contained Executables**: Embed base pet assets directly inside the binary to run out-of-the-box without requiring environment variables.
* **Windows Support Improvements**: Refine pkg-config and dependency handling on Windows targets.
* **Bug Fixes**: Resolve the growth stage calculation where commit count > 100 resets the stage to Baby, and integrate the Adult stage.
