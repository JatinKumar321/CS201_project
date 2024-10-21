// rtree.h
#ifndef RTREE_H
#define RTREE_H

typedef struct {
    char name[100];
    double latitude;
    double longitude;
} Hotel;

typedef struct RTreeNode {
    double min[2]; // Min bounding box (longitude, latitude)
    double max[2]; // Max bounding box (longitude, latitude)
    struct RTreeNode** children; // Child nodes
    Hotel* hotels; // Hotels in this leaf node
    int num_children;
    int is_leaf;
} RTreeNode;

// Function prototypes
RTreeNode* create_node(int is_leaf);
void insert_hotel(RTreeNode* node, Hotel hotel);
void find_nearest_hotel(RTreeNode* node, double latitude, double longitude, Hotel* nearest);
double distance(double lat1, double lon1, double lat2, double lon2);

#endif // RTREE_H
