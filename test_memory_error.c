#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    printf("Enter the size of the array: ");
    scanf("%d", &n); // Get the size of the array from the user

    // Allocate memory for the array
    int *array = malloc(n * sizeof(int));
    if (!array) {
        printf("Memory allocation failed.\n");
        return 1; // Exit if memory allocation fails
    }

    // Initialize the array with some values (e.g., 0 to n-1)
    for (int i = 0; i < n; i++) {
        array[i] = i;
    }

    // Ask the user for an index to access
    int index;
    printf("Enter the index to access (0 to %d): ", n - 1);
    scanf("%d", &index);

    // Access the array
    if (index >= 0 && index < n) {
        printf("Value at array[%d]: %d\n", index, array[index]);
    } else {
        printf("Index out of bounds! Simulating a segmentation fault...\n");
        // Simulate a segmentation fault by accessing invalid memory
        int *invalid_ptr = NULL;  // NULL pointer
        *invalid_ptr = 42;        // Dereference the NULL pointer (causes SIGSEGV)
    }

    // Free the allocated memory
    free(array);

    return 0;
}
