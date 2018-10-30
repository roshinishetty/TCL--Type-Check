# TCL Type Check And Type Equivalence

Type is checked and Name, Internal name and Structural equivalence  check for TCL language using C programming.

## Getting Started

### Prerequisites

This project requires the following for running on the system:
```
gcc compiler
```

### Installing

* Clone or download the plugin into the ```wp-content/plugins ``` directory in your site.
```
https://github.com/roshinishetty/TCL--Type-Check.git
```
* Activate the plugin.
``` 
wp plugin activate TCL--Type-Check
```

## Running the tests

Steps to run code1:
1. compile code1 - ```gcc -o code1 code1.c ```
2. run code1 - ```./code1 ```
3. enter tcl script file to be run on

Example:
```
>>> gcc -o code1 code1.c
>>> ./code1
t1.tcl
```

Steps to run code2:
1. compile code1 - ```gcc -o code1 code1.c ```
2. run code1 and save in result.txt file - ```./code1 > result.txt ```
3. enter tcl script file to be run on.
4. compile code2 - ```gcc -o code2 code2.c ```
5. run code2 - ```./code2 ```
6. enter 2 variables present in the script to check for type equivalence 

Example:
```
>>> gcc -o code1 code1.c
>>> ./code1 > result.txt
t1.tcl
>>> gcc -o code2 code2.c
>>> ./code2
```

### Break down into end to end tests

Code1 identifies all the variables and procedures available in TCL script file inputted. Checks the type. It tests the name, internal name and structural equivalence for each pair of variables and procedures.

Example of TCL script file:
```
set x a
set y 10
```
Output:
```
Table for Structural equivalence:
	x y
x	Y Y
y	  Y

Name Equivalence:
x, y are name equivalent.

Internal Name Equivalent:
1. x, y are internal equivalent.
```
Code2 checks if the pair of variables inputted are equivalent in the TCL script given to based on the rules inputted.

```
x y
```

Output:
```
False as x, y are not internal equivalent.
```

## Versioning

Version of this project is 1.o. 

## Authors

Contributors for this project are:
* Roshini Shetty
* Srilekha Talluri
* Sreeja Gurijala
* Aditi Sharma

## License

This project is licensed under the MIT License 