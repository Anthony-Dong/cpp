struct X {
    typedef int type;
};

struct Y {
    typedef int type2;
};

struct Z {};

template <typename T>
void foo(typename T::type){}; // Foo0
template <typename T>
void foo(typename T::type2){}; // Foo1
template <typename T>
void foo(T){}; // Foo2

int main() {
    foo<X>(5);       // Foo0: Succeed, Foo1: Failed,  Foo2: Failed
    foo<Y>(10);      // Foo0: Failed,  Foo1: Succeed, Foo2: Failed
    foo<Z>(Z{});     // Foo0: Failed,  Foo1: Succeed, Foo2: Failed
    foo<int>(15);    // Foo0: Failed,  Foo1: Failed,  Foo2: Succeed
    foo<bool>(true); // Foo0: Failed,  Foo1: Failed,  Foo2: Succeed
}