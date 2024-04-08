#include <queue>
#include <iostream>

int main(){
    std::queue<int> queue;
    queue.push(1);
    queue.push(2);
    std::cout << queue.front() << std::endl;
    queue.pop();
}