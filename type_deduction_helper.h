///////////////////////////////////////////////////////////
// some compile-time diagnostics for auto type deduction //
///////////////////////////////////////////////////////////

/** undefined templated class that can be used to get the type of a variable as a compiler error message.
 * usage:
 * \code
 * TD<decltype(var)> varType; // elicit error containing the type of var
 * \endcode
 *
 * Taken from: Effective Modern C++, Scott Meyers, O'Reilly Media, Inc. (R) 2014
 */
template<typename T>
class TD;
