validate.cpp checks the complete input syntax and all stated constraints. It
also checks graph connectivity and verifies that exactly ceil(n/100) guide
guesses are incorrect. A valid input causes exit status 42.

Compile and run from the repository root:

  g++ -std=c++17 -O2 -Wall -Wextra -pedantic \
      input_format_validators/validate.cpp -o validate
  ./validate < data/sample/sample01.in
