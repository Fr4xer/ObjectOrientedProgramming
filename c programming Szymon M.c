#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

struct treeNode {
	char name[20];
	int age;
	struct treeNode *left_parent;
	struct treeNode *right_parent;
};

typedef struct treeNode Node;

Node *loadData(Node *root);

Node *createNode(Node *root, char *name, int age, char *lParent, char *rParent);

Node *searchNode(Node *root, char *name);

void preorder(Node *root);

void menu(Node *);

void printTree(Node *root, int space);

Node *findOldest(Node *root, Node *node);

int isEmpty(Node *);

void printLeaves(Node *root);

void addNode(Node *);

void removeNode(Node *);

int main() {
	Node *root = NULL;
	
	root = loadData(root);
	if (root == NULL) {
		printf("** File Not Found **");
		return 0;
	}
	
	menu(root);
}

void menu(Node *root) {
	int option = 0;
	do {
		printf("\n");
		printf("1. Add a family member\n");
		printf("2. Print tree\n");
		printf("3. Find oldest\n");
		printf("4. Remove a person\n");
		printf("5. Print tree leaves\n");
		printf("6. Exit\n\n");
		
		printf("Enter option: ");
		scanf("%d", &option);
		printf("\n");
		
		switch (option) {
			case 1:
				addNode(root);
				break;
			case 2:
				preorder(root);
				printf("\n\n");
				printTree(root, 0);
				break;
			case 3:
				if (isEmpty(root) == TRUE) {
					printf("** Tree Is Empty **\n");
				} else {
					Node *node = root;
					node = findOldest(root, node);
					printf("Oldest Person: %s - %d years old\n", node->name, node->age);
				}
				break;
			case 4:
				removeNode(root);
				break;
			case 5:
				printf("Tree Leaves: ");
				printLeaves(root);
				printf("\n");
				break;
			case 6:
				break;
			default:
				printf("** Invalid Option **\n");
		}
	} while (option != 6);
}

void freeNodes(Node *root) {
	if (root != NULL) {
		if (root->left_parent != NULL) {
			freeNodes(root->left_parent);
			root->left_parent = NULL;
			free(root->left_parent);
		}
		if (root->right_parent != NULL) {
			freeNodes(root->right_parent);
			root->right_parent = NULL;
			free(root->right_parent);
		}
		
		free(root);
	}
}

Node *searchChild(Node *root, char *name) {
	if (isEmpty(root) == TRUE) return NULL;
	
	if (root->left_parent != NULL) {
		if (strcasecmp(root->left_parent->name, name) == 0) {
			return root;
		}
	}
	
	if (root->right_parent != NULL) {
		if (strcasecmp(root->right_parent->name, name) == 0) {
			return root;
		}
	}
	
	Node *node = searchChild(root->left_parent, name);
	if (node != NULL) {
		
		return node;
	}
	return searchChild(root->right_parent, name);
}

void removeNode(Node *root) {
	char name[20];
	printf("Enter Name To Remove: ");
	scanf("%s", name);
	
	Node *node = searchNode(root, name);
	if (node == NULL) {
		printf("\n** No Such Person Exists In The Tree **\n");
	} else {
		if (node == root) {
			freeNodes(root);
			root = NULL;
		} else {
			Node *child = searchChild(root, name);
			if (child->left_parent == node) {
				freeNodes(child->left_parent);
				child->left_parent = NULL;
			} else {
				freeNodes(child->right_parent);
				child->right_parent = NULL;
			}
		}
	}
}


void addNode(Node *root) {
	char name[20];
	int age;
	
	printf("Enter Name: ");
	scanf("%s", name);
	printf("Enter Age: ");
	scanf("%d", &age);
	
	Node *node = searchNode(root, name);
	if (isEmpty(node) != TRUE) {
		if (node->age == age) {
			printf("\n** Node Already Exists **\n");
			return;
		}
	}
	
	node = (Node *) malloc(sizeof(Node));
	strcpy(node->name, name);
	node->age = age;
	node->right_parent = NULL;
	node->left_parent = NULL;
	
	char child[20];
	printf("\nEnter Child Name: ");
	scanf("%s", child);
	Node *childNode = searchNode(root, child);
	if (childNode->left_parent == NULL) {
		childNode->left_parent = node;
	} else if (childNode->right_parent == NULL) {
		childNode->right_parent = node;
	} else {
		printf("\n** Child Has Already Both Parents In The Tree **\n");
	}
}

Node *loadData(Node *root) {
	FILE *file;
	file = fopen("familyinfo.txt", "r");
	if (file == NULL) {
		return 0;
	}
	
	char name[20], lParent[20], rParent[20];
	int age;
	while (fscanf(file, "%s %d %s %s", name, &age, lParent, rParent) != EOF) {
		Node *node = createNode(root, name, age, lParent, rParent);
		if (isEmpty(root) == TRUE) {
			root = node;
		}
	}
	return root;
}


Node *createNode(Node *root, char *name, int age, char *lParent, char *rParent) {
	Node *node = NULL;
	if (isEmpty(root) == FALSE) {
		node = searchNode(root, name);
	}
	
	if (node == NULL) {
		node = (Node *) malloc(sizeof(Node));
		strcpy(node->name, name);
	}
	
	node->age = age;
	if (strcmp(lParent, "None") == 0) {
		node->left_parent = NULL;
	} else {
		Node *lNode = (Node *) malloc(sizeof(Node));
		strcpy(lNode->name, lParent);
		lNode->age = 0;
		lNode->left_parent = NULL;
		lNode->right_parent = NULL;
		
		node->left_parent = lNode;
	}
	
	if (strcmp(rParent, "None") == 0) {
		node->right_parent = NULL;
	} else {
		Node *rNode = (Node *) malloc(sizeof(Node));
		strcpy(rNode->name, rParent);
		rNode->age = 0;
		rNode->left_parent = NULL;
		rNode->right_parent = NULL;
		
		node->right_parent = rNode;
	}
	
	return node;
	
}

Node *searchNode(Node *root, char *name) {
	if (isEmpty(root) == TRUE) return NULL;
	
	if (strcasecmp(root->name, name) == 0) {
		return root;
	}
	
	Node *node = searchNode(root->left_parent, name);
	if (isEmpty(node) == FALSE) {
		return node;
	}
	return searchNode(root->right_parent, name);
}

Node *findOldest(Node *root, Node *node) {
	if (isEmpty(root) != TRUE) {
		
		if (root->age > node->age) {
			node = root;
		}
		node = findOldest(root->left_parent, node);
		node = findOldest(root->right_parent, node);
	}
	return node;
}

void preorder(Node *root) {
	if (isEmpty(root) == TRUE)
		return;
	printf("%s, ", root->name);
	preorder(root->left_parent);
	preorder(root->right_parent);
}

void printTree(Node *root, int space) {
	if (isEmpty(root) == TRUE)
		return;
	
	space += 10;
	
	printTree(root->right_parent, space);
	
	for (int i = 10; i < space; i++)
		printf(" ");
	printf("%s %d\n", root->name, root->age);
	
	// Process left child
	printTree(root->left_parent, space);
}

void printLeaves(Node *root) {
	if (isEmpty(root) == TRUE)
		return;
	
	if (isEmpty(root->left_parent) && isEmpty(root->right_parent)) {
		printf("%s, ", root->name);
		return;
	}
	
	printLeaves(root->left_parent);
	printLeaves(root->right_parent);
}

int isEmpty(Node *root) {
	if (root == NULL) {
		return TRUE;
	}
	return FALSE;
}
