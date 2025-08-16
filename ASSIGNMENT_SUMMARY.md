# Programming Assignment Summary (C Version)

## Requirements Fulfilled ✅

### 1. Array Creation

- ✅ Created an array of size 10,000 (`int buffer[ARRAY_SIZE]` with `ARRAY_SIZE = 10000`)

### 2. Two Threads

- ✅ **Producer Thread**: Generates random numbers and writes to array
- ✅ **Consumer Thread**: Reads values from array and displays them

### 3. Producer Thread Functionality

- ✅ Generates random numbers between 0 and 1000
- ✅ Enters critical section controlled by semaphores
- ✅ Writes generated values to the pre-defined array
- ✅ Writes to "Producer_activity.txt" with format: "Producer produced item number X with the value Y"
- ✅ Exits critical section
- ✅ Repeats 10,000 times

### 4. Consumer Thread Functionality

- ✅ Requests entrance to critical section controlled by semaphores
- ✅ Reads values from the array
- ✅ Writes to screen with format: "Consumer consumed item number X with the value Y"
- ✅ Writes to "Consumer_activity.txt"
- ✅ Exits critical section
- ✅ Repeats until producer finishes production

### 5. Synchronization (MANDATORY)

- ✅ **Two semaphores implemented** using POSIX semaphores (`sem_t`):
  - `notFull`: Ensures producer doesn't overflow buffer
  - `notEmpty`: Ensures consumer doesn't read from empty buffer

## Technical Implementation Details

### Synchronization Mechanism

```c
pthread_mutex_t mutex;
sem_t notFull;   // Semaphore 1
sem_t notEmpty;  // Semaphore 2
```

### Critical Section Control

- **Producer**: Waits for `notFull` before entering critical section
- **Consumer**: Waits for `notEmpty` before entering critical section
- Both threads properly signal each other when exiting critical sections

### File Output

- **Producer_activity.txt**: Contains all producer activities
- **Consumer_activity.txt**: Contains all consumer activities
- Both files use the exact format specified in requirements

### Random Number Generation

- Uses `rand()` for random number generation
- Range: 0 to 1000 (inclusive)
- Generates exactly 10,000 numbers

## Program Files Created

1. **`producer_consumer.c`** - Main program source code
2. **`producer_consumer.exe`** - Compiled executable
3. **`Makefile`** - For compilation on Unix-like systems
4. **`compile.bat`** - For compilation on Windows
5. **`README.md`** - Instructions and documentation
6. **`Producer_activity.txt`** - Producer output file
7. **`Consumer_activity.txt`** - Consumer output file

## How to Run

### Windows

```bash
# Option 1: Use batch file
compile.bat

# Option 2: Direct compilation
gcc -Wall -Wextra -O2 -pthread -o producer_consumer.exe producer_consumer.c
./producer_consumer.exe
```

### Unix/Linux

```bash
make
./producer_consumer
```

## Verification

The program successfully:

- ✅ Creates 10,000 random numbers (0-1000)
- ✅ Writes all producer activities to file
- ✅ Displays all consumer activities on screen
- ✅ Writes all consumer activities to file
- ✅ Maintains proper synchronization throughout execution
- ✅ Terminates cleanly when all items are processed

## Sample Output Verification

Both output files contain the exact format required:

- Producer: "Producer produced item number 1 with the value 842"
- Consumer: "Consumer consumed item number 1 with the value 842"

The values match between producer and consumer files, confirming proper synchronization and data integrity.

## C-Specific Implementation Notes

- Uses POSIX threads (`pthread.h`)
- Uses POSIX semaphores (`semaphore.h`)
- Uses standard C file I/O (`stdio.h`)
- Proper memory management with automatic cleanup
- Thread-safe random number generation
- Immediate file flushing for real-time output
