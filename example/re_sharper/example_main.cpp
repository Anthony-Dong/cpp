#include "absl/time/clock.h"
#include "absl/time/time.h"

#include <iostream>

int test() {
    return 1;
}

int main() {
    std::cout << absl::FormatTime(absl::Now()) << "\n";
    auto x = test();
}