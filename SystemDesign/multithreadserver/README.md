Jongbin Baek
cse130
asgn3
-----------------------------------------------------------------------------
httpserver.c: This assignment is based on the asgn1. The purpose of this assignment is to make sure the asgn1 is working well to start the multithreading httpserver. Also, We need to implement the auit log, which will tell you simply whats going on with request-id. 
1. In this time, httpserver should take three comand line arguements like "./httpserver -t (threads_num) -l (logfilename) portnum".
2. I used getopt to parse the three command line arguments
3. I am going to write inside the logfile.
4. after that, I am supposed to think about using threads in order to support multiple clients at the same time. and will output audit log to the location specified by logfile.
5. Inside the logfile, the structure is supposed to be like "method, URL, statuscode, requestid" with right format
6. I haven't completed the multi threading part of the assignment which is connected to this assignment. However, it looks like I am supposed to use one dispatcher thread and worker threads. These threads needs to interact with each other to support multiple client at the same time. 
7. dispatcher thread is going to be the main thread that should wait for the connections from the clients.
8. each worker thread will perform HTTP processing from what we did on asgn1.
-------------------------------------------------------------------------------
httpserver.h: I just moved some of the function to make it easier to read. I have created the write_log function here to write things in logfile.
-------------------------------------------------------------------------------
There are no test_script and testfile was provided this time.