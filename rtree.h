#ifndef RTREE_H
#define RTREE_H

// Include standard boolean library
#include <stdbool.h>

// Define maximum number of entries in a node
#define MAX_ENTRIES 4
// Define minimum number of entries in a node
#define MIN_ENTRIES 2

// Define a structure for a rectangle
typedef struct Rect {
    // Minimum coordinates of the rectangle
    float min[2];
    // Maximum coordinates of the rectangle
    float max[2];
} Rect;

// Define a structure for an entry in the R-tree
typedef struct Entry {
    // Rectangle associated with the entry
    Rect rect;
    // Union to store either a child node or data
    union {
        struct RTreeNode *child;
        void *data;
    };
} Entry;

// Define a structure for a node in the R-tree
typedef struct RTreeNode {
    // Boolean to indicate if the node is a leaf
    bool is_leaf;
    // Number of entries in the node
    int num_entries;
    // Array of entries in the node
    Entry *entries[MAX_ENTRIES + 1];
    // Pointer to the parent node
    struct RTreeNode *parent;
} RTreeNode;

// Define a structure for the R-tree
typedef struct RTree {
    // Pointer to the root node of the tree
    RTreeNode *root;
    // Maximum number of entries in a node
    int max_entries;
    // Minimum number of entries in a node
    int min_entries;
} RTree;

// Function declarations
RTree* init_tree();
void insert(RTree *tree, Entry *entry);
Entry* nearest_neighbor(RTree *tree, float point[2]);
void save_tree(RTree *tree, const char *filename);
RTree* load_tree(const char *filename);

#endif // RTREE_H