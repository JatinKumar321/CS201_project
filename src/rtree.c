// rtree.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "rtree.h"

// Create a new R-tree node
RTreeNode* create_node(int is_leaf) {
    RTreeNode* node = (RTreeNode*)malloc(sizeof(RTreeNode));
    node->is_leaf = is_leaf;
    node->num_children = 0;
    node->children = NULL;
    node->hotels = NULL;

    for (int i = 0; i < 2; i++) {
        node->min[i] = INFINITY;
        node->max[i] = -INFINITY;
    }

    return node;
}

// Insert a hotel into the R-tree
void insert_hotel(RTreeNode* node, Hotel hotel) {
    if (node->is_leaf) {
        node->hotels = realloc(node->hotels, (node->num_children + 1) * sizeof(Hotel));
        node->hotels[node->num_children] = hotel;
        node->num_children++;
        
        // Update bounding box
        if (hotel.longitude < node->min[0]) node->min[0] = hotel.longitude;
        if (hotel.latitude < node->min[1]) node->min[1] = hotel.latitude;
        if (hotel.longitude > node->max[0]) node->max[0] = hotel.longitude;
        if (hotel.latitude > node->max[1]) node->max[1] = hotel.latitude;
    } else {
        // For simplicity, always insert into the first child
        if (node->children == NULL) {
            node->children = malloc(sizeof(RTreeNode*));
            node->children[0] = create_node(1);
            node->num_children = 1;
        }
        insert_hotel(node->children[0], hotel);
    }
}

// Calculate distance between two geographical points
double distance(double lat1, double lon1, double lat2, double lon2) {
    return sqrt(pow(lat1 - lat2, 2) + pow(lon1 - lon2, 2));
}

// Find the nearest hotel to a given location
void find_nearest_hotel(RTreeNode* node, double latitude, double longitude, Hotel* nearest) {
    if (node->is_leaf) {
        for (int i = 0; i < node->num_children; i++) {
            double dist = distance(latitude, longitude, node->hotels[i].latitude, node->hotels[i].longitude);
            if (nearest->name[0] == '\0' || dist < distance(latitude, longitude, nearest->latitude, nearest->longitude)) {
                *nearest = node->hotels[i];
            }
        }
    } else {
        for (int i = 0; i < node->num_children; i++) {
            find_nearest_hotel(node->children[i], latitude, longitude, nearest);
        }
    }
}