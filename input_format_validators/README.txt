validate.cpp and validate.py strictly check the complete input syntax and all
stated constraints. They also check graph connectivity and verify that exactly
ceil(n/100) guide guesses are incorrect. A valid input causes exit status 42.

Compile and run from the project root:

  g++ -std=c++17 -O2 -Wall -Wextra -pedantic \
      input_format_validators/validate.cpp -o validate
  ./validate < data/sample/sample01.in

Alternatively, run the Python validator:

  python3 input_format_validators/validate.py < data/sample/sample01.in
