
## Building

Build the project by running `cmake .` followed by `make -j$(nproc)`.

## Formatting

Format the source files by running 

```bash
find src -iname *.h -o -iname *.cpp | xargs clang-format -i
```