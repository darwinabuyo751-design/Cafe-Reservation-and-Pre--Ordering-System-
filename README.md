# Cafe-Reservation-and-Pre--Ordering-System-
This project is a console-based application written in C that allows users to reserve tables and place advance orders in a cafe setting.  The system ensures proper scheduling of reservations and processes customer orders efficiently using different data structures and algorithms.

Purpose
The goal of this project is to demonstrate the application of Data Structures and Algorithms (DSA) in solving real-world problems such as reservation management and order processing.

 Features
- Add table reservations with time slots
- Prevent double booking of tables
- Undo last reservation (Stack)
- View all reservations
- Place orders (only if reserved)
- Process orders in sequence (Queue)
- Search reservation by name
- Sort reservations by name or time

  Data Structures Used

1. Array
Used to store reservations and orders.
- reservations[MAX]
- orders[MAX]

2. Stack (LIFO)
Used for undo functionality.
- push(): adds reservation to stack
- pop(): removes last reservation

3. Queue (FIFO)
Used for order processing.
- enqueue(): adds order
- dequeue(): processes order

   Algorithms Used
1. Searching (Linear Search)
Used to find a reservation by customer name.
 2. Sorting (Bubble Sort)
- Sort by name (alphabetical)
- Sort by time (based on time slot index)

   How to Compile and Run
 Step 1: Compile the program
gcc main.c -o cafe

Step 2: Run the program
./cafe

 Sample Usage Flow

1. Add a reservation
2. View reservations
3. Place an order (only if reserved)
4. Process order
5. Search or sort reservations
6. Undo reservation if needed

 Input Validation and Edge Cases

The system handles the following:
- Invalid name input (letters only)
- Table number must be between 1–10
- Prevents duplicate table reservations for same time slot
- Prevents ordering without reservation
- Prevents empty orders
- Handles full reservation list
- Handles empty queue and undo operations

   👤 Author
Name: Abuyo, Darwin D.
      Felizardo, Raiza I.
      Marmol, John Harvey
Course: BSIT
Subject: Data Structures and Algorithms
   
