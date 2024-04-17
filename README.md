## league++
a league client written in c++

## view compiler
a simple compiler for the frontend to get it to a usable format

### running the compiler
```bash
$ node --no-deprecation view_compiler.js -c CONFIG
```

## building
The build process has 2 steps
* CMake (the C++ backend code)
* HTML/CSS/JS (the frontend)

### how to build
build the C++ first in either Debug / Release. this will create the base files for the 2nd step

after that run the view compiler with the same config.

## cloc
```bash
$ npm run cloc
```