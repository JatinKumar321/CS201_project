#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

// Forward declaration of RTreeNode
typedef struct RTreeNode RTreeNode;

// Structure for a priority queue node
typedef struct PriorityQueueNode {
    // Pointer to the R-tree node
    RTreeNode *node;
    // Distance value for the priority queue
    float distance;
} PriorityQueueNode;

// Structure for the priority queue
typedef struct PriorityQueue {
    // Array of priority queue nodes
    PriorityQueueNode *nodes;
    // Capacity of the priority queue
    int capacity;
    // Current size of the priority queue
    int size;
} PriorityQueue;

// Function declarations
PriorityQueue* create_priority_queue(int capacity);
void priority_queue_push(PriorityQueue *pq, RTreeNode *node, float distance);
PriorityQueueNode priority_queue_pop(PriorityQueue *pq);

#endif // PRIORITY_QUEUE_H