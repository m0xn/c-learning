#include <stdio.h>
#include <stdlib.h>

struct Node {
	int val;
	struct Node *right;
	struct Node *left;
};

typedef struct Node Node;

int powd(int base, int exp)
{
	if (exp == 0)
		return 1;

	int p;

	for (p = 1; exp > 0; --exp)
		p *= base;

	return p;
}

void print_spaces(int amount)
{
	for (int i = 0; i < amount; ++i)
		printf(" ");
}

void gen_tree(Node *root, int depth)
{
	int n = root->val;
	int current_depth = 0;
	Node **nodes = (Node**)malloc(sizeof(Node));
	nodes[0] = root;

	for (; depth > 1; --depth) {
		Node **node_buffer = (Node**)calloc(powd(2, current_depth+1), sizeof(Node));
		int buffer_idx = 0;
		for (int i = 0; i < powd(2, current_depth); ++i) {
			Node *left_node = (Node*)malloc(sizeof(Node));
			Node *right_node = (Node*)malloc(sizeof(Node));

			left_node->val = ++n;
			right_node->val = ++n;

			nodes[i]->left = left_node;
			nodes[i]->right = right_node;
			
			node_buffer[buffer_idx++] = left_node;
			node_buffer[buffer_idx++] = right_node;
		}
		nodes = node_buffer;
		current_depth++;
	}
}

//  TODO: Fix the formatting for the tree, not quite there but the algorithm works well
// void print_tree(Node *root, int depth)
// {
// 	int current_depth = 0;
// 	int tree_depth = depth;
// 	Node **nodes = (Node**)malloc(sizeof(Node));
// 	nodes[0] = root;
//
// 	for (; depth > 0; --depth) {
// 		Node **node_buffer = (Node**)calloc(powd(2, current_depth+1), sizeof(Node));
// 		int buffer_idx = 0;
// 		for (int i = 0; i < powd(2, current_depth); ++i) {
// 			print_spaces(depth*tree_depth*2);
// 			printf("%d", nodes[i]->val);
// 			node_buffer[buffer_idx++] = nodes[i]->left;
// 			node_buffer[buffer_idx++] = nodes[i]->right;
// 		}
// 		printf("\n");
// 		nodes = node_buffer;
// 		++current_depth;
// 	}
//
// 	free(nodes);
// }

int main()
{
	Node root = {0};
	int depth = 6;
	gen_tree(&root, depth);
	print_tree(&root, depth);
	return 0;
}
