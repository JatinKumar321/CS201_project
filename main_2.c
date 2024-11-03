#include "rtree.h"
#include <stdio.h>
#include <stdlib.h>

void entry_callback(Entry *entry) {
    printf("Found entry: [%f, %f] - [%f, %f]\n",
           entry->rect.min[0], entry->rect.min[1],
           entry->rect.max[0], entry->rect.max[1]);
}

int main() {
    RTree *tree = init_tree();
    int choice;
    while (1) {
        printf("\n1. Insert Entry\n");
        
        printf("2. Search Nearest Neighbor\n");
        printf("3. Save Tree\n");
        printf("4. Load Tree\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Entry *entry = (Entry *)malloc(sizeof(Entry));
            printf("Enter rectangle min x, min y, max x, max y: ");
            scanf("%f %f %f %f", &entry->rect.min[0], &entry->rect.min[1], 
                                  &entry->rect.max[0], &entry->rect.max[1]);
            entry->child = NULL;  // Assuming no child for this example
            insert(tree, entry);
            printf("Entry inserted.\n");

        } else if (choice == 2) {
            float point[2];
            printf("Enter point (x y) to find nearest neighbor: ");
            scanf("%f %f", &point[0], &point[1]);
            Entry *nearest = nearest_neighbor(tree, point);
            if (nearest) {
                printf("Nearest neighbor found: [%f, %f] - [%f, %f]\n",
                       nearest->rect.min[0], nearest->rect.min[1],
                       nearest->rect.max[0], nearest->rect.max[1]);
            } else {
                printf("No neighbor found.\n");
            }

        } else if (choice == 3) {
            char filename[256];
            printf("Enter filename to save tree: ");
            scanf("%s", filename);
            save_tree(tree, filename);

        } else if (choice == 4) {
            char filename[256];
            printf("Enter filename to load tree: ");
            scanf("%s", filename);
            RTree *loaded_tree = load_tree(filename);
            if (loaded_tree) {
                free(tree); // Clean up the old tree
                tree = loaded_tree; // Switch to the loaded tree
            }

        } else if (choice == 5) {
            // Clean up
            free(tree);
            break;

        } else {
            printf("Invalid option, please try again.\n");
        }
    }
    return 0;
}