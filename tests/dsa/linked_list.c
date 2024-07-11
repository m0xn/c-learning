#include <stdio.h>
#include <stdlib.h>

#define UNIMPLEMENTED printf("[ERROR]:%s:%d %s has been not implemented yet\n", __FILE__, __LINE__, __func__)

struct Node {
	int val;
	struct Node *next;
};

typedef struct Node Node;

Node* add_node(Node *last, int val) {
	Node *new_node = (Node*)malloc(sizeof(Node));
	last->val = val;
	return new_node;
}

void pop_node(Node *last) {
	UNIMPLEMENTED;
}

Node* populate_recursive_ll(Node *ref, int max) {
	if (ref->val == max)
		return ref;

	Node *node = (Node*)malloc(sizeof(Node));
	node->val = ref->val+1;
	ref->next = node;
	
	populate_recursive_ll(node, max);
}

Node* populate_ll(Node *root, int iters)
{
	Node *curr = root;
	for (int i = root->val + 1; i <= root->val + iters; ++i) {
		Node *node = (Node*)malloc(sizeof(Node));
		node->val = i;
		curr->next = node;
		curr = node;
	}
	
	return curr;
}

void print_ll(Node root) {
	Node *curr = &root;
	while (curr->next != NULL) {
		printf("%d -> ", curr->val);
		curr = curr->next;
	}
	printf("%d\n", curr->val);
}

int main()
{
	Node root = {0};
	Node *last;
	last = populate_recursive_ll(&root, 100);
	print_ll(root);
	return 0;
}
