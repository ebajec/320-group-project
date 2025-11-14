# Setup

## Package installation

Make sure CMake is installed on your machine, along with a C++ compiler

### Fedora/RHEL

```
dnf install raylib raylib-devel
```

### MacOS

```
brew update

brew install raylib
```

## Building 

Configure cmake and build

## Player profiles & persistence

Running the game now creates/updates a `profile.env` file (next to the executable). It stores a simple account so every clone of the repo still feels personal. You can edit it manually before launching to pick a display name or starting bankroll.

```
PLAYER_NAME=Guest
BANKROLL=1000
BLACKJACK_WINS=0
BLACKJACK_LOSSES=0
BLACKJACK_PUSHES=0
ROULETTE_SPINS=0
ROULETTE_NET=0
```

Both Blackjack and Roulette read/write these stats in real time. Delete the file if you want to reset your account.

## Window resizing

The raylib window is resizable (min size 640x480). Layouts for the main menu and both games respond to size changes, so feel free to stretch the window while playing.

## UI helpers

Reusable raylib helpers live under `src/ui/`:

- `ui/button.hpp` – minimal rectangle button struct with helper functions to draw, hover, and click detection.
- `ui/input.hpp` – tiny wrappers for mouse state queries you can call from anywhere.
- `ui/ui.hpp` – shared inline helpers (e.g., quick text labels) plus umbrella include for the rest of the UI pieces.

Include `ui/ui.hpp` in any state or screen that needs buttons or mouse helpers, and call `ui::DrawButton()` + `ui::IsButtonClicked()` as needed.
