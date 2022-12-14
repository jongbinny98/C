#asgn1
#Jongbin Baek - 1745430

1. Client: the client will first create the socket and then try to connect with the server by request. after the server accepts the client's request, the client and the server will start to send and receive the data with each other. when everything is done, it will close the socket
2. Server: The server is abit complicated to process. The server will bind the socket. After that, the server will listen if any request from the client. and when the request is received, it accepts the request from the client. once they are connected, the client can HTTP request such as PUT, GET, HEAD.
---------------------------------------------------------------------------------------------------------------------------------------------
httpserver.c: 
httpserver.c file is to create the server that can interact with client. 
1. startercode "bind.c" was procided and it is supposed to create the socket and bind and listen.
2. Changed string type portnumber to uint16_t format and passed in to the create_listen_socket()
3. Accepted server socket that is created as in fd(client_socket)
4. Created two function one function called "recv_process_request" and one function called "server_response"
5. In the function "recv_process_request", Took the request from the client and parsed the string by using sscanf()
6. In the function "server_response", it store the status_code for each situation and store the response into fd
7. printed out these response to the stdout.

method == GET || method == head 
1. GET method or HEAD method is to get the data from the server.
2. GET will take all the data while HEAD will just take the header.
3. Printout status_code when file is not there(404), no read permission(403).

method == PUT
1. PUT method is to write to the server.
2. PUT will create if the server doesn't have the file. and get response Created(201)
3. PUT will override the data if the file in the server exist.
4. Printout status_code when the file has no write permission(403)

1. Wrong method used --> not implemented(501)
2. Internal server error(500)
3. The requset is not formatted well --> Bad request(400) checked by error_code(file)

To run: 
1. GET: curl http://localhost:1234/foo.txt -o download.txt
2. HEAD: curl -I http://localhost:1234/foo.txt 
3. PUT: curl -T download.txt http://localhost:1234/foo.txt or
printf "PUT /foo.txt HTTP/1.1\r\nContent-Length: 12\r\n\r\nHello World?" |nc localhost 1234

---------------------------------------------------------------------------------------------------------------------------------------------
Makefile: 
1. make: run "make" in the command line to get httpserver(executable file) and httpserver.o(object file)
2. clean: run "make clean" in the command line to remove what the command "make" created.
---------------------------------------------------------------------------------------------------------------------------------------------
test_files
1. frankenstein.txt

---------------------------------------------------------------------------------------------------------------------------------------------
test_scripts: 

1. get_ascii_large.sh --> check if it works for large file than the ascii small.
2. 


