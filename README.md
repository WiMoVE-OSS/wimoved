
## Building

Build the project by running `cmake .` followed by `make -j$(nproc)`.

## Formatting

Format the source files by running `make format`.
Lint the source files by running `make lint`.
Run both checks and the tests by doing `make precommit`.

As linting takes a long time, we recommend integrating `clang-tidy` into your editor.

## Testing

Build the tests by running `make test`.