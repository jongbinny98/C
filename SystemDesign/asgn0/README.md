#asgn0
#Jongbin Baek - 1745430

---------------------------------------------------------------------------------------------------------------------------------------------
split.c: 
This .c file will take a list of files as input and a delimiter. Also, it will go through the files and check with delimiter to replace to \n. 
1. needed to make sure the last arguement must be single-character(delimiter)
2. needed to understand read(), write() to read and write in the right place.

3. needed for loop to go through each argument by using *argv[].
4. needed while loop to check if there is "-" then the program need to read from stdin(0) and write to the stdout(1).
5. needed another while loop to check if there is no "-" then the program need to read from the file descriptor and write to the stdout(1).
6. depends on the buffer_size(I set to 512), need to think that file is larger than the buffer_size.
    --> I solved this use while loop to read again if my program was not able to read all. My program will read until the size is equal to 0.
7. To run:
./split filename1.txt filename2.txt delimiter or ./split filename1.txt filename2.txt - delimiter

---------------------------------------------------------------------------------------------------------------------------------------------
Makefile: 
1. make: run "make" in the command line to get split(executable file) and split.o(object file)
2. clean: run "make clean" in the command line to remove what the command "make" created.
---------------------------------------------------------------------------------------------------------------------------------------------
test_files
1. toSuccess.txt: this .txt file contains the simple Syllabus for cse130. just to compare general length .txt file with some special character.
2. happy.txt: this .txt file contains a lot of special character and to check if it works as same as what is given.
3. newline.txt: .txt file with all the newline and some character.
4. dog.c: example file to test if the program works on other type file.

---------------------------------------------------------------------------------------------------------------------------------------------

test_scripts: 
My program passed all the test_script. There are some extra test_scripts to test. if it prints 0 in stdout, it passes. if not, it is not passed.
1. happy-test.sh: to test special character as delimiter
2. dash-test.sh: to test "-". if 0, then it passed. After that, it needs to exit by CTRL+d
3. delimiter-test.sh: to test if the delimiter is not exist in the file.txt(happy.txt in this case)
4. newline-test.sh: to test if the newline "\n" inside the file is going to be recongnized or not 
5. cfile-test.sh: to test if the program works with .c file



