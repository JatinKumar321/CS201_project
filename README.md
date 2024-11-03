# R-tree implementaion and application in nearest neighbor finding

# file structure
1; priority_queue.h - header for priority queue
2; priority_queue.c - implementation of priority queue
3; r_tree.h - header for rtree
4; r_tree.c - implementation of rtree
5; main_2.c - contain main function with ui inteface

# Operations on R-tree
1; Initialize R-tree
2; Insert a point
Entry *entry = (Entry *)malloc(sizeof(Entry));
entry->rect.min[0] = ...;
entry->rect.min[1] = ...;
entry->rect.max[0] = ...;
entry->rect.max[1] = ...;
insert(tree, entry);

3; Search for nearest neighbor
float point[2] = {..., ...};
Entry *nearest = nearest_neighbor(tree, point);

4; save and load R-tree
save_tree(tree, "tree.txt");
tree = load_tree("tree.txt");

# How to run
<p>
gcc -o code.exe main_2.c rtree.c priority_queue.c -lm
./code.exe

The ui will guide you through the process of creating and searching for nearest neighbors in the R-tree.
example:
1; Insert a point
2; Search for nearest neighbor
3; Save tree
4; Load tree
5; Exit

> ./code.exe
     
1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 1
Enter rectangle min x, min y, max x, max y: 0 0 1 2
Entry inserted.

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 3
Enter filename to save tree: tree
Tree saved successfully to tree

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 4
Enter filename to load tree: tree
Tree loaded successfully from tree

1. Insert Entry
2. Search Nearest Neighbor
2. Search Nearest Neighbor
2. Search Nearest Neighbor
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 2
Enter point (x y) to find nearest neighbor: 0 0
4. Load Tree
5. Exit
Choose an option: 2
Enter point (x y) to find nearest neighbor: 0 0
Enter point (x y) to find nearest neighbor: 0 0
Nearest neighbor found: [0.000000, 0.000000] - [1.000000, 2.000000] 

1. Insert Entry
1. Insert Entry
2. Search Nearest Neighbor
2. Search Nearest Neighbor
3. Save Tree
3. Save Tree
4. Load Tree
5. Exit
4. Load Tree
5. Exit
5. Exit
Choose an option: En
PS E:\Course\CS201\project\final\final_code\CS201_project> .\code.exe

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 1
Enter rectangle min x, min y, max x, max y: 0 0 1 2
Entry inserted.

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 1
Enter rectangle min x, min y, max x, max y: 9 9 10 10 
Entry inserted.

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 2
Enter point (x y) to find nearest neighbor: -1 -1
Nearest neighbor found: [0.000000, 0.000000] - [1.000000, 2.000000] 

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 2
Enter point (x y) to find nearest neighbor: 10 10
Nearest neighbor found: [9.000000, 9.000000] - [10.000000, 10.000000]

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 3
Enter filename to save tree: my_tree
Tree saved successfully to my_tree

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 4
Enter filename to load tree: my_tree
Tree loaded successfully from my_tree

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 2
Enter point (x y) to find nearest neighbor: 2 2 
Nearest neighbor found: [0.000000, 0.000000] - [1.000000, 2.000000] 

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 2
Enter point (x y) to find nearest neighbor: 11 11
Nearest neighbor found: [9.000000, 9.000000] - [10.000000, 10.000000]

1. Insert Entry
2. Search Nearest Neighbor
3. Save Tree
4. Load Tree
5. Exit
Choose an option: 5 </p>