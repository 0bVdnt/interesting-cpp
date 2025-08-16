
/* NOTE:
 * CTAD (Class Template Argument Deduction) is a C++17 feature that lets the
 * compiler deduce template arguments from constructor arguments, so you don’t
 * have to explicitly specify them.
 */

#include <cstddef>
#include <cxxabi.h>
#include <initializer_list>
#include <iostream>
#include <typeinfo>
#include <vector>

// A wrapper class around any type T.
// Prints messages whenever constructors are invoked.
template <class T> class TypedClass {
  T val;

public:
  TypedClass() {
    int status;
    std::cout << "Default constructor of TypedClass<"
              << abi::__cxa_demangle(typeid(T).name(), 0, 0, &status) << ">"
              << std::endl;
  }
  TypedClass(const T &x) : val(x) {
    int status;
    std::cout << "Parameterized constructor of TypedClass<"
              << abi::__cxa_demangle(typeid(T).name(), 0, 0, &status) << ">"
              << std::endl;
  }

  // Explicit copy constructor: invoked when copying another TypedClass<T>
  TypedClass(const TypedClass &obj) : val(obj.val) {
    int status;
    std::cout << "Copy constructor of TypedClass<"
              << abi::__cxa_demangle(typeid(T).name(), 0, 0, &status) << ">"
              << " with value " << val << std::endl;
  }

  // Getter
  T getData() const { return val; }
};

// A wrapper around std::vector<T> with a few constructors
template <class T> class DynamicArray {
  std::vector<T> arr;

public:
  // Empty vector
  DynamicArray() : arr() {}

  // Constructs vector with 'sz' default-initialized Ts
  DynamicArray(size_t sz) : arr(sz, T{}) {}

  // Constructs vector from an initializer list {a, b, c, ...}
  DynamicArray(std::initializer_list<T> init) : arr(init) {
    int status;
    std::cout << "Used initializer list in DynamicArray<"
              << abi::__cxa_demangle(typeid(T).name(), 0, 0, &status) << ">"
              << std::endl;
  }

  // Constructs vector with 'sz' copies of 'val'
  DynamicArray(size_t sz, const T &val) : arr(sz, val) {}

  // Getter
  std::vector<T> &getArr() { return arr; }
};

// Deduction guide:
// If DynamicArray(size_t, T) is called,
// deduce DynamicArray<TypedClass<T>>.
//
// Example: DynamicArray(5, 1.3)
//   -> becomes DynamicArray<TypedClass<double>>.
template <typename T> DynamicArray(size_t, T) -> DynamicArray<TypedClass<T>>;

int main() {
  // Case 1: Single element {0} → deduces DynamicArray<int>
  // Calls initializer_list<int> constructor  std::cout << "1." << std::endl;
  DynamicArray arr{0};
  for (int j = 0; const auto &i : arr.getArr()) {
    std::cout << j++ << ": " << i << std::endl;
  }
  std::cout << "--------------------------------------------------------------"
            << std::endl;

  // Case 2: Braced list {10.0, 1.3} → deduces DynamicArray<double>
  // Calls initializer_list<double> constructor
  DynamicArray arr1{10.0, 1.3};
  for (int j = 0; const auto &i : arr1.getArr()) {
    std::cout << j++ << ": " << i << std::endl;
  }
  std::cout << "--------------------------------------------------------------"
            << std::endl;

  // Case 3: Braced List {10, 1.3}
  // When braced initialization is used, the std::initializer_list constructor
  // takes precedence over other constructors. However, the element type T of
  // the initializer_list still needs to be deduced. This is where the deduction
  // guide comes into play: the first argument (10) is interpreted as size_t and
  // the second (1.3) as T = double. As a result, the initializer_list contains
  // elements of type TypedClass<double>. Each element is first constructed via
  // TypedClass<double>(value), then copied into the vector, invoking the copy
  // constructor. Therefore, the resulting object is of type
  // DynamicArray<TypedClass<double>> rather than DynamicArray<double>.
  DynamicArray arr2{10, 1.3};
  for (int j = 0; const auto &i : arr2.getArr()) {
    std::cout << j++ << ": " << i.getData() << std::endl;
  }
  std::cout << "--------------------------------------------------------------"
            << std::endl;

  // Case 4: Braced list of explicit TypedClass<double> objects
  // Deduction: DynamicArray<TypedClass<double>>
  // Calls initializer_list<TypedClass<double>> constructor directly
  DynamicArray typedClassArr{TypedClass{10.34}, TypedClass{9.23},
                             TypedClass{3.14}};
  for (int j = 0; const auto &i : typedClassArr.getArr()) {
    std::cout << j++ << ": " << i.getData() << std::endl;
  }
  std::cout << "--------------------------------------------------------------"
            << std::endl;

  // Case 5: Parentheses (5, 1.3)
  // Deduction guide applies: (size_t, T) -> DynamicArray<TypedClass<T>>
  //  -> T = double, so type is DynamicArray<TypedClass<double>>
  // Calls (size_t sz, const TypedClass<double>& val) constructor
  // Creates 5 copies of TypedClass<double>(1.3)
  DynamicArray typedDataArr(5, 1.3);
  for (int j = 0; const auto &i : typedDataArr.getArr()) {
    std::cout << j++ << ": " << i.getData() << std::endl;
  }
  std::cout << "--------------------------------------------------------------";
}
