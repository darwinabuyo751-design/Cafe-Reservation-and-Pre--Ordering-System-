#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50              // Maximum number of reservations/orders
#define MAX_NAME 30         // Maximum length of customer name
#define TIME_SLOTS 14       // Number of available time slots

// ================= TIME SLOTS =================
// Array of available reservation times
char *timeSlots[TIME_SLOTS] = {
    "7:30-8:30 AM","8:30-9:30 AM","9:30-10:30 AM","10:30-11:30 AM",
    "11:30-12:30 PM","12:30-1:30 PM","1:30-2:30 PM","2:30-3:30 PM",
    "3:30-4:30 PM","4:30-5:30 PM","5:30-6:30 PM","6:30-7:30 PM",
    "7:30-8:30 PM","8:30-9:30 PM"
};

// ================= STRUCT DEFINITIONS =================
// Structure for storing reservation details
typedef struct {
    char name[MAX_NAME];
    int table;
    int timeIndex;
} Reservation;

// Structure for storing order details
typedef struct {
    char name[MAX_NAME];
    char items[200];
    int total;
} Order;

// ================= GLOBAL VARIABLES =================
Reservation reservations[MAX];   // List of reservations
Order orders[MAX];               // Queue of orders

Reservation stack[MAX];          // Stack for undo feature
int top = -1;

int front = -1, rear = -1;       // Queue pointers
int reservationCount = 0;        // Current number of reservations

// ================= UTILITY FUNCTIONS =================
// Convert string to lowercase (used for case-insensitive comparison)
void toLowerCase(char *str) {
    for(int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

// Validate name (letters and spaces only)
int validName(char *name) {
    if(strlen(name) == 0) return 0;

    for(int i = 0; name[i]; i++) {
        if(!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }
    return 1;
}

// ================= CHECK SLOT =================
// Check if a table and time slot is already taken
int isSlotTaken(int table, int timeIndex) {
    for(int i = 0; i < reservationCount; i++) {
        if(reservations[i].table == table &&
           reservations[i].timeIndex == timeIndex) {
            return 1;
        }
    }
    return 0;
}

// ================= STACK (UNDO FEATURE) =================
// Push reservation into stack
void push(Reservation r) {
    if(top >= MAX-1) {
        printf("Undo stack is full!\n");
        return;
    }
    stack[++top] = r;
}

// Pop last reservation (undo)
void pop() {
    if(top >= 0 && reservationCount > 0) {
        Reservation removed = stack[top--];

        // Find and remove from reservations list
        for(int i = 0; i < reservationCount; i++) {
            if(strcmp(reservations[i].name, removed.name) == 0 &&
               reservations[i].table == removed.table &&
               reservations[i].timeIndex == removed.timeIndex) {

                // Shift elements left
                for(int j = i; j < reservationCount - 1; j++)
                    reservations[j] = reservations[j + 1];

                reservationCount--;
                printf("Undo: %s removed\n", removed.name);
                return;
            }
        }
        printf("Undo failed: reservation not found\n");
    } else {
        printf("Nothing to undo\n");
    }
}

// ================= QUEUE (ORDER PROCESSING) =================
// Add order to queue
void enqueue(Order o) {
    if(rear == MAX-1) {
        printf("Queue Full\n");
        return;
    }
    if(front == -1) front = 0;
    orders[++rear] = o;
}

// Process order from queue
void dequeue() {
    if(front == -1 || front > rear) {
        printf("No orders\n");
        return;
    }

    printf("\nProcessing Order...\n");
    printf("Customer: %s\n", orders[front].name);
    printf("Items: %s\n", orders[front].items);
    printf("Total: %d\n", orders[front].total);

    front++;

    // Reset queue if empty
    if(front > rear) {
        front = rear = -1;
    }
}

// ================= SEARCH =================
// Search reservation by name (case-insensitive)
void searchReservation() {
    char name[MAX_NAME], temp[MAX_NAME];

    printf("Enter name to search: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name,"\n")] = 0;

    strcpy(temp, name);
    toLowerCase(temp);

    for(int i = 0; i < reservationCount; i++) {
        char stored[MAX_NAME];
        strcpy(stored, reservations[i].name);
        toLowerCase(stored);

        if(strcmp(stored, temp) == 0) {
            printf("FOUND: %s | Table %d | %s\n",
                reservations[i].name,
                reservations[i].table,
                timeSlots[reservations[i].timeIndex]);
            return;
        }
    }
    printf("Not found\n");
}

// ================= SORTING =================
// Sort reservations alphabetically by name
void sortByName() {
    for(int i = 0; i < reservationCount - 1; i++) {
        for(int j = 0; j < reservationCount - i - 1; j++) {
            if(strcmp(reservations[j].name, reservations[j+1].name) > 0) {
                Reservation temp = reservations[j];
                reservations[j] = reservations[j+1];
                reservations[j+1] = temp;
            }
        }
    }
    printf("Sorted by name!\n");
}

// Sort reservations by time slot
void sortByTime() {
    for(int i = 0; i < reservationCount - 1; i++) {
        for(int j = 0; j < reservationCount - i - 1; j++) {
            if(reservations[j].timeIndex > reservations[j+1].timeIndex) {
                Reservation temp = reservations[j];
                reservations[j] = reservations[j+1];
                reservations[j+1] = temp;
            }
        }
    }
    printf("Sorted by time!\n");
}

// ================= MENU =================
// Display drink menu
void showMenu() {
    printf("\n===== MENU =====\n");
    printf("1. Iced Lemonade - 45\n");
    printf("2. Iced Americano - 50\n");
    printf("3. Iced Latte - 90\n");
    printf("4. Salted Caramel - 90\n");
    printf("5. Caramel Macchiato - 90\n");
    printf("0. Done\n");
}

// Return price based on item number
int price(int item) {
    switch(item) {
        case 1: return 45;
        case 2: return 50;
        case 3: return 90;
        case 4: return 90;
        case 5: return 90;
        default: return 0;
    }
}

// ================= TIME DISPLAY =================
// Show all available time slots
void showTimeSlots() {
    printf("\n===== TIME SLOTS =====\n");
    for(int i = 0; i < TIME_SLOTS; i++) {
        printf("%d. %s\n", i+1, timeSlots[i]);
    }
}

// ================= ADD RESERVATION =================
void addReservation() {

    // Prevent overflow
    if(reservationCount >= MAX) {
        printf("Reservation list is full!\n");
        return;
    }

    Reservation r;
    char buffer[10];

    // Input name
    printf("Enter name: ");
    fgets(r.name, sizeof(r.name), stdin);
    r.name[strcspn(r.name,"\n")] = 0;

    if(!validName(r.name)) {
        printf("Invalid name!\n");
        return;
    }

    // Input table number with validation
    char tableInput[10];
    int valid = 0;

    while(!valid) {
        printf("Table number (1-10): ");
        fgets(tableInput, sizeof(tableInput), stdin);
        tableInput[strcspn(tableInput, "\n")] = 0;

        int isNumber = 1;
        for(int i = 0; tableInput[i]; i++) {
            if(!isdigit(tableInput[i])) {
                isNumber = 0;
                break;
            }
        }

        if(!isNumber) {
            printf("INVALID CHOICE! Numbers only.\n");
            continue;
        }

        r.table = atoi(tableInput);

        if(r.table < 1 || r.table > 10)
            printf("INVALID CHOICE! Only 1-10 allowed.\n");
        else
            valid = 1;
    }

    // Select time slot
    showTimeSlots();
    printf("Enter NUMBER (1-14): ");

    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer,"\n")] = 0;

    int isNumber = 1;
    for(int i = 0; buffer[i]; i++) {
        if(!isdigit(buffer[i])) {
            isNumber = 0;
            break;
        }
    }

    if(!isNumber) {
        printf("Invalid input!\n");
        return;
    }

    int timeChoice = atoi(buffer);

    if(timeChoice < 1 || timeChoice > TIME_SLOTS) {
        printf("Invalid time!\n");
        return;
    }

    r.timeIndex = timeChoice - 1;

    // Check if slot is taken
    if(isSlotTaken(r.table, r.timeIndex)) {
        printf("Slot already taken!\n");
        return;
    }

    // Save reservation
    reservations[reservationCount++] = r;
    push(r);

    printf("Reservation added at %s\n", timeSlots[r.timeIndex]);
}

// ================= DISPLAY =================
// Show all reservations
void showReservations() {
    printf("\n===== RESERVATIONS =====\n");

    if(reservationCount == 0) {
        printf("No reservations\n");
        return;
    }

    for(int i = 0; i < reservationCount; i++) {
        printf("%d. %s | Table %d | %s\n",
            i+1,
            reservations[i].name,
            reservations[i].table,
            timeSlots[reservations[i].timeIndex]);
    }
}

// ================= PLACE ORDER =================
void placeOrder() {
    Order o;
    int item, total = 0;
    char itemsList[200] = "";

    printf("Customer name: ");
    fgets(o.name, sizeof(o.name), stdin);
    o.name[strcspn(o.name,"\n")] = 0;

    // Check if customer has reservation
    char tempInput[MAX_NAME], tempStored[MAX_NAME];
    int found = 0;

    strcpy(tempInput, o.name);
    toLowerCase(tempInput);

    for(int i = 0; i < reservationCount; i++) {
        strcpy(tempStored, reservations[i].name);
        toLowerCase(tempStored);

        if(strcmp(tempStored, tempInput) == 0) {
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("You must reserve first before placing an order!\n");
        return;
    }

    // Menu loop
    while(1) {
        showMenu();
        printf("Choice: ");
        scanf("%d", &item);

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if(item >= 1 && item <= 5) {
            total += price(item);

            switch(item) {
                case 1: strcat(itemsList, "Iced Lemonade, "); break;
                case 2: strcat(itemsList, "Iced Americano, "); break;
                case 3: strcat(itemsList, "Iced Latte, "); break;
                case 4: strcat(itemsList, "Salted Caramel, "); break;
                case 5: strcat(itemsList, "Caramel Macchiato, "); break;
            }
        } else if(item == 0) break;
        else printf("Invalid choice!\n");
    }

    // Prevent empty order
    if(total == 0) {
        printf("No items selected. Order cancelled.\n");
        return;
    }

    itemsList[strlen(itemsList)-2] = 0;

    strcpy(o.items, itemsList);
    o.total = total;

    enqueue(o);

    printf("Order placed! Total: %d\n", total);
}

// ================= MAIN =================
int main() {
    int choice, sortChoice;

    do {
        printf("\n=== Cafe Reservation & Pre-Ordering System ===\n");
        printf("1. Add Reservation\n");
        printf("2. Undo Reservation\n");
        printf("3. View Reservations\n");
        printf("4. Place Order\n");
        printf("5. Process Order\n");
        printf("6. Search Reservation\n");
        printf("7. Sort Reservations\n");
        printf("0. Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch(choice) {
            case 1: addReservation(); break;
            case 2: pop(); break;
            case 3: showReservations(); break;
            case 4: placeOrder(); break;
            case 5: dequeue(); break;
            case 6: searchReservation(); break;

            case 7:
                printf("1. Sort by Name\n2. Sort by Time\nChoice: ");
                scanf("%d", &sortChoice);
                while ((c = getchar()) != '\n' && c != EOF);

                if(sortChoice == 1) sortByName();
                else if(sortChoice == 2) sortByTime();
                else printf("Invalid choice\n");
                break;

            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while(choice != 0);

    return 0;
}