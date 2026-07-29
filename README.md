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

1. Create a pet configuration folder containing a `pet.toml` and subfolders with frame PNGs for each stage.
   Example directory structure:
   ```
   assets/
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
   For example, if your folder is `/usr/local/share/git-pet/cat`, set `GIT_PET_PATH` to `/usr/local/share/git-pet`. The local configuration will default to loading "cat".

## Whats next

* **Additional Interaction**: Support petting, feeding, or training actions triggered via key bindings.
* **Conditional Evolution**: Diversify pet evolution paths based on commit characteristics (e.g., chore commits versus feature/fix commits).
* **Self-Contained Executables**: Embed base pet assets directly inside the binary to run out-of-the-box without requiring environment variables.
* **Windows Support Improvements**: Refine pkg-config and dependency handling on Windows targets.
* **Bug Fixes**: Resolve the growth stage calculation where commit count > 100 resets the stage to Baby, and integrate the Adult stage.
