#include "RedBlackTree.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <climits> // it's probably not necessary but I'm paranoid

using namespace std;

RedBlackTree::RedBlackTree() {
	// root doesn't need to be initialized, since it gets replaced by the first value inserted into the tree.
	// empty is a null node used to mark empty space in the tree's paths
    empty->left = nullptr;
    empty->right = nullptr;
    empty->color = COLOR_BLACK_BLACK;
    empty->val = 0;
}

RedBlackTree::RedBlackTree(const RedBlackTree &other) {
	RedBlackTree* temp = new RedBlackTree;
	copy_helper(other.root, temp);
	root = temp->root;
}

void RedBlackTree::copy_helper(RBTNode* where, RedBlackTree *rbt) {
	if (where->left != empty && where->left != nullptr) {
		copy_helper(where->left, rbt);
	}
	if (where->right != empty && where->right != nullptr) {
		copy_helper(where->right, rbt);
	}
	if (where->val != 0) {
		cout << "val: " << where->val << endl;
		rbt->Insert(where->val);
	}
}

RedBlackTree::~RedBlackTree() {
	while (numItems != 0) {
		Remove(GetMax());
	}
}

bool RedBlackTree::Search(RBTNode* where, int a) {
	bool l_path = false, r_path = false;
	if (where == nullptr) {
		return false;
	}
	if (where->val == a) {
			return true;
	}
	if (where->left != empty) {
		if (where->left->val < a) {
			l_path = Search(where->left, a);
		}
	}
	if (where->right != empty) {
		if (where->right->val >= a) {
			r_path = Search(where->right, a);
		}
	}
	return (l_path || r_path);
}

int RedBlackTree::SearchMin(RBTNode* where) {
	while (where->left != empty && where->right != nullptr) {
		where = where->left;
	}
	return where->val;
}

int RedBlackTree::SearchMax(RBTNode* where) {
    while (where->right != empty && where->right != nullptr) {
		where = where->right;
	}
	return where->val;
}

string GetColor(RBTNode* rbt) {
if (rbt->color == COLOR_BLACK) {
    return "B";
}
else if (rbt->color == COLOR_RED) {
    return "R";
}
else if (rbt->color == COLOR_BLACK_BLACK) {
    return "BB";
}
return "";
}

RBTNode* RedBlackTree::zenith(RBTNode* where, int a) {
    RBTNode* destination = nullptr;
	if (where->val == a) {
		return where;
	}
	while (where != empty) { // Scan down the tree until you find a null node
			destination = where;
			if (a < where->val) {
				where = where->left;
			} else {
				where = where->right;
			}
		}
	return destination;
}

bool RedBlackTree::Contains(int a) {
	if (root == empty || root == nullptr) {
		return false;
	}
    return Search(root, a);
}

int RedBlackTree::GetMin() {
    return SearchMin(root);
}

int RedBlackTree::GetMax() {
    return SearchMax(root);
}

void RedBlackTree::L_Rotate(RBTNode* rbt) {
    //cout << "rotate left!" << endl;
    RBTNode* pivot = rbt->right;
		rbt->right = pivot->left;
		if (pivot->left != empty) {
			pivot->left->prev = rbt;
		}
		pivot->prev = rbt->prev;
		if (rbt->prev == nullptr) {
			this->root = pivot;
		} else if (rbt == rbt->prev->left) {
			rbt->prev->left = pivot;
		} else {
			rbt->prev->right = pivot;
		}
		pivot->left = rbt;
		rbt->prev = pivot;
}

void RedBlackTree::R_Rotate(RBTNode* rbt) {
    //cout << "rotate right!" << endl;
    RBTNode* pivot = rbt->left;
		rbt->left = pivot->right;
		if (pivot->right != empty) {
			pivot->right->prev = rbt;
		}
		pivot->prev = rbt->prev;
		if (rbt->prev == nullptr) {
			this->root = pivot;
		} else if (rbt == rbt->prev->right) {
			rbt->prev->right = pivot;
		} else {
			rbt->prev->left = pivot;
		}
		pivot->right = rbt;
		rbt->prev = pivot;
}

void RedBlackTree::validate(RBTNode* rbt) {
        //cout << "fixing tree!" << endl;
		RBTNode* uncle;
		while (rbt->prev->color == COLOR_RED) {
			if (rbt->prev == rbt->prev->prev->right) {
				// If the previous offenders are skewed to the right
				uncle = rbt->prev->prev->left; // uncle
				if (uncle->color == COLOR_RED) {
					// If the uncle is red
					uncle->color = COLOR_BLACK;
					rbt->prev->color = COLOR_BLACK;
					rbt->prev->prev->color = COLOR_RED;
					rbt = rbt->prev->prev;
				} else {
					if (rbt == rbt->prev->left) {
						// If the current offenders are skewed to the right
						rbt = rbt->prev;
						R_Rotate(rbt);
					}
					// If the current offenders are skewed to the left
					rbt->prev->color = COLOR_BLACK;
					rbt->prev->prev->color = COLOR_RED;
					L_Rotate(rbt->prev->prev);
				}
			} else {
				// If the previous offenders are skewed to the left
				// (everything is the same, but left is swapped with right and vice versa)
				uncle = rbt->prev->prev->right; // uncle

				if (uncle->color == COLOR_RED) {
					// ...
					uncle->color = COLOR_BLACK;
					rbt->prev->color = COLOR_BLACK;
					rbt->prev->prev->color = COLOR_RED;
					rbt = rbt->prev->prev;	
				} else {
					if (rbt == rbt->prev->right) {
						// ...
						rbt = rbt->prev;
						L_Rotate(rbt);
					}
					// ...
					rbt->prev->color = COLOR_BLACK;
					rbt->prev->prev->color = COLOR_RED;
					R_Rotate(rbt->prev->prev);
				}
			}
			if (rbt == root) {
				break;
			}
		}
		root->color = COLOR_BLACK;
		// Root should ALWAYS be black
}

void RedBlackTree::Insert(int a) {

	numItems++;

	if (Contains(a)) {
		throw invalid_argument("Error: duplicate values cannot be inserted into the tree!");
	}

	RBTNode* insertMe = new RBTNode; // turning 'a' into a leaf; it's red, has empty children, and stores 'a' as a value
	// Side note: I probably could've used nullptr instead of an empty double black node, but based on the links
	// and other sources that I found, double black nodes are instrumental to making remove work, so I think it's best to add them now.
	insertMe->prev = nullptr; insertMe->val = a; insertMe->left = empty; insertMe->right = empty; insertMe->color = COLOR_RED;

	if (root == nullptr) {
		// If root isn't defined, that new node becomes black and replaces root.
		root = insertMe;
		root->color = COLOR_BLACK;
		return;
	}

	// Otherwise, run a function to find the last node that would appear before the inserted node.
	// Ex: if the tree has 10, 20, 30, 40, 50 with root 20, inserting 35 would trace through 20->40->30 and place a new node to its right.
	RBTNode* destination = zenith(root, a);

	insertMe->prev = destination;
	if (insertMe->val < destination->val) {
		destination->left = insertMe;
	}
	else {
		destination->right = insertMe;
	}
    validate(insertMe);
}

RBTNode* RedBlackTree::in_order_successor(RBTNode* rbt) {
	RBTNode* output = rbt;
	while (output->left != empty) {
		output = output->left;
	}
	return output;
}

void RedBlackTree::Remove(int a) {
	remove_helper(root, a);
	numItems--;
};

void RedBlackTree::remove_helper(RBTNode* where, int a) {
	RBTNode* temp;
	RBTNode* deleteMe = zenith(where, a);
	RBTNode* problem_child;
	int temp_color = COLOR_RED;
	if (deleteMe == nullptr) {
		// throw exception if node cannot be found
		throw invalid_argument("Error: Cannot find node to be deleted!");
	}
	// One child: replace node with that child
	if (deleteMe->left == empty) {
		problem_child = deleteMe->right;
		shift_up(deleteMe,problem_child);
	}
	else if (deleteMe->right == empty) {
		problem_child = deleteMe->left;
		shift_up(deleteMe,problem_child);
	}
	// Two children: find i_o_s and replace node with i_o_s
	else {
			temp = in_order_successor(deleteMe);
			temp_color = temp->color;
			problem_child = temp->right;
			if (temp->prev == deleteMe) {
				problem_child->prev = temp;
			}
			else {
				shift_up(temp, temp->right);
				temp->right = deleteMe->right;
				temp->right->prev = temp;
			}

			shift_up(deleteMe, temp);
			temp->left = deleteMe->left;
			temp->left->prev = temp;
			temp->color = deleteMe->color;
		}
	if (temp_color == COLOR_BLACK) {
		validate_remove(deleteMe);
	}
}

void RedBlackTree::validate_remove(RBTNode* where) {
	RBTNode* marker;
		while (where != root && where->color == COLOR_BLACK) {
			if (where == where->prev->left) {
				marker = where->prev->right;
				if (marker->color == COLOR_RED) {
					marker->color = COLOR_BLACK;
					where->prev->color = COLOR_RED;
					L_Rotate(where->prev);
					marker = where->prev->right;
				}

				if (marker->left->color == COLOR_BLACK && marker->right->color == COLOR_BLACK) {
					marker->color = COLOR_RED;
					where = where->prev;
				}
				else {
					if (marker->right->color == COLOR_BLACK) {
						marker->left->color = COLOR_BLACK;
						marker->color = COLOR_RED;
						R_Rotate(marker);
						marker = where->prev->right;
					} 

					marker->color = where->prev->color;
					where->prev->color = COLOR_BLACK;
					marker->right->color = COLOR_BLACK;
					L_Rotate(where->prev);
					where = root;
				}
			}
			else {
				marker = where->prev->left;
				if (marker->color == COLOR_RED) {
					marker->color = COLOR_BLACK;
					where->prev->color = COLOR_RED;
					R_Rotate(where->prev);
					marker = where->prev->left;
				}

				if (marker->right->color == COLOR_BLACK && marker->right->color == COLOR_BLACK) {
					marker->color = COLOR_RED;
					where = where->prev;
				}
				else {
					if (marker->left->color == COLOR_BLACK) {
						marker->right->color = COLOR_BLACK;
						marker->color = COLOR_RED;
						L_Rotate(marker);
						marker = where->prev->left;
					} 

					marker->color = where->prev->color;
					where->prev->color = COLOR_BLACK;
					marker->left->color = COLOR_BLACK;
					R_Rotate(where->prev);
					where = root;
				}
			} 
		}
		where->color = COLOR_BLACK;
}



void RedBlackTree::shift_up(RBTNode* to, RBTNode* from) {
		if (to->prev == nullptr) {
			root = from;
		}
		else if (to == to->prev->left) {
			to->prev->left = from;
		}
		else {
			to->prev->right = from;
		}
		from->prev = to->prev;
	}

const string RedBlackTree::ToInfixString(RBTNode* rbt) const {
    string output = "";
    if (rbt == nullptr) {
        return output;
    }
    string curr = GetColor(rbt) + to_string(rbt->val);
	// Go left, then print, then go right
    if (rbt->left != empty) {
        output += ToInfixString(rbt->left);
    }
	output += (" " + curr + " ");
    if (rbt->right != empty) {
        output += ToInfixString(rbt->right);
    }
    return output;
};
const string RedBlackTree::ToPrefixString(RBTNode* rbt) const {
    string output = "";
    if (rbt == nullptr) {
        return output;
    }
    string curr = GetColor(rbt) + to_string(rbt->val);
	// Print, then go left, then go right
	if (rbt->color != COLOR_BLACK_BLACK) {
		output += (" " + curr + " ");
	}
    
    if (rbt->left != empty) {
        output += ToPrefixString(rbt->left);
    }
    if (rbt->right != empty) {
        output += ToPrefixString(rbt->right);
    }
    return output;
};
const string RedBlackTree::ToPostfixString(RBTNode* rbt) const {
    string output = "";
    if (rbt == nullptr) {
        return output;
    }
    string curr = GetColor(rbt) + to_string(rbt->val);
	// go left, then go right, then print
    if (rbt->left != empty) {
        output += ToPostfixString(rbt->left);
    }
    if (rbt->right != empty) {
        output += ToPostfixString(rbt->right);
    }
	if (rbt->color != COLOR_BLACK_BLACK) {
		output += (" " + curr + " ");
	}
    return output;
};