/*
 * NOTE: C++ Value Categories
 * In C++, every expression has a value category, which determines whether it
 * represents an object that can be identified (an lvalue) or a temporary/
 * unnamed object that cannot be identified (a rvalue). These categories are
 * fundamental to understanding how C++ handles expressions and move semantics.
 */

#include <iostream>
#include <vector>

int main() {
  // glvalue (generalized lvalue)
  //  A glvalue is an expression that has an identity. It can be either an
  //  lvalue or an xvalue.

  // lvalue (left-hand side value) An lvalue is an expression that
  //  refers to a memory location and has an identity. It can appear on the left
  //  side of an assignment operator. Examples include variables, function
  //  return values that are references, and dereferenced pointers.
  int x = 10; // `x` is an lvalue
  int &y = x; // `y` is an lvalue reference, so it refers to the lvalue `x`
  *(&x) = 11; // The dereferenced pointer `*(&x)` is an lvalue

  // rvalue(right - hand side value)
  //  An rvalue is an expression that is not an lvalue. It represents a
  //  temporary, unnamed object. It can only appear on the right side of an
  //  assignment operator. Examples include literals, function calls that return
  //  by value, and temporary objects.
  int a = 5;            // `5` is an rvalue
  int b = a + 2;        // `a + 2` is an rvalue
  int c = std::move(a); // `std::move(a)` returns an rvalue reference

  // prvalue (pure rvalue)
  //  A prvalue is the most basic type of rvalue. It is an expression that
  //  creates a temporary object. All rvalues are either prvalues or xvalues.
  int d = 10;      // `10` is a prvalue
  int sum = 1 + 2; // `1 + 2` is a prvalue

  // xvalue (expiring value)
  //  An xvalue is an rvalue that refers to an object that can be moved from. It
  //  is typically created by casting an lvalue to an rvalue reference. This is
  //  what std::move does.
  int e = 10;
  int f = std::move(x); // `std::move(x)` is an xvalue

  /* NOTE:
     - An lvalue is an object you can name and find a memory address for.
     - A prvalue is a temporary object that is about to be created.
     - An xvalue is a temporary object that is about to be "moved" from.
     - An rvalue is either a prvalue or an xvalue.
     This is critical for move semantics, which allows for efficient transfer
     of resources (like memory from a large array) from a temporary object to
     a new object without creating a deep copy. Instead of copying, the new
     object "steals" the resources from the temporary object, which is then
     destroyed.
    */

  // std::move(Tp &&t) : Does not specifically move something. It's cast that
  // converts an lvalue into an xvalue (expiring value). This conversion is what
  // makes move semantics possible. The actual "move" operation happens in the
  // constructor or assignment operator of the object that receives the xvalue.
  std::vector<int> source = {1, 2, 3};
  std::cout << "Source vector size before move: " << source.size() << std::endl;

  // This is where the magic happens.
  // std::move(source) casts `source` from an lvalue to an xvalue.
  // The move constructor of destination is called, which "steals" the resources
  // from source.
  std::vector<int> destination = std::move(source);

  std::cout << "Source vector size after move: " << source.size() << std::endl;
  std::cout << "Destination vector size after move: " << destination.size()
            << std::endl;
  // Uses
  //  Move Semantics: The primary use of std::move is to enable move semantics,
  //  which can significantly improve performance by avoiding costly deep
  //  copies. This is particularly useful for objects that manage resources like
  //  dynamic memory, file handles, or network connections.
  //
  //  Function Arguments: std::move can be used when passing an object to a
  //  function by value to ensure that a move constructor is used instead of a
  //  copy constructor, if a move constructor is available.
  //
  //  Returning Values: While not always necessary due to Return Value
  //  Optimization (RVO), std::move can be used to explicitly move local
  //  variables out of a function. However, using it on named return values can
  //  sometimes inhibit RVO and decrease performance.

  // Limitations
  //  Doesn't Always Move: std::move is just a cast. If the object you are
  //  moving to does not have a move constructor or move assignment operator,
  //  the compiler will fall back to the copy constructor/assignment operator.
  //
  //  Leaves Object in a Valid but Unspecified State: After a move, the
  //  moved-from object is in a "valid but unspecified" state. It can't be
  //  relied on its contents or state. It's generally a good practice to only
  //  destroy or reassign the moved-from object.
  //
  //  Not a Physical Move: std::move does not guarantee a physical move of data
  //  in memory. The "move" is a conceptual transfer of ownership. It is up to
  //  the class's move constructor to implement an efficient resource transfer.
  //
  //  Performance Overhead: While typically faster than a deep copy, std::move
  //  is not free. The move constructor still performs some operations (like
  //  transferring pointers and updating states).
  //
  //  Final Use: Once you have moved from an object, it shouldn't be use it
  //  anymore, with the exception of assigning a new value to it or letting it
  //  go out of scope. Using a moved-from object can lead to undefined behavior.
  return 0;
}
