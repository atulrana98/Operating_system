// #include "types.h"
// #include "user.h"

// int
// main(void)
// {
//     char buffer[8];  // Buffer
//     int x, y;

//     printf(1, "Enter first value: ");
//     read(0, buffer, sizeof(buffer));  // Assuming file descriptor 0 (stdin)

//     // Convert the input string to an integer
//     x = atoi(buffer);

//     // Prompt user for second input
//     printf(1, "Enter second value: ");
//     read(0, buffer, sizeof(buffer));

//     // Convert the second input string to an integer
//     y = atoi(buffer);

//     // Use the add system call
//     int sum = add(x, y);

//     // Display the sum
//     printf(1, "Result of adding %d and %d is: %d\n", x, y, sum);

//     exit();
// }

#include "types.h"
#include "user.h"

int atoi_custom(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; str[i] != '\0' && str[i] != '\n'; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            // Handle invalid characters in the input string
            printf(1, "Error: Invalid character in input.\n");
            exit();
        }
    }

    return sign * result;
}

int main(void) {
    char buffer[8];  // Buffer
    int x, y;

    printf(1, "Enter first value: ");
    read(0, buffer, sizeof(buffer));  // Assuming file descriptor 0 (stdin)

    // Convert the input string to an integer
    x = atoi_custom(buffer);

    // Prompt user for second input
    printf(1, "Enter second value: ");
    read(0, buffer, sizeof(buffer));

    // Convert the second input string to an integer
    y = atoi_custom(buffer);

    // Use the add system call
    int sum = add(x, y);

    // Display the sum
    printf(1, "Result of adding %d and %d is: %d\n", x, y, sum);

    exit();
}



