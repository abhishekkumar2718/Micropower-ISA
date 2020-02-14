# uPower Assembler

A limited Micropower ISA assembler.

**Usage**
```
./bin/u_asm <path to asm file> [--debug] [--step-run]
```

## Features

- [x] Set up command line arguments
- [x] Read and pre process files
- [x] Initialize text and data segment
- [x] Initialize registers
- [x] Generate symbol table
- [ ] Parse instructions
- [ ] Encode instructions
- [ ] Execute instructions

## Installation

1. Clone the repository.

```bash
git clone git@github.com:abhishekkumar2718/Micropower-ISA.git && cd Micropower-ISA
```

2. Build the application.

```bash
make
```

3. Use the simulator on your micropower asm files.

```bash
./bin/u_sim <path to asm file>
```

4. [**Optional**] Run tests

```bash
make test
```

## References

1. [A Simple C++ Project Structure](https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/)
2. [MIPS Memory Layout](http://www.it.uu.se/education/course/homepage/os/vt18/module-0/mips-and-mars/mips-memory-layout/)
