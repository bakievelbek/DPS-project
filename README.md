## In C++, you can declare variables, functions, and classes in the following ways:

### Declaring variables:

```c++
int myVariable;
```

You can also declare multiple variables of the same type at once:

```c++
int var1, var2, var3;
```

And you can declare a variable and initialize it with a value:

```c++
int myVariable = 10;
```

### Declaring functions

To declare a function in C++, you need to specify the return type, the name of the function, and the parameters of the
function. For example:

```c++
int myFunction(int x, int y) {
    // function body
};
```

If the function does not return a value, you can use the void type as the return type:

```c++
void myFunction(int x, int y) {
    // function body
};
```

### Declaring classes

To declare a class in C++, you need to use the class keyword followed by the name of the class. For example:

```c++
class MyClass {
    // class body
};
```

Inside the class definition, you can declare class members such as variables and functions. For example:

```c++
class MyClass {
  public:
    int myVariable;
    void myFunction(int x);
  private:
    int anotherVariable;
};
```

## Code style in C++

Code style refers to the way that code is written and formatted. In C++, there are several conventions that are commonly
followed to ensure that code is easy to read, understand, and maintain. Here are a few examples of common C++ code style
conventions:

1. Use indentation to show the structure of the code:

```c++
if (condition) {
    // indented code
} else {
    // indented code
};
```

2. Use whitespace to improve readability:

```c++
int x = 10; // good
int y=20;   // bad

if (condition) {
    // code block
}; // good

if(condition){ // bad
    // code block
};
```

3. Use descriptive and meaningful names for variables, functions, and classes:

```c++
int age; // good
int x;   // bad
```

4. Use comments to explain the purpose of code blocks and to provide context for other developers:

```c++
// This function calculates the average of two numbers
double calculateAverage(double x, double y) {
return (x + y) / 2;
};
```

5. Follow naming conventions for variables, functions, and classes:

* Use camelCase for variables and functions: `int myVariable`;
* Use PascalCase for classes: `class MyClass {}`;
* Use UPPERCASE_WITH_UNDERSCORES for constants: `const int MY_CONSTANT = 10`;