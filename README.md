# Multi-threaded Restaurant Simulation

## Overview
This project simulates a restaurant environment using multi-threaded programming in C. It employs **POSIX threads (pthreads)** and **semaphores** to model the interaction between customers, chefs, and waiters.

## Features
- **Multi-threading** using `pthread` for concurrent execution.
- **Synchronization** with semaphores to manage resource access.
- **Simulated real-world scenario** of order placement, preparation, and serving.
- **Graceful thread termination** after serving a predefined number of orders.

## Prerequisites
Ensure you have a C compiler installed that supports POSIX threads, such as `gcc`.

## Compilation and Execution
### Compile the Code
```sh
gcc -o main main.c -pthread
```

### Run the Program
```sh
./main
```

## Code Explanation
### 1. **Thread Management**
- The program creates three main types of threads:
  - **Customers**: Randomly place orders at the restaurant.
  - **Chef**: Prepares orders sequentially.
  - **Waiter**: Serves completed orders.

### 2. **Order Flow**
- Customers place orders, which are queued if the kitchen is full.
- The chef prepares orders in the sequence they arrive.
- The waiter serves completed orders to customers.

### 3. **Synchronization Mechanisms**
- **Semaphores** control access to shared resources such as the kitchen and serving queue.
- **Mutex locks** ensure that no two threads modify shared variables at the same time.

### 4. **Graceful Termination**
- The simulation runs until a predefined number of total orders is served (`MAX_TOTAL_ORDERS`).
- After reaching the limit, all threads are safely terminated.

## Sample Output
```
Customer placed an order. Orders in kitchen: 3
Chef prepared an order. Orders in kitchen: 2
Waiter served an order. Orders in queue: 1
```

## Possible Enhancements
- Implement a **priority queue** for orders.
- Introduce **multiple chefs and waiters** for scalability.
- Use **condition variables** instead of busy waiting.

## Author
**Mahmoud Hany** - Computer Engineering Student & AI Enthusiast

