#include "rtree.h"
#include "priority_queue.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Initializes a new R-tree node
RTreeNode* init_node(bool is_leaf);

// Initializes a new R-tree
RTree* init_tree();

// Adds an entry to a node
void add_entry(RTreeNode *node, Entry *entry);

// Computes the bounding box for a set of rectangles
Rect bounding_box(Rect *rects, int count);

// Computes the enlargement needed to include a new rectangle
float enlargement(Rect *r1, Rect *r2);

// Chooses the appropriate leaf node for insertion
RTreeNode* choose_leaf(RTreeNode *node, Entry *entry);

// Splits a node into two nodes
RTreeNode* split_node(RTree *tree, RTreeNode *node);

// Adjusts the tree after insertion
void adjust_tree(RTree *tree, RTreeNode *node);

// Checks if two rectangles overlap
bool overlap(Rect *r1, Rect *r2);

// Searches the tree for entries that overlap with a given rectangle
void search(RTreeNode *node, Rect *rect, void (*callback)(Entry *));

// Finds the leaf node containing a specific entry
RTreeNode* find_leaf(RTreeNode *node, Entry *entry);

// Inserts an entry into the tree
void insert(RTree *tree, Entry *entry);

// Condenses the tree after deletion
void condense_tree(RTree *tree, RTreeNode *node);

// Deletes an entry from the tree
void delete_entry(RTree *tree, Entry *entry);

// Computes the minimum distance from a point to a rectangle
float min_distance(Rect *rect, float point[2]);

// Finds the nearest neighbor to a given point
Entry* nearest_neighbor(RTree *tree, float point[2]);

// Saves a node to a file
void save_node(FILE *file, RTreeNode *node);

// Saves the tree to a file
void save_tree(RTree *tree, const char *filename);

// Loads a node from a file
RTreeNode* load_node(FILE *file);

// Loads the tree from a file
RTree* load_tree(const char *filename);

// Initializes a new R-tree node
// is_leaf: boolean indicating if the node is a leaf
// Returns a pointer to the newly created R-tree node
RTreeNode* init_node(bool is_leaf) {
    // Allocate memory for a new R-tree node
    RTreeNode *node = (RTreeNode *)malloc(sizeof(RTreeNode));
    // Set the is_leaf property of the node
    node->is_leaf = is_leaf;
    // Initialize the number of entries in the node to 0
    node->num_entries = 0;
    // Set the parent of the node to NULL
    node->parent = NULL;
    // Return the newly created node
    return node;
}

// Initializes a new R-tree
// Returns a pointer to the newly created R-tree
RTree* init_tree() {
    // Allocate memory for a new R-tree
    RTree *tree = (RTree *)malloc(sizeof(RTree));
    // Initialize the root of the tree as a leaf node
    tree->root = init_node(true);
    // Set the maximum number of entries in a node
    tree->max_entries = MAX_ENTRIES;
    // Set the minimum number of entries in a node
    tree->min_entries = MIN_ENTRIES;
    // Return the newly created tree
    return tree;
}

// Adds an entry to a node
// node: pointer to the R-tree node
// entry: pointer to the entry to be added
void add_entry(RTreeNode *node, Entry *entry) {
    // Add the entry to the node's entries array
    node->entries[node->num_entries++] = entry;
    // If the entry has a child node, set its parent to the current node
    if (entry->child != NULL) {
        entry->child->parent = node;
    }
}

// Computes the bounding box for a set of rectangles
// rects: array of rectangles
// count: number of rectangles in the array
// Returns the bounding box that contains all the rectangles
Rect bounding_box(Rect *rects, int count) {
    // Initialize the bounding box with extreme values
    Rect bbox = {{FLT_MAX, FLT_MAX}, {-FLT_MAX, -FLT_MAX}};
    // Iterate over each rectangle
    for (int i = 0; i < count; i++) {
        // Update the bounding box to include the current rectangle
        for (int j = 0; j < 2; j++) {
            if (rects[i].min[j] < bbox.min[j]) bbox.min[j] = rects[i].min[j];
            if (rects[i].max[j] > bbox.max[j]) bbox.max[j] = rects[i].max[j];
        }
    }
    // Return the computed bounding box
    return bbox;
}

// Computes the enlargement needed to include a new rectangle
// r1: pointer to the first rectangle
// r2: pointer to the second rectangle
// Returns the enlargement area needed to include both rectangles
float enlargement(Rect *r1, Rect *r2) {
    // Compute the bounding box that includes both rectangles
    Rect bbox = bounding_box((Rect[]){*r1, *r2}, 2);
    // Compute the area of the first rectangle
    float area1 = (r1->max[0] - r1->min[0]) * (r1->max[1] - r1->min[1]);
    // Compute the area of the bounding box
    float area2 = (bbox.max[0] - bbox.min[0]) * (bbox.max[1] - bbox.min[1]);
    // Return the difference between the bounding box area and the first rectangle area
    return area2 - area1;
}

// Chooses the appropriate leaf node for insertion
// node: pointer to the current R-tree node
// entry: pointer to the entry to be inserted
// Returns the leaf node where the entry should be inserted
RTreeNode* choose_leaf(RTreeNode *node, Entry *entry) {
    // If the current node is a leaf, return it
    if (node->is_leaf) return node;
    // Initialize the minimum enlargement to a large value
    float min_enlargement = FLT_MAX;
    // Initialize the best choice node to NULL
    RTreeNode *best_choice = NULL;
    // Iterate over each entry in the current node
    for (int i = 0; i < node->num_entries; i++) {
        // Compute the enlargement needed to include the entry's rectangle
        float e = enlargement(&node->entries[i]->rect, &entry->rect);
        // If the enlargement is smaller than the current minimum, update the best choice
        if (e < min_enlargement) {
            min_enlargement = e;
            best_choice = node->entries[i]->child;
        }
    }
    // Recursively choose the leaf node in the best choice subtree
    return choose_leaf(best_choice, entry);
}

// Splits a node into two nodes
// tree: pointer to the R-tree
// node: pointer to the node to be split
// Returns the new sibling node created by the split
RTreeNode* split_node(RTree *tree, RTreeNode *node) {
    // Compute the midpoint of the node's entries
    int mid = node->num_entries / 2;
    // Initialize a new sibling node with the same leaf status as the current node
    RTreeNode *sibling = init_node(node->is_leaf);
    // Move the second half of the entries to the sibling node
    for (int i = mid; i < node->num_entries; i++) {
        add_entry(sibling, node->entries[i]);
    }
    // Update the number of entries in the current node
    node->num_entries = mid;
    // Return the new sibling node
    return sibling;
}

// Adjusts the tree after insertion
// tree: pointer to the R-tree
// node: pointer to the node that was just inserted into
void adjust_tree(RTree *tree, RTreeNode *node) {
    // If the node is the root
    if (node == tree->root) {
        // If the root has more entries than allowed
        if (node->num_entries > tree->max_entries) {
            // Create a new root node
            RTreeNode *new_root = init_node(false);
            // Split the current root node
            RTreeNode *sibling = split_node(tree, node);
            // Create entries for the new root
            Entry *entry1 = (Entry *)malloc(sizeof(Entry));
            entry1->rect = bounding_box((Rect[]){node->entries[0]->rect}, node->num_entries);
            entry1->child = node;
            Entry *entry2 = (Entry *)malloc(sizeof(Entry));
            entry2->rect = bounding_box((Rect[]){sibling->entries[0]->rect}, sibling->num_entries);
            entry2->child = sibling;
            // Add the entries to the new root
            add_entry(new_root, entry1);
            add_entry(new_root, entry2);
            // Update the tree's root
            tree->root = new_root;
        }
    } else {
        // If the node is not the root
        RTreeNode *parent = node->parent;
        // If the node has more entries than allowed
        if (node->num_entries > tree->max_entries) {
            // Split the node
            RTreeNode *sibling = split_node(tree, node);
            // Create an entry for the parent
            Entry *entry = (Entry *)malloc(sizeof(Entry));
            entry->rect = bounding_box((Rect[]){sibling->entries[0]->rect}, sibling->num_entries);
            entry->child = sibling;
            // Add the entry to the parent
            add_entry(parent, entry);
            // Recursively adjust the tree
            adjust_tree(tree, parent);
        }
    }
}

// Checks if two rectangles overlap
// r1: pointer to the first rectangle
// r2: pointer to the second rectangle
// Returns true if the rectangles overlap, false otherwise
bool overlap(Rect *r1, Rect *r2) {
    // Check if the rectangles do not overlap on the x-axis or y-axis
    return !(r1->max[0] < r2->min[0] || r1->min[0] > r2->max[0] || r1->max[1] < r2->min[1] || r1->min[1] > r2->max[1]);
}

// Searches the tree for entries that overlap with a given rectangle
// node: pointer to the current R-tree node
// rect: pointer to the rectangle to search for
// callback: function to call for each overlapping entry
void search(RTreeNode *node, Rect *rect, void (*callback)(Entry *)) {
    // Iterate over each entry in the node
    for (int i = 0; i < node->num_entries; i++) {
        // If the entry's rectangle overlaps with the search rectangle
        if (overlap(&node->entries[i]->rect, rect)) {
            // If the node is a leaf, call the callback function with the entry
            if (node->is_leaf) {
                callback(node->entries[i]);
            } else {
                // If the node is not a leaf, recursively search the child node
                search(node->entries[i]->child, rect, callback);
            }
        }
    }
}

// Finds the leaf node containing a specific entry
// node: pointer to the current R-tree node
// entry: pointer to the entry to search for
// Returns the leaf node containing the entry, or NULL if not found
RTreeNode* find_leaf(RTreeNode *node, Entry *entry) {
    // If the node is a leaf
    if (node->is_leaf) {
        // Iterate over each entry in the node
        for (int i = 0; i < node->num_entries; i++) {
            // If the entry is found, return the node
            if (node->entries[i] == entry) {
                return node;
            }
        }
        // If the entry is not found, return NULL
        return NULL;
    }
    // If the node is not a leaf, iterate over each entry
    for (int i = 0; i < node->num_entries; i++) {
        // If the entry's rectangle overlaps with the search entry's rectangle
        if (overlap(&node->entries[i]->rect, &entry->rect)) {
            // Recursively search the child node
            RTreeNode *result = find_leaf(node->entries[i]->child, entry);
            // If the entry is found in the child node, return the result
            if (result) return result;
        }
    }
    // If the entry is not found, return NULL
    return NULL;
}

// Inserts an entry into the tree
// tree: pointer to the R-tree
// entry: pointer to the entry to be inserted
void insert(RTree *tree, Entry *entry) {
    // Choose the appropriate leaf node for insertion
    RTreeNode *leaf = choose_leaf(tree->root, entry);
    // Add the entry to the leaf node
    add_entry(leaf, entry);
    // If the leaf node has more entries than allowed, adjust the tree
    if (leaf->num_entries > tree->max_entries) {
        adjust_tree(tree, leaf);
    }
}

// Condenses the tree after deletion
// tree: pointer to the R-tree
// node: pointer to the node that was just deleted from
void condense_tree(RTree *tree, RTreeNode *node) {
    // If the node is the root
    if (node == tree->root) {
        // If the root has only one entry and is not a leaf
        if (node->num_entries == 1 && !node->is_leaf) {
            // Update the root to be the single child node
            tree->root = node->entries[0]->child;
            tree->root->parent = NULL;
        }
        return;
    }
    // If the node is not the root
    RTreeNode *parent = node->parent;
    // If the node has fewer entries than allowed
    if (node->num_entries < tree->min_entries) {
        // Remove the node from its parent's entries
        for (int i = 0; i < parent->num_entries; i++) {
            if (parent->entries[i]->child == node) {
                for (int j = i; j < parent->num_entries - 1; j++) {
                    parent->entries[j] = parent->entries[j + 1];
                }
                parent->num_entries--;
                break;
            }
        }
        // Reinsert the node's entries into the tree
        for (int i = 0; i < node->num_entries; i++) {
            insert(tree, node->entries[i]);
        }
    }
    // Recursively condense the tree
    condense_tree(tree, parent);
}


// Computes the minimum distance from a point to a rectangle
// rect: pointer to the rectangle
// point: array representing the point (x, y)
// Returns the minimum distance from the point to the rectangle
float min_distance(Rect *rect, float point[2]) {
    // Compute the distance on the x-axis
    float dx = fmaxf(fmaxf(rect->min[0] - point[0], 0.0f), point[0] - rect->max[0]);
    // Compute the distance on the y-axis
    float dy = fmaxf(fmaxf(rect->min[1] - point[1], 0.0f), point[1] - rect->max[1]);
    // Return the Euclidean distance
    return sqrtf(dx * dx + dy * dy);
}

// Finds the nearest neighbor to a given point
// tree: pointer to the R-tree
// point: array representing the point (x, y)
// Returns the nearest neighbor entry to the point
Entry* nearest_neighbor(RTree *tree, float point[2]) {
    // Create a priority queue for the search
    PriorityQueue *pq = create_priority_queue(10);
    // Push the root node into the priority queue with distance 0
    priority_queue_push(pq, tree->root, 0.0f);
    // Initialize the nearest neighbor and its distance
    Entry *nearest = NULL;
    float nearest_distance = FLT_MAX;

    // While there are nodes in the priority queue
    while (pq->size > 0) {
        // Pop the node with the smallest distance
        PriorityQueueNode pq_node = priority_queue_pop(pq);

        // If the distance is greater than or equal to the nearest distance, skip it
        if (pq_node.distance >= nearest_distance) continue;

        // Get the current node
        RTreeNode *node = pq_node.node;
        // Iterate over each entry in the node
        for (int i = 0; i < node->num_entries; i++) {
            // Compute the distance from the point to the entry's rectangle
            float distance = min_distance(&node->entries[i]->rect, point);
            // If the distance is smaller than the nearest distance
            if (distance < nearest_distance) {
                // If the node is a leaf, update the nearest neighbor and distance
                if (node->is_leaf) {
                    nearest = node->entries[i];
                    nearest_distance = distance;
                } else {
                    // If the node is not a leaf, push the child node into the priority queue
                    priority_queue_push(pq, node->entries[i]->child, distance);
                }
            }
        }
    }

    // Free the priority queue
    free(pq->nodes);
    free(pq);

    // Return the nearest neighbor
    return nearest;
}

// Saves a node to a file
// file: pointer to the file
// node: pointer to the R-tree node to be saved
void save_node(FILE *file, RTreeNode *node) {
    // Write the is_leaf property to the file
    fwrite(&node->is_leaf, sizeof(bool), 1, file);
    // Write the number of entries to the file
    fwrite(&node->num_entries, sizeof(int), 1, file);
    // Iterate over each entry in the node
    for (int i = 0; i < node->num_entries; i++) {
        // Write the entry's rectangle to the file
        fwrite(&node->entries[i]->rect, sizeof(Rect), 1, file);
        // If the node is not a leaf, recursively save the child node
        if (!node->is_leaf) {
            save_node(file, node->entries[i]->child);
        }
    }
}

// Saves the tree to a file
// tree: pointer to the R-tree
// filename: name of the file to save the tree to
void save_tree(RTree *tree, const char *filename) {
    // Open the file for writing in binary mode
    FILE *file = fopen(filename, "wb");
    // If the file is successfully opened
    if (file) {
        // Save the root node to the file
        save_node(file, tree->root);
        // Close the file
        fclose(file);
        // Print a success message
        printf("Tree saved successfully to %s\n", filename);
    } else {
        // Print an error message if the file could not be opened
        printf("Failed to open file %s for writing\n", filename);
    }
}

// Loads a node from a file
// file: pointer to the file to read from
// Returns a pointer to the loaded R-tree node
RTreeNode* load_node(FILE *file) {
    // Declare a variable to store whether the node is a leaf
    bool is_leaf;
    // Read the is_leaf property from the file
    fread(&is_leaf, sizeof(bool), 1, file);
    
    // Initialize a new node with the is_leaf property
    RTreeNode *node = init_node(is_leaf);
    
    // Read the number of entries in the node from the file
    fread(&node->num_entries, sizeof(int), 1, file);
    
    // Iterate over the number of entries
    for (int i = 0; i < node->num_entries; i++) {
        // Allocate memory for a new entry
        node->entries[i] = (Entry *)malloc(sizeof(Entry));
        
        // Read the rectangle of the entry from the file
        fread(&node->entries[i]->rect, sizeof(Rect), 1, file);
        
        // If the node is not a leaf, recursively load the child node
        if (!is_leaf) {
            node->entries[i]->child = load_node(file);
        }
    }
    
    // Return the loaded node
    return node;
}

// Loads the tree from a file
// filename: name of the file to read from
// Returns a pointer to the loaded R-tree
RTree* load_tree(const char *filename) {
    // Open the file in binary read mode
    FILE *file = fopen(filename, "rb");
    
    // Check if the file was successfully opened
    if (!file) {
        // Print an error message if the file could not be opened
        printf("Failed to open file %s for reading\n", filename);
        // Return NULL to indicate failure
        return NULL;
    }
    
    // Allocate memory for a new R-tree
    RTree *tree = (RTree *)malloc(sizeof(RTree));
    
    // Load the root node of the tree from the file
    tree->root = load_node(file);
    
    // Close the file after reading
    fclose(file);
    
    // Print a success message
    printf("Tree loaded successfully from %s\n", filename);
    
    // Return the loaded tree
    return tree;
}