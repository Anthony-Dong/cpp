#include <fstream>

int main() {
    std::fstream f("");

    f.write("hello world", 10);
}