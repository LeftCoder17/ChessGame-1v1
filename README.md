# Chess App

This Chess Game is still on development.

# To build it and execute it
In project_dir, create build directory

```
mkdir build
```

Now create the builder

```
cmake -S . -B .\build
```

Now build the project

```
cmake --build .\build
```

This will create the ChessApp inside build.

If changes made, with the cmd in the build directory, use:

```
ninja clean
cmake --build .
```
