
## Authors

- [@Dolev Dublon, ID: 207867342](https://www.github.com/dolev146)
- [@Yakov Khodorkovsky, ID: 207045063 ](https://www.github.com/yakov103)

 
# Multi-thread server in c with sbrk(2) instead of malloc for dynamic memeory with blocking stack

* The server can handle multiple connections at once by using a queue to store all the socket's connecting to the server,
we are using a thread pool that always check if the queue is empty if it doesn't we take out the connection and handle it's request, untill it finishing and then we follow up to the next socket.

* we also had to implement a stack that support: 
1) PUSH <txt>
2) POP
3) TOP

* locking mechanisim so that no 2 threads can access the memory at once.

* we also had to implement malloc and free with sbrk(2)


## Run Locally

Clone the project

```bash
  git clone https://github.com/dolev146/ex4_os.git
```

Go to the project directory

```bash
  cd ex4_os
```

Install dependencies

```bash
  sudo apt install ruby
  sudo apt install make
  sudo apt install g++ 
```

Start the server

```bash
  make; ./server
```

and in a diffrent terminal

```bash
  ./client
```



## Running Tests

The first test will test handling multiple clients

first  run the server

```bash
  make clean;make; ./server
```

and in a diffrent terminal run

```bash
  bash ./manyclients.bash
```

this is testing makeing 150! connections that sending messages and reciving mesesges from the server


press ctrl+c to terminate the server terminal


The second test will be testing the stack implementation for blocking


```bash
  make clean;make; ./server
```

and in a diffrent terminal run

```bash
  ./clienttest
```

you will see that the size of the stack will be 4000 
if you comment out the lines that use stack_mutex then you will get a smaller number as an affect of not handling correncty about multi-thread

because our architecure with the queue doesn't allow us to turn down the server
the only way to terminate the server is using ctrl+c

so in order to test our malloc implementation we used a diffrent repository

https://github.com/dolev146/malloc_free_implementation

open it and follow the instruction , we check that no bytes are lost with valgrind



