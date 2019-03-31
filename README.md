# Theory-of-Algorithims-Project

Garry Cummins - G00335806

## Project Outline 

This project uses the c programming language to calculate the SHA-256 checksum of a input using the specific alogorithim. The algorithm is specified in the SecureHash Standard document supplied by the (United States) National Institute
of Standards and Technology. The project was completed in a vi linux instance on google cloud.

## Compiling and running the program 
Firstly you must clone the repository using this command on your cmd:

git clone https://github.com/yrrag5/Theory-of-Algorithims-Project.git

Make sure your in the SHA directory folder which the c files are located. Compile SHA256 by running: 

### gcc -o SHA256 SHA256.c

followed by running it using : 

### ./SHA256 SHA256.c

A file will then need to be selected in order to test. file.txt can be used in this case located with the c files:

### ./SHA256 file.txt

This should output the checksum of the file.

### Output 

a8d443b5 3f2b7ee8 44a20e28 ac98e808 547b81b3 fe2843a be207811 3880a3cc



