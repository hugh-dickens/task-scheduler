# Task Scheduler with TCP Communication

## Overview
This project implements a **multi-threaded task scheduler** that receives tasks over a **TCP network**. The system consists of:

- A **Task Scheduler** that schedules and executes commands at a given delay.
- A **Server** that listens for client connections and forwards commands to the scheduler.
- A **Client** that sends commands to be scheduled.

The project supports **Linux, Windows (via WSL)**. However, tested on Windows via WSL running on Ubuntu 22.04. It may work on other systems but this is not supported nor tested.

---

## Features
- ✅ Multi-threaded task scheduler using priority queues.
- ✅ TCP-based communication between client and server.
- ✅ Executes system commands (e.g., `ls`, `echo Hello`) and file conversions (e.g. csv to JSON).
- ✅ Uses `std::system()` to run commands.
- ✅ Designed for scalability with separate client and server binaries.
- ✅ Logging system for executed tasks. 
- ✅ Secure command execution, denies root execution. Requires extension. 

---

## System Requirements
- **C++20** or later
- **CMake (3.10+)**
- **g++ (GCC) or Clang**
- **pthread library** (for multithreading)
- **WSL** (if using Windows)

---

## Setup Instructions

### **Linux (Ubuntu, Debian, Arch, etc.)**
```sh
# Install required dependencies
sudo apt update
sudo apt install -y build-essential cmake clang-format clang-tidy cppcheck libgtest-dev googletest
```

### **Windows (Using WSL)**
1. Install and setup WSL. I installed Ubuntu 22.04 and then ran the same as above in the downloaded Ubuntu app.

### Building the Project
1. Clone the Repo

2. Build the Project using CMake
```sh
mkdir build && cd build
cmake ..
make
```
3. Running the Server
```sh 
./bin/Server
```

4. Running the Client
```sh
./bin/Client
```

### Example Usage

After running the Server then the Client, the terminal will prompt you for a command e.g. `ls`, and a delay e.g. `2s`

### Static Analysis

This project used clang-tidy and cpp check for static analysis. To run:

1. Build the Project
```sh
cmake -B build
```

2. Run ```clang-tidy```
```sh
cmake --build build --target run_clang_tidy
```

3. Run ```cppcheck```
```sh
cmake --build build --target run_cppcheck
```

### Code formatting

Setup with a pre-commit hook, however, to manually run:

```sh
clang-format -i src/*.cpp include/*.hpp
```

### Unit tests

Using the googletest framework with GMock and GTest. To run the tests after building the project:

```sh
./bin/run_tests
```

NB: currently on test_imports exists... to be extended.


### License

This project is open-source and available under the **MIT License**.
