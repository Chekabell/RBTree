#ifndef RBTree_H
#define RBTree_H
#include <iostream>

template <typename typeKey, typename typeObj>
class RBTree {
protected:
	class Node {
	public:
		typeKey key;
		typeObj obj;
		bool color;
		Node* left;
		Node* right;
		Node* parent;
		Node(typeKey k, typeObj o) {
			key = k;
			obj = o;
			color = true;
			parent = nullptr;
			left = nullptr;
			right = nullptr;
		}
		Node() {
			key = 0;
			color = false;
			parent = nullptr;
			left = nullptr;
			right = nullptr;
		}
		~Node() {
			if (this->parent == nullptr ) return;
			if (this->left->left != nullptr)
				delete this->left;
			if(this->right->right != nullptr)
				delete this->right;
		}
	};
	typedef Node* pNode;
	class Iterator {
	private:
		RBTree* tree;
		pNode node;
	public:
		Iterator(pNode n, RBTree* t) {
			node = n;
			tree = t;
		}
		typeObj& operator*() {
			return node->obj;
		}
		bool operator != (const Iterator& it) {
			return node != it.node;
		}
		bool operator == (const Iterator& it) {
			return node == it.node;
		}
		Iterator& operator++ () {
			_increment();
			return *this;
		}
		Iterator operator++ (int) {
			Iterator temp(*this);
			_increment();
			return temp;
		}
		Iterator& operator-- () {
			_decrement();
			return *this;
		}
		Iterator operator-- (int) {
			Iterator temp(*this);
			_decrement();
			return temp;
		}
	private:
		void _increment() {
			pNode _nul = tree->_getNul();
			if (node->right != _nul) {
				node = tree->_getMin(node->right);
			}
			else {
				pNode par = node->parent;
				while (node == par->right) {
					node = par;
					par = par->parent;
				}
				if (node->right != par) node = par;
				if (par == _nul && node->right == _nul) node = _nul;
			}
		}
		void _decrement() {
			pNode nul = tree->_getNul();
			if (node->left != nul) {
				node = tree->_getMax(node->left);
			}
			else {
				pNode par = node->parent;
				while (node == par->left) {
					node = par;
					par = par->parent;
				}
				if (node->left != par) node = par;
				if (par == nul && node->left == nul) node = nul;
			}
		}
	};
private:
	pNode root;
	int size;
	pNode nul;
public:
	RBTree() {
		nul = new Node();
		root = nul;
		size = 0;
	}

	int getSize() const
	{
		return size;
	}
	void insert(typeKey key, typeObj obj) {
		pNode curr = root;
		pNode parent = nul;
		while (curr != nul) {
			parent = curr;
			if (key < curr->key) curr = curr->left;
			else curr = curr->right;
		}
		pNode newNode = new Node(key, obj);
		newNode->left = nul;
		newNode->right = nul;
		newNode->parent = parent;
		if (parent == nul) root = newNode;
		else if (key < parent->key) parent->left = newNode;
		else parent->right = newNode;
		_balanceInsert(newNode);
		size++;
	}
	void remove(typeKey key) {
		pNode nodeToDelete = _search(key);
		pNode child;
		bool removedNodeColor = nodeToDelete->color;
		if (_getChildrenCount(nodeToDelete) < 2) {
			child = _getChildOrNul(nodeToDelete);
			_transplantNode(nodeToDelete, child);
			delete nodeToDelete;
		}
		else {
			pNode minNode = _getMin(nodeToDelete->right);
			nodeToDelete->key = minNode->key;
			nodeToDelete->obj = minNode->obj;
			removedNodeColor = minNode->color;
			child = _getChildOrNul(minNode);
			_transplantNode(minNode, child);
			delete minNode;
		}
		if (removedNodeColor == false) _fixAfterRemoval(child);
		nul->parent = nullptr;
		size--;
	}
	typeObj& operator [](typeKey key) {
		return _search(key)->obj;
	}
	void clear() {
		delete this->root;
		root = nul;
		size = 0;
	}
	void print() {
		if (root == nul) throw std::exception("Tree is empty!");
		_print(root, 0);
	}
	Iterator begin() {
		if (root == nul) throw std::exception("Tree is empty!");
		return Iterator(_getMin(root), this); 
	}
	Iterator rbegin() {
		if (root == nul) throw std::exception("Tree is empty!");
		return Iterator(_getMax(root), this);
	}
	Iterator end() { 
		if (root == nul) throw std::exception("Tree is empty!");
		return Iterator(_getMax(root)->right, this); 
	}
	Iterator rend() {
		if (root == nul) throw "Tree is empty!";
		return Iterator(_getMin(root)->left, this);
	}
	
private:
	pNode _search(typeKey key) {
		pNode curr = root;
		while (curr->key != key) {
			if (curr == nul) throw std::exception("Key doesn't exist in the tree!");
			if (curr->key > key)
				curr = curr->left;
			else
				curr = curr->right;
		}
		return curr;
	}
	void _print(pNode node, int level){
		if (node == nul) return; 
		_print(node->left, level + 1);
		for (int i = 0; i < level; i++) std::cout << "   ";
		std::cout << node->color << ":" << node->key << std::endl;
		_print(node->right, level + 1);
	}
	void _clearTree(pNode node) {
		if (node == nul) return;
		_clearTree(node->left);
		_clearTree(node->right);
		delete node;
	}
	void _balanceInsert(pNode newNode) {
		while (newNode->parent->color) {
			if (newNode->parent == newNode->parent->parent->left) {
				pNode uncle = newNode->parent->parent->right;
				if (uncle->color) {
					newNode->parent->color			= false;
					uncle->color					= false;
					newNode->parent->parent->color	= true;
					newNode							= newNode->parent->parent;
				}
				else {
					if (newNode == newNode->parent->right) {
						newNode						= newNode->parent;
						_leftRotate(newNode);
					}
					newNode->parent->color			= false;
					newNode->parent->parent->color	= true;
					_rightRotate(newNode->parent->parent);
				}
			}
			else {
				pNode uncle = newNode->parent->parent->left;
				if (uncle->color) {
					newNode->parent->color			= false;
					uncle->color					= false;
					newNode->parent->parent->color	= true;
					newNode = newNode->parent->parent;
				}
				else {
					if (newNode == newNode->parent->left) {
						newNode						= newNode->parent;
						_rightRotate(newNode);
					}
					newNode->parent->color			= false;
					newNode->parent->parent->color	= true;
					_leftRotate(newNode->parent->parent);
				}
			}
		}
		root->color = false;
	}
	int _getChildrenCount(pNode node) {
		int count = 0;
		if (node->left != nul)	count += 1;
		if (node->right != nul)	count += 1;
		return count;
	}
	pNode _getChildOrNul(pNode node) {
		return (node->left != nul) ? node->left : node->right;
	}
	void _fixAfterRemoval(pNode node) {
		while (node != root && !node->color) {
			pNode brother;
			if (node == node->parent->left) {
				brother = node->parent->right;
				if (brother->color) {
					brother->color = false;
					node->parent->color = true;
					_leftRotate(node->parent);
					brother = node->parent->right;
				}
				if (!brother->left->color && !brother->right->color) {
					brother->color = true;
					node = node->parent;
				}
				else {
					if (!brother->right->color) {
						brother->left->color = false;
						brother->color = true;
						_rightRotate(brother);
						brother = node->parent->right;
					}
					brother->color = node->parent->color;
					node->parent->color = false;
					brother->right->color = false;
					_leftRotate(node->parent);
					node = root;
				}
			}
			else {
				brother = node->parent->left;
				if (brother->color) {
					brother->color = false;
					node->parent->color = true;
					_rightRotate(node->parent);
					brother = node->parent->left;
				}
				if (!brother->left->color && !brother->right->color) {
					brother->color = true;
					node = node->parent;
				}
				else {
					if (!brother->left->color) {
						brother->right->color = false;
						brother->color = true;
						_leftRotate(brother);
						brother = node->parent->left;
					}
					brother->color = node->parent->color;
					node->parent->color = false;
					brother->left->color = false;
					_rightRotate(node->parent);
					node = root;
				}
			}
		}
		node->color = false;
	}
	void _transplantNode(pNode toNode, pNode fromNode) {
		if (toNode == root) root = fromNode;
		if (toNode == toNode->parent->left) toNode->parent->left = fromNode;
		else toNode->parent->right = fromNode;
		fromNode->parent = toNode->parent;
	}
	void _leftRotate(pNode node) {
		pNode buffer = node->right->left;

		if (node == root) {
			node->right->parent = nul;
			root = node->right;
		}
		else if (node == node->parent->left)
			node->parent->left = node->right;
		else 
			node->parent->right = node->right;

		node->right->parent = node->parent;
		node->parent = node->right;
		node->parent->left = node;

		node->right = buffer;
		if (node->right != nul) node->right->parent = node;
	}
	void _rightRotate(pNode node) {
		pNode buffer = node->left->right;

		if (node == root) {
			node->left->parent = nul;
			root = node->left;
		}
		else if (node == node->parent->left)
			node->parent->left = node->left;
		else
			node->parent->right = node->left;

		node->left->parent = node->parent;
		node->parent = node->left;
		node->parent->right = node;

		node->left = buffer;

		if (node->left != nul) node->left->parent = node;
	}
	pNode _getMin(pNode node) {
		if (node == nul) return nul;
		if (node->left == nul) return node;
		return _getMin(node->left);
	}
	pNode _getMax(pNode node) {
		if (node == nul) return nul;
		if (node->right == nul) return node;
		return _getMax(node->right);
	}
	pNode _getNul() {
		return nul;
	}
};
#endif // RBT_HPP
