# JSON Parser and Container

A C++17 university project developed for the Programming and Laboratory course (academic year 2022–2023).

The project implements a parser for a simplified JSON format and a custom C++ container to store, access, and manipulate the parsed data.

## Features

- Parsing of JSON values: null, booleans, numbers, strings, lists, and dictionaries.
- Support for nested lists and dictionaries.
- Custom container implementation using linked lists.
- Custom forward iterators for lists and dictionaries.
- Operator overloading for JSON input and output streams.
- Copy and move constructors and assignment operators.
- Exception handling for parsing errors and invalid operations.

## Implementation

The container uses a Pimpl-based design and manually manages dynamically allocated memory.

The implementation was developed as a university assignment and received a grade of **30/30**.

## Requirements

- A C++17-compatible compiler, such as GCC.
- The provided `json.hpp` header.

## Project Structure

- `json.cpp` — implementation of the JSON parser and container.
- `json.hpp` — class declarations and required interfaces.

## Compilation

The project does not include a `main()` function because it was designed to be tested by an external driver.

To compile the implementation into an object file with GCC:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -c json.cpp
```

This produces `json.o`. It does not create an executable; a separate program using the `json` class is required to link and run it.

## Scope

This project was developed for educational purposes and implements the simplified JSON format specified in the original assignment.
