#include "logguard/core/Version.hpp"

#include <iostream>

int main() {
    std::cout << "logguard " << logguard::Version() << '\n';
    return 0;
}
