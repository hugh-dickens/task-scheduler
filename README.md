# Task Scheduler with TCP Communication

## Overview
This project implements a **multi-threaded task scheduler** that receives tasks over a **TCP network**. The system consists of:

- A **Task Scheduler** that schedules and executes commands at a given delay.
- A **Server** that listens for client connections and forwards commands to the scheduler.
- A **Client** that sends commands to be scheduled.

The project supports **Linux, Windows (via WSL)**. However, tested on Windows via WSL running on Ubuntu 22.04.

---

## Features
- ✅ Multi-threaded task scheduler using priority queues.
- ✅ TCP-based communication between client and server.
- ✅ Executes system commands (e.g., `ls`, `echo Hello`).
- ✅ Uses `std::system()` to run commands.
- ✅ Designed for scalability with separate client and server binaries.

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
sudo apt install -y build-essential cmake
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
./Server
```

4. Running the Client
```sh
./Client
```

### Example Usage

After running the Server then the Client, the terminal will prompt you for a command e.g. `ls`, and a delay e.g. `2s`

### Future Improvements
- Logging system for executed tasks. 
- Secure command execution (prevent dangerous commands). 
- WebSocket integration for real-time task updates.

### License

This project is open-source and available under the **MIT License**.
