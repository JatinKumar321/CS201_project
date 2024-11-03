#include "priority_queue.h"

// Function declarations
void swap(PriorityQueueNode *a, PriorityQueueNode *b);
void heapify_up(PriorityQueue *pq, int index);
void heapify_down(PriorityQueue *pq, int index);
PriorityQueue* create_priority_queue(int capacity);
void priority_queue_push(PriorityQueue *pq, RTreeNode *node, float distance);
PriorityQueueNode priority_queue_pop(PriorityQueue *pq);

// Function definitions

// Swap two nodes in the priority queue
void swap(PriorityQueueNode *a, PriorityQueueNode *b) {
    // Temporary storage for swapping
    PriorityQueueNode temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify up to maintain the heap property
void heapify_up(PriorityQueue *pq, int index) {
    // While the current node is not the root and its distance is less than its parent's distance
    while (index > 0 && pq->nodes[index].distance < pq->nodes[(index - 1) / 2].distance) {
        // Swap the current node with its parent
        swap(&pq->nodes[index], &pq->nodes[(index - 1) / 2]);
        // Move to the parent's index
        index = (index - 1) / 2;
    }
}

// Heapify down to maintain the heap property
void heapify_down(PriorityQueue *pq, int index) {
    // Initialize the smallest index as the current index
    int smallest = index;
    // Calculate the left and right child indices
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // If the left child exists and its distance is less than the smallest distance
    if (left < pq->size && pq->nodes[left].distance < pq->nodes[smallest].distance) {
        // Update the smallest index to the left child
        smallest = left;
    }
    // If the right child exists and its distance is less than the smallest distance
    if (right < pq->size && pq->nodes[right].distance < pq->nodes[smallest].distance) {
        // Update the smallest index to the right child
        smallest = right;
    }
    // If the smallest index is not the current index
    if (smallest != index) {
        // Swap the current node with the smallest node
        swap(&pq->nodes[index], &pq->nodes[smallest]);
        // Recursively heapify down the smallest node
        heapify_down(pq, smallest);
    }
}

// Create a priority queue with a given capacity
PriorityQueue* create_priority_queue(int capacity) {
    // Allocate memory for the priority queue
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    // Allocate memory for the nodes in the priority queue
    pq->nodes = (PriorityQueueNode *)malloc(sizeof(PriorityQueueNode) * capacity);
    // Set the capacity of the priority queue
    pq->capacity = capacity;
    // Initialize the size of the priority queue to 0
    pq->size = 0;
    // Return the created priority queue
    return pq;
}

// Push a node with a given distance into the priority queue
void priority_queue_push(PriorityQueue *pq, RTreeNode *node, float distance) {
    // If the priority queue is full
    if (pq->size == pq->capacity) {
        // Double the capacity of the priority queue
        pq->capacity *= 2;
        // Reallocate memory for the nodes in the priority queue
        pq->nodes = (PriorityQueueNode *)realloc(pq->nodes, sizeof(PriorityQueueNode) * pq->capacity);
    }
    // Set the node and distance for the new entry
    pq->nodes[pq->size].node = node;
    pq->nodes[pq->size].distance = distance;
    // Heapify up to maintain the heap property
    heapify_up(pq, pq->size);
    // Increment the size of the priority queue
    pq->size++;
}

// Pop the node with the smallest distance from the priority queue
PriorityQueueNode priority_queue_pop(PriorityQueue *pq) {
    // Store the root node
    PriorityQueueNode root = pq->nodes[0];
    // Replace the root node with the last node
    pq->nodes[0] = pq->nodes[pq->size - 1];
    // Decrement the size of the priority queue
    pq->size--;
    // Heapify down to maintain the heap property
    heapify_down(pq, 0);
    // Return the root node
    return root;
}