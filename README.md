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

## UI helpers

Reusable raylib helpers live under `src/ui/`:

- `ui/button.hpp` – minimal rectangle button struct with helper functions to draw, hover, and click detection.
- `ui/input.hpp` – tiny wrappers for mouse state queries you can call from anywhere.
- `ui/ui.hpp` – shared inline helpers (e.g., quick text labels) plus umbrella include for the rest of the UI pieces.

Include `ui/ui.hpp` in any state or screen that needs buttons or mouse helpers, and call `ui::DrawButton()` + `ui::IsButtonClicked()` as needed.
