# Runner

This project was my third completed game, made back in 2015. It's a simple
endless runner game where you must jump over and slide under obstacles whilst
collecting as many coins as possible to get a highscore. A download for the
game can be found over on  **[itch.io](https://jrob774.itch.io/runner)**, the
game is also playable in browser on the same page.

## Development

The game was developed in C++ using the **[SDL2](https://libsdl.org/)** library.
A port for web browsers was also recently developed using
**[emscripten](https://github.com/emscripten-core/emscripten)**.

## Building

### Windows

Launch the `build/win32/win32` shortcut to setup a command prompt in the right
environment. The game can then be built by typing `build` in the console.
Different types of builds can be made by specifying an extra argument after the
initial build command, these arguments are:

```
debug   - builds a debug version of the game (default)
release - builds a release version of the game
final   - builds and packages a release version
```

In order to be able to execute builds Visual Studio must be installed. All
other dependencies (SDL2, etc.) are provided within the repository.

### Web

Launch the `build/web/web` shortcut to setup a command prompt in the right
environment. The game can then be built by typing `build` in the console.
Different types of builds can be made by specifying an extra argument after
the initial build command, these arguments are:

```
release - builds a release version of the game (default)
final   - builds and packages a release version
```

In order to be able to execute builds emscripten must be installed. By default
the build script looks for emscripten at the relative location
`../../../.emsdk`, in order to change this path open the 'build/web/setup.bat'
file and change the `emsdk_location` variable.

## License

The project's code is available under the
**[MIT License](https://github.com/JROB774/runner/blob/master/LICENSE)**.
