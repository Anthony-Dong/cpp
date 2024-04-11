#include <iostream>

#if defined(__has_include) && __has_include(<source_location>)
#include <source_location>
#define HAS_SOURCE_LOCATION 1
#else
#define HAS_SOURCE_LOCATION 0
#endif

#if HAS_SOURCE_LOCATION
void print_source_location(const std::source_location& location = std::source_location::current()) {
    std::cout << "info: " << location.file_name() << ":" << location.line() << ": " << location.function_name() << ": "
              << "hello" << '\n';
}
int main() {
    print_source_location();
}
#else
int main() {
    std::cout << "Has source_location: " << HAS_SOURCE_LOCATION << std::endl;
}
#endif
