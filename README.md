# C-Memory-Administration

A C program that simulates dynamic memory management using malloc, calloc, realloc, and free functions from an input file containing a sequence of operations. It demonstrates memory problems such as fragmentation and memory leaks.

## Features

- **Memory Simulation**: Allocates a large memory block (1MB) from the OS and performs all operations within it
- **First Fit Algorithm**: Uses First Fit strategy to find free memory blocks
- **Memory Tracking**: Maintains a control table of all allocated blocks
- **Visual Memory Map**: Shows memory state with visual representation
- **Memory Statistics**: Displays allocation statistics and memory usage

## Supported Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| `MALLOC` | `MALLOC <name> <size>` | Allocate memory block of specified size |
| `FREE` | `FREE <name>` | Free previously allocated memory block |
| `REALLOC` | `REALLOC <name> <new_size>` | Resize existing memory block |
| `PRINT` | `PRINT` | Display current memory allocation status |

## Compilation and Execution

### Compile the program:
```bash
gcc -o memory_admin main.c
```

### Run the program:
```bash
./memory_admin <command_file> <fit_algorithm>
```

### Example command file (`commands.txt`):
```
MALLOC A 100
MALLOC B 50
MALLOC C 200
PRINT
FREE B
MALLOC D 75
PRINT
REALLOC A 150
PRINT
FREE A
FREE C
FREE D
PRINT
```

## Program Structure

### Core Functions

| Function | Purpose | Implementation |
|----------|---------|----------------|
| `malloc()` | Allocate a block of memory of given byte size | Uses First Fit algorithm to find free space |
| `free()` | Release memory previously allocated | Marks memory as free and updates control table |
| `realloc()` | Resize (expand or shrink) an existing allocated block | Frees old block and allocates new one |
| `print_memory()` | Display current memory allocation status | Shows detailed allocation table and statistics |

### Memory Management Features

- **Block Control Table**: Tracks start position, size, name, and status of each block
- **First Fit Allocation**: Finds the first available space that fits the requested size
- **Memory Visualization**: Shows memory state using characters (`.` for free, letters for allocated)
- **Error Handling**: Detects allocation failures, double-free attempts, and invalid operations

## Output Example

```
MALLOC A 100: Allocated at position 0
MALLOC B 50: Allocated at position 100
MALLOC C 200: Allocated at position 150

=== CURRENT MEMORY ALLOCATION STATUS ===

Currently allocated blocks:
Name   | Start  | Size   | End
-------|--------|--------|--------
   A   |      0 |    100 |     99
   B   |    100 |     50 |    149
   C   |    150 |    200 |    349
```

## Memory Problems Demonstrated

- **Fragmentation**: Shows how freed blocks create gaps in memory
- **Memory Leaks**: Tracks blocks that are allocated but never freed
- **Allocation Failures**: Demonstrates what happens when no suitable block is found

