# C on WSL

From PowerShell: wsl -d Ubuntu

Then:
    cd ~/c-programming/hello
    make run

Edit main.c and run make run again.

- make: compile with C17, warnings, and debug symbols.
- make debug: start GDB. Enter "break main", "run", "next", then "quit".
- make check: run AddressSanitizer/UndefinedBehaviorSanitizer and Valgrind.
- make clean: remove generated executables.
- code .: open the folder using VS Code's WSL connection.

In VS Code, install Microsoft's C/C++ extension in WSL when prompted.
Ctrl+Shift+B builds; F5 debugs.

Tools: GCC, G++, Make, GDB, Clang, clangd, clang-format, CMake, Ninja,
Valgrind, Git, pkg-config, and C development manual pages.

Keep projects under ~/c-programming inside Linux.

For another single file:
    gcc -std=c17 -Wall -Wextra -Wpedantic -g program.c -o program
    ./program
