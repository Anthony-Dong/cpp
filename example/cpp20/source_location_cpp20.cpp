#include <iostream>

#if __cplusplus >= 202002L
#include <source_location>
void print_source_location(const std::source_location& location = std::source_location::current()) {
    std::cout << "info: " << location.file_name() << ":" << location.line() << ": " << location.function_name() << ": "
              << "hello" << '\n';
}
int main() {
    print_source_location();
}
#else
int main() {
}
#endif
