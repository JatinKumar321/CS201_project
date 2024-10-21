// main.c
#include <stdio.h>
#include <math.h>
#include "rtree.h"

int main() {
    RTreeNode* root = create_node(0); // Create root node

    // Example hotels
    Hotel hotel1 = {"Hotel A", 37.7749, -122.4194};
    Hotel hotel2 = {"Hotel B", 34.0522, -118.2437};

    insert_hotel(root, hotel1);
    insert_hotel(root, hotel2);

    Hotel nearest = {"", 0, 0}; // Initialize nearest hotel
    find_nearest_hotel(root, 37.7749, -122.4194, &nearest); // Your location

    printf("Nearest Hotel: %s at (%lf, %lf)\n", nearest.name, nearest.latitude, nearest.longitude);

    // Free allocated memory here (not shown for brevity)

    return 0;
}