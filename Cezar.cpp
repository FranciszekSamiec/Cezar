#include <stdio.h>
#include <iostream>

using namespace std;

// Results may be big so we need to divide them modulo.
static const long long mod = 1000000000 + 7;

// Key is only for ease of operation insert. Reverse bit is a flag
// which tells us whether segment represented by subtree is reversed 
// or not. Left and Right desc means number of descendants, we use that
// info too keep track of order of elements (we cannot simply keep info 
// like index because segments are going to be reversed). RR, RG and so 
// on are the numbers of segments represented by the subtree which start 
// with R or G and end with R or G for instance RR is the number of 
// proper sequnces in the subtree which start and end with R. Nationality 
// means the nationality of a node: R for Roman ang G for Gaul.
class node
{
public:
	int leftDesc;
	int rightDesc;
	int key;
	int reverseBit;
	long long RR, RG, GR, GG;
	char nationality;
	node* left, * right;
};

node* newNode(int key, char c)
{
	node* Node = new node();
	Node->reverseBit = 0;
	Node->key = key;
	Node->left = Node->right = NULL;
	Node->nationality = c;
	if (c == 'R') {
		Node->RR = 1;
		Node->RG = 0;
		Node->GR = 0;
		Node->GG = 0;
	}
	else {
		Node->RR = 0;
		Node->RG = 0;
		Node->GR = 0;
		Node->GG = 1;
	}

	return (Node);
}

void copyRRs(node* from, node* to) {
	to->RR = from->RR;
	to->RG = from->RG;
	to->GR = from->GR;
	to->GG = from->GG;
}

void combineRRs(node* a, node* b, node* target) {
	if (a == NULL) copyRRs(b, target);
	else if (b == NULL) copyRRs(a, target);
	else {
		target->RR = (a->RR * (b->RR + b->GR + 1) + a->RG * b->RR + b->RR) % mod;
		target->RG = (a->RG + b->RG + (a->RG * b->RG) + (a->RR * b->RG) + (a->RR * b->GG)) % mod;
		target->GR = (a->GR + b->GR + (a->GR * b->GR) + (a->GR * b->RR) + (a->GG * b->RR)) % mod;
		target->GG = (a->GG + b->GG + (a->GR * b->RG) + (a->GR * b->GG) + (a->GG * b->RG)) % mod;
	}
}

void updateRRs(node* n) {
	node* target = newNode(-1, 'G');

	if (n->nationality == 'G') {
		n->RR = 0;
		n->RG = 0;
		n->GR = 0;
		n->GG = 1;
	}
	else {
		n->RR = 1;
		n->RG = 0;
		n->GR = 0;
		n->GG = 0;
	}

	combineRRs(n->left, n, target);
	combineRRs(target, n->right, n);

	delete target;
}

void updateDesc(node* n) {
	if (n->left != NULL)
		n->leftDesc = n->left->leftDesc + n->left->rightDesc + 1;
	else
		n->leftDesc = 0;

	if (n->right != NULL)
		n->rightDesc = n->right->leftDesc + n->right->rightDesc + 1;
	else
		n->rightDesc = 0;
}

void updateNode(node* n) {
	updateDesc(n);
	updateRRs(n);
}

node* rightRotate(node* x)
{
	node* y = x->left;
	y->leftDesc = x->left->leftDesc;
	x->leftDesc = y->rightDesc;
	x->left = y->right;
	updateRRs(x);
	y->right = x;
	updateRRs(y);
	y->rightDesc = x->leftDesc + x->rightDesc + 1;

	return y;
}

node* leftRotate(node* x)
{
	node* y = x->right;
	y->rightDesc = x->right->rightDesc;

	x->right = y->left;
	updateRRs(x);

	y->left = x;
	updateRRs(y);

	x->rightDesc = y->leftDesc;
	y->leftDesc = x->leftDesc + x->rightDesc + 1;
	return y;
}

void dealWithRev(node* n) {
	if (n != NULL) {
		if (n->reverseBit == 1) {
			int curr = n->rightDesc;
			n->rightDesc = n->leftDesc;
			n->leftDesc = curr;
			node* temp = n->right;
			n->right = n->left;
			n->left = temp;

			if (n->left != NULL) {
				node* l = n->left;
				long long temp = l->RG;
				l->RG = l->GR;
				l->GR = temp;

			}
			if (n->right != NULL) {
				node* r = n->right;
				long long temp = r->RG;
				r->RG = r->GR;
				r->GR = temp;

			}
			updateRRs(n);
			if (n->left != NULL) {
				if (n->left->reverseBit == 0) {
					n->left->reverseBit = 1;
				}
				else
					if (n->left->reverseBit == 1) {
						n->left->reverseBit = 0;
					}
			}
			if (n->right != NULL) {
				if (n->right->reverseBit == 0) {

					n->right->reverseBit = 1;
				}
				else
					if (n->right->reverseBit == 1) {
						n->right->reverseBit = 0;
					}
			}
			n->reverseBit = 0;
		}
	}
}

node* splay(node* root, int key)
{
	dealWithRev(root);
	updateNode(root);

	if (root == NULL || root->leftDesc == key - 1)
		return root;

	if (root->leftDesc > key - 1)
	{
		dealWithRev(root->left);
		updateNode(root);
		if (root->left == NULL) return root;

		if (root->left->leftDesc > key - 1)
		{
			root->left->left = splay(root->left->left, key);
			root = rightRotate(root);
		}
		else if (root->left->leftDesc < key - 1) 
		{
			key--;
			key -= root->left->leftDesc;
			root->left->right = splay(root->left->right, key);
			if (root->left->right != NULL)
				root->left = leftRotate(root->left);
		}
		return (root->left == NULL) ? root : rightRotate(root);
	}
	else 
	{
		dealWithRev(root->right);
		updateNode(root);
		key--;
		key -= root->leftDesc;
		if (root->right == NULL) return root;

		if (root->right->leftDesc > key - 1)
		{
			root->right->left = splay(root->right->left, key);

			if (root->right->left != NULL)
				root->right = rightRotate(root->right);
		}
		else if (root->right->leftDesc < key - 1)
		{
			key--;
			key -= root->right->leftDesc;
			root->right->right = splay(root->right->right, key);
			root = leftRotate(root);
		}
		// Do second rotation for root  
		return (root->right == NULL) ? root : leftRotate(root);
	}
}

// Function to insert a new key k  
// in splay tree with given root  
node* insert(node* root, long long k, char c)
{
	if (root == NULL) return newNode(k, c);

	root = splay(root, k);

	if (root->key == k) return root;

	node* newnode = newNode(k, c);

	if (root->key > k)
	{
		newnode->right = root;
		newnode->left = root->left;
		root->left = NULL;
	}
	else
	{
		newnode->left = root;
		newnode->right = root->right;
		root->right = NULL;
	}
	return newnode; 
}

int setDesc(node* root) {
	if (root != NULL) {
		root->reverseBit = 0;
		root->leftDesc = setDesc(root->left);
		root->rightDesc = setDesc(root->right);
		return root->leftDesc + root->rightDesc + 1;
	}
	else {
		return 0;
	}
}

void setRRs(node* n) {
	if (n != NULL) {

		setRRs(n->left);
		setRRs(n->right);
	}
}

node* subStrings(node* n, int a, int b) {
	node* root = splay(n, a);
	node* leftTree = root->left;
	root->left = NULL;
	updateNode(root);
	root = splay(root, b - a + 1);
	node* rightTree = root->right;
	root->right = NULL;
	updateNode(root);
	cout << (root->RR + root->RG + root->GR + root->GG) % mod << endl;

	root->right = rightTree;
	updateNode(root);
	root = splay(root, 1);
	root->left = leftTree;
	updateNode(root);
	return root;
}


node* reverse(node* n, int a, int b) {
	node* root = splay(n, a);
	node* leftTree = root->left;
	root->left = NULL;
	updateNode(root);

	root = splay(root, b - a + 1);

	node* rightTree = root->right;
	root->right = NULL;
	updateNode(root);

	if (root->reverseBit == 0)
		root->reverseBit = 1;
	else
		root->reverseBit = 0;

	root = splay(root, 1);

	root->left = leftTree;
	updateNode(root);

	root = splay(root, b);
	root->right = rightTree;
	updateNode(root);

	return root;
}

int main()
{
	node* root = NULL;
	long long n, m;
	cin >> n;
	cin >> m;
	long long count = 1;

	while (count <= n)
	{
		char l;
		cin >> l;
		root = insert(root, count, l);
		count++;
	}
	
	setDesc(root);
	setRRs(root);
	count = m;

	while (count > 0) {
		char questionType;
		cin >> questionType;

		int a, b;
		cin >> a;
		cin >> b;

		if (questionType == '?') {
			root = subStrings(root, a, b);
		}
		else {
			root = reverse(root, a, b);
		}
		count--;
	}
	return 0;
}
