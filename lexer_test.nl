import std.io;
import std.fmt;

typedef u8 uchar_t;
typedef s16 wchar_t;

// Very cool C++ comment // help

/* Very cool C comment
 * Help me pls
 * k, bye!
 */

struct CoolStruct {
  struct {
    string some_string = "Hello";
  } nested_struct;

  int m_iVal = -32;
  uchar_t m_uVal = 3;
  bool m_bVal = true;

  [result] // Idk how i must call it, so use `result` for now
  bool is_ival_positive() {
    return m_iVal >= 0;
  }

  uchar_t get_uval() {
    return m_uVal;
  }
}

int main(int argc, char **argv) {
  int result = 0;

  CoolStruct struct1 = {0}; // Zeroed out
  bool is_positive1 = struct1.is_ival_positive(); // Should return true

  CoolStruct struct2; // Preinitialized with default values
  println(struct2.nested_struct.some_string); // Print "Hello"

  char char_array[42] = {
    // First 5 values will be initializes with these values:
    [0] = "H",
    [1] = "e",
    [2] = "l",
    [3] = "l",
    [4] = "o",

    // Everything else will be zeroed out
  };
  println(char_array); // Print "Hello"

  return result;
}
