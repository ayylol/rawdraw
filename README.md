# Bimgus C template

Skeleton C project to use as a quick and general starting point. 

The makefile compiles everything in the src directory up to two subdirectories
down. It also includes those directories.

Default executable name is `a.out`, remember to change it!

includes targets: 
- `debug`/`release` which compile the program, adding the appropriate flags;
- `clean` which removes the build directory and the executable;
- `rremake`/`dremake` which runs clean and then compiles either in relase/debug;
- `echo` prints out the relevant files
- `all` compiles the program in release mode


