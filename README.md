
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

 
![IMAGE ALT TEXT HERE](https://user-images.githubusercontent.com/62290677/165375827-de78d043-8283-4028-9bac-c79f73192469.png)](https://www.youtube.com/watch?v=uKM5gwQ0zEE)

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
 ![image](https://user-images.githubusercontent.com/62290677/165369732-ab2a21db-b3b7-4a93-9ca0-4d430fc942f3.png)

 

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
 
 ![image](https://user-images.githubusercontent.com/62290677/165369565-c343dc24-b4be-4688-8811-62f95d9dc1cd.png)


you will see that the size of the stack will be 4000 
if you comment out the lines that use stack_mutex then you will get a smaller number as an affect of not handling correncty about multi-thread

because our architecure with the queue doesn't allow us to turn down the server
the only way to terminate the server is using ctrl+c

so in order to test our malloc implementation we used a diffrent repository

https://github.com/dolev146/malloc_free_implementation

open it and follow the instruction , we check that no bytes are lost with valgrind
 
 
 ![image](https://user-images.githubusercontent.com/62290677/165367469-bcc1290f-8205-4904-bc53-0431cdbc6162.png)
 



# JACOB-SORBER-multithreaded-server-all-parts

https://github.com/dolev146/JACOB-SORBER-multithreaded-server-all-parts

# part 1

* https://www.youtube.com/watch?v=Pg_4Jz8ZIH4&list=RDCMUCwd5VFu4KoJNjkWJZMFJGHQ&start_radio=1&t=862s&ab_channel=JacobSorber

# part 2

* https://www.youtube.com/watch?v=FMNnusHqjpw&t=80s&ab_channel=JacobSorber

# part 3

* https://www.youtube.com/watch?v=P6Z5K8zmEmc&t=17s&ab_channel=JacobSorber


# malloc implemnetation

https://github.com/chaitanyamehta/custom-malloc.git
