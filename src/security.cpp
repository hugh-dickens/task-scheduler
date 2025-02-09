#include <unistd.h>

#include <iostream>

void preventRootExecution() {
    if (geteuid() == 0) {
        std::cerr << "Error: Do not run this program as root!" << "\n";
        exit(1);
    }
}
