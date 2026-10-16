# PPOIS Labs

Репозиторий лабораторных работ по дисциплине PPOIS.

## Structure

| Lab | Language | Contents | Documentation |
|---|---|---|---|
| 1 | C++17 | [Kubick](./lab1/Kubick), [Markov](./lab1/Markov) | Doxygen |

## Requirements

- C++17 compatible compiler (`g++`)
- [CMake](https://cmake.org/)
- [Catch2 v3](https://github.com/catchorg/Catch2) — unit testing
- [gcovr](https://gcovr.com/) — code coverage analysis
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) — static analysis
- [Doxygen](https://www.doxygen.nl/) — documentation generation

## Lab 1

### Kubick

Implementation of a cube model with operations for manipulating its state.

Source code, unit tests, CMake configuration and Doxygen configuration are located in `lab1/Kubick/`.

### Markov

Implementation of a Markov algorithm for string transformations.

Source code, unit tests, CMake configuration and Doxygen configuration are located in `lab1/Markov/`.

## Build and run

Build each project from its own directory.

### Kubick

```bash
cd lab1/Kubick
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

### Markov

```bash
cd lab1/Markov
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

## Code coverage

The projects use `gcovr` to generate code coverage reports. The CI workflow requires at least 91% line coverage.

Example:

```bash
gcovr --root . --object-directory build \
  --exclude 'tests/.*' \
  --exclude 'build/.*' \
  --print-summary
```

Run the command from the corresponding project directory after configuring and building with coverage enabled.

## Static analysis

`clang-tidy` is used to analyze the C++ source code.

Example for Kubick:

```bash
clang-tidy kubik.cpp -p build
```

Example for Markov:

```bash
clang-tidy algos.cpp -p build
```

## Documentation

Documentation is generated with [Doxygen](https://www.doxygen.nl/) using the `Doxyfile` configuration in each project.

To generate documentation, run:

```bash
cd lab1/Kubick
doxygen Doxyfile
```

For Markov:

```bash
cd lab1/Markov
doxygen Doxyfile
```

Open the generated `html/index.html` file in a browser. The exact output directory is defined by each project's `Doxyfile`.

## CI/CD

GitHub Actions automates the following tasks:

- Building both projects.
- Running unit tests.
- Checking line coverage against the configured threshold.
- Running static analysis.
- Building release executables and preparing a Linux archive.
- Publishing a GitHub Release after a successful workflow run on `main`, when configured.

## Releases

Downloadable builds are intended to be published on the repository's [GitHub Releases](https://github.com/pavlence32/Ppois/releases) page.

Release archives contain prebuilt Linux executables. Extract the archive and run the appropriate executable from a terminal.

## Repository

[GitHub repository](https://github.com/pavlence32/Ppois)
