#include <iostream>
#include "RBTree.h"
#include <chrono>
#include <random>


RBTree<int,int>* genTree(const int length, const bool mode) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, length);
	auto tree = new RBTree<int,int>();
	if (length > 0) {
		if (mode) {
			for (int i = 1; i <= length; i++)
				tree->insert(i,i);
		}
		else {
			for (int i = 0; i < length; i++) {
				int key = dis(gen);
				tree->insert(key,key);
			}
		}
	}
	return tree;
}

//auto start = std::chrono::high_resolution_clock::now();
//func(array, len);
//auto end = std::chrono::high_resolution_clock::now();
//if (verifyArray(array, len)) {
//	std::chrono::duration<double, std::milli> time = end - start;
//	std::cout << method[i] << " T(n) = " << time.count() << " S(n) = " << globalSn << std::endl;
//	i++;
//}

//int main(void) {
//	srand((unsigned int)time(NULL));
//	const int maxIter = 100;
//	const int multiply = 10000;
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	for (int i = 1; i <= maxIter; i++) {
//		std::uniform_int_distribution<> dis(1, multiply * i);
//		int j = 0;
//		double time = 0;
//		auto tree = genTree(multiply * i, false);
//		while (j < 10) {
//			int key = dis(gen);
//			auto start = std::chrono::high_resolution_clock::now();
//			tree->insert(key,key);
//			auto end = std::chrono::high_resolution_clock::now();
//			time += std::chrono::duration<double, std::milli> (end - start).count();
//			j++;
//		}
//		std::cout << /*"Quantity of elements = " << tree->getSize() << "; Average time = " <<*/ time / j << std::endl;
//		delete tree;
//	}
//	return 0;
//}



int main(void) {
	int key;
	int bufKey, bufValue;
	auto tree = *new RBTree<int,int>();
	while (true) {
		std::cout << "1 - insert\n2 - remove\n3 - print all\n4 - print element\n5 - print element with iterators\n6 - clear\n7 - get size\n0 - exit\n";
		std::cout << "> ";
		std::cin >> key;
		switch (key) {
		case 1:
			std::cout << "Enter key: ";
			std::cin >> bufKey;
			std::cout << "Enter value: ";
			std::cin >> bufValue;
			tree.insert(bufKey, bufValue);
			break;
		case 2:
			std::cout << "Enter key for remove: ";
			std::cin >> bufKey;
			try {
				tree.remove(bufKey);
			}
			catch(const char* err){
				std::cout << err << '\n';
			}
			break;
		case 3:
			try {
				tree.print();
			}
			catch(const char* err){
				std::cout << err << '\n';
			}
			break;
		case 4:
			std::cout << "Enter key element: ";
			std::cin >> bufKey;
			try {
				std::cout << tree[bufKey] << '\n';
			}
			catch (const char* err) {
				std::cout << err << '\n';
			}
			break;
		case 5:
			try {
				for (int& iter : tree)
					std::cout << iter << " ";
				std::cout << '\n';
			}
			catch(const char* err){
				std::cout << err << '\n';
			}
			break;
		case 6:
			tree.clear();
			break;
		case 7:
			std::cout << tree.getSize() << '\n';
			break;
		default:
		case 0:
			delete &tree;
			return 0;
		}
	}
}