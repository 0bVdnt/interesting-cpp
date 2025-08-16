#include <cxxabi.h> // To convert typenames into readable names
#include <iostream>
#include <type_traits> // Compile time utilities for querying and modifying
                       // templates, defines std::is_same_v<T1, T2>
#include <typeinfo>    // For typeid operator and std::type_info class

template <class T> class Id {
public:
  Id() { std::cout << "Hello from Id" << std::endl; }
};

// Checking actual type at the runtime
template <typename T> void checkType(const T &value) {
  std::cout << "Type: " << typeid(T).name() << "\n";
};

template <typename T> void checkTypeDem(const T &value) {
  int status;
  std::cout << "Demangled Type: "
            << abi::__cxa_demangle(typeid(T).name(), 0, 0, &status)
            << std::endl;
}

// Compile time checks of types using `if constexpr` and type traits
template <typename T> void compileTimeTypeCheck(const T &) {
  if constexpr (std::is_same_v<T, int>)
    std::cout << "Type at compile time is int" << std::endl;
  else if constexpr (std::is_same_v<T, double>)
    std::cout << "Type at compile time is double" << std::endl;
  else if constexpr (std::is_same_v<T, Id<float>>)
    std::cout << "Type at compile time is Id<float>" << std::endl;
  else
    std::cout << "Type at compile time is something else" << std::endl;
}

int main() {
  Id<float> i;

  // Without *demangling
  checkType(42);      // prints int
  checkType(3.14);    // prints double
  checkType("hello"); // prints char const*
  checkType(i);       // prints the code for the type Id<float>

  // With demangling
  checkTypeDem(42);
  checkTypeDem(3.14);
  checkTypeDem("Hello");
  checkTypeDem(i);

  Id<double> j;
  // Compile time typechecks
  compileTimeTypeCheck(42);
  compileTimeTypeCheck(3.14);
  compileTimeTypeCheck("Hello");
  compileTimeTypeCheck(i);
  compileTimeTypeCheck(j);
}

/*
 NOTE:
 - Transforming C++ ABI identifiers (like RTTI (Runtime Type Information)
 symbols) into the original C++ source identifiers is called “demangling.”
 */
