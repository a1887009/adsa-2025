#include <iostream>
#include <algorithm>
#include <vector>

// Structure for integer node
struct Node
{
    int key;
    int height;
    Node *left, *right;
    Node(int val) : key(val), height(1), left(nullptr), right(nullptr) {}
};

// Useful functions for interacting with nodes and structure
int getHeight(Node* node)
{
    if (node == nullptr)
        return 0;
    return node->height;
}

int getBalance(Node* node)
{
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

void updateHeight(Node* node)
{
    if (node)
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
}

// Function for right rotation
Node* rotateRight(Node* y)
{
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

// Function for left rotation
Node* rotateLeft(Node* x)
{
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// Function to balance the tree
Node* balance(Node* node)
{
    updateHeight(node);
    int balance = getBalance(node);

    // Left heavy
    if (balance > 1)
    {
        if (getBalance(node->left) < 0)
            node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right heavy
    if (balance < -1)
    {
        if (getBalance(node->right) > 0)
            node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Even
    return node;
}

Node* insert(Node* node, int key)
{
    // Follows down the tree to insert
    if (node == nullptr) return new Node(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else 
        return node; // Ignore duplicate data
    
    // Balance and return
    return balance(node);
}

Node* minValueNode(Node* node)
{
    // Min value always found on the left
    Node* current = node;
    while (current->left != nullptr) current = current->left;
    return current;
}

Node* deleteNode(Node* root, int key)
{
    if (root == nullptr)
        return root;
    
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else 
    {
        // Node with only 1 child or no child
        if ((root->left == nullptr) || (root->right == nullptr))
        {
            Node* temp = root->left ? root->left : root->right;
            if (temp == nullptr)
            {
                temp = root;
                root = nullptr;
            }
            else 
                *root = *temp;
            delete temp;
        }
        else 
        {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == nullptr) return root;

    // Balance the tree
    return balance(root); 
}

void preorder(Node* root, std::vector<int>& res)
{
    if (root == nullptr) return;
    res.push_back(root->key);
    preorder(root->left, res);
    preorder(root->right, res);
}

void inorder(Node* root, std::vector<int>& res)
{
    if (root == nullptr) return;
    inorder(root->left, res);
    res.push_back(root->key);
    inorder(root->right, res);
}

void postorder(Node* root, std::vector<int>& res)
{
    if (root == nullptr) return;
    postorder(root->left, res);
    postorder(root->right, res);
    res.push_back(root->key);
}

int main()
{
    Node* root = nullptr;
    std::string token;

    // Keeps reading until the end is hit
    while (std::cin >> token) {
        if (token == "PRE" || token == "IN" || token == "POST") {
            // If the end is hit the vector is generated
            std::vector<int> res;
            if (token == "PRE") preorder(root, res);
            else if (token == "IN") inorder(root, res);
            else if (token == "POST") postorder(root, res);

            if (res.empty()) {
                std::cout << "EMPTY\n";
            } else {
                for (int i = 0; i < (int)res.size(); i++) {
                    if (i) std::cout << " ";
                    std::cout << res[i];
                }
                std::cout << "\n";
            }
            break;
        }

        // If its an insert or delete we follow this logic
        if (token[0] == 'A') {
            int val = stoi(token.substr(1));
            root = insert(root, val);
        } else if (token[0] == 'D') {
            int val = stoi(token.substr(1));
            root = deleteNode(root, val);
        }
    }

    return 0;
}