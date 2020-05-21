#pragma once
#include <iostream>
#include <stdexcept>

template <class classType> class functorAdd {
	classType data;
public:
	functorAdd() : data(NULL) {}
	functorAdd(classType given) : data(given) {}

	classType operator()(classType arg) {
		return arg + data;
	}
};

template <class classType> class functorSub {
	classType data;
public:
	functorSub() : data(NULL) {}
	functorSub(classType given) : data(given) {}

	classType operator()(classType arg) {
		return arg - data;
	}
};

template <class classType> class multiset // am presupus ca classType poate fi comparat cu <,>,=, fiind un set ordonat
{
	struct node {

		classType data=NULL;

		node* left = nullptr, * right = nullptr;

	} *root;
	int size;

protected:
	void Del(node*& current);
	
	void pop(node*& current); //sterge un element si rearanjeaza arborele
	
	void insert(classType data, node*& current);
	
	void remove(classType data, node*& current);//itereaza recurziv prin arbore
	
	void SRDcount(node* current, int &counter, classType &lastSeen);
	
	void SRDcout(const node* current, std::ostream& output) const;
	
	void RSDcout(const node* current, std::ostream& output) const;
	
	void RSDcopy(const node* original, node *&destindation);
	
	template <class Func>
	void SRDfunctor(node*& current, Func &functor);

public:
	multiset();
	
	multiset(const multiset &obj);
	multiset<classType>& operator =(const multiset& obj);

	~multiset();

	void insert(classType data);
	void pop(classType data);

	int multiplicity(classType data);

	bool has(classType data);

	void remove(classType data);

	int elemCount();

	template <class Func>
	void modify(Func &functor);
	
	template <class classType>
	friend std::ostream& operator<< (std::ostream& output, const multiset<classType> & obj);
};

template<class classType>
multiset<classType>::multiset() : 
	root (nullptr), 
	size (0)
{}

template<class classType>
multiset<classType>::multiset(const multiset &obj)
{
	this->size = obj.size;
	RSDcopy(obj.root, this->root);
}

template<class classType>
inline multiset<classType>& multiset<classType>::operator=(const multiset& obj)
{
	this->size = obj.size;
	RSDcopy(obj.root, this->root);
	return *this;
}

template<class classType>
inline void multiset<classType>::Del(node* &current)
{
	if (current != nullptr) {

		Del(current->left);

		Del(current->right);

	}
	delete current;
}

template<class classType>
multiset<classType>::~multiset()
{
	Del(this->root);
}

template<class classType>
inline void multiset<classType>::insert(classType x, node*& current) {
	if (current == nullptr) {
		current = new node;
		current->data = x;
		this->size++;
	}
	else {
		if (x <= current->data) {
			insert(x, current->left);
		}
		else {
			insert(x, current->right);
		}
	}
}

template<class classType>
inline void multiset<classType>::remove(classType data, node*& current)
{
	if (current != nullptr) 
	{
		if (current->data == data) {
			pop(current);
			remove(data, current);
		}

		remove(data, current->left);

		remove(data, current->right);
	}
}

template<class classType>
inline void multiset<classType>::SRDcount(node* current, int &counter, classType &lastSeen)
{
	if (current != nullptr) {

		SRDcount(current->left, counter, lastSeen);

		if (current->data != lastSeen) {
			lastSeen = current->data;
			counter++;
		}

		SRDcount(current->right, counter, lastSeen);

	}
}

template<class classType>
inline void multiset<classType>::SRDcout (const node* current, std::ostream &output)const
{
	if (current != nullptr) {

		SRDcout(current->left, output);

		output << current->data << ' ';

		SRDcout(current->right, output);

	}
}

template<class classType>
inline void multiset<classType>::RSDcout (const node* current, std::ostream& output)const
{
	if (current != nullptr) {

		output << current->data << ' ';

		RSDcout(current->left, output);

		RSDcout(current->right, output);

	}
}

template<class classType>
inline void multiset<classType>::RSDcopy(const node* original, node *&destination)
{
	if (original != nullptr) {
		destination = new node;

		std::memcpy(destination, original, sizeof(original));

		RSDcopy(original->left, destination->left);
		
		RSDcopy(original->right, destination->right);
	}
}

template<class classType>
inline void multiset<classType>::pop(node*& current)
{
	if (current->left == nullptr) {
		if (current->right == nullptr) {
			delete current;
			current = nullptr;
		}
		else {
			current = current->right;
		}
	}
	else {
		if (current->right == nullptr) {
			current = current->left;
		}
		else {
			node*& replacement = current->left;
			while (replacement->right != nullptr)
				replacement = replacement->right;
			current->data = replacement->data;
			pop(replacement);
		}
	}
}

template<class classType>
inline void multiset<classType>::insert(classType data)
{
	insert(data, this->root);
}

template<class classType>
inline void multiset<classType>::pop(classType data)
{
	node* current = this->root;
	while (current != nullptr and current->data != data) {
		if (data > current->data)
			current = current->right;
		else
			current = current->left;
	}
	if (current!=nullptr) {
		pop(current);
		size--;
	}
	else
	{
		std::cout << "Nu a fost gasit.";
	}
}

template<class classType>
inline int multiset<classType>::multiplicity(classType data)
{
	node* current = this->root;
	while (current != nullptr and current->data != data) {
		if (data > current->data)
			current = current->right;
		else
			current = current->left;
	}
	if (current != nullptr) {
		int count = 1;
		while (current->left != nullptr) {
			current = current->left;
			count++;
		}
		return count;
	}
	return 0;
}

template<class classType>
inline bool multiset<classType>::has(classType data)
{
	node* current = this->root;
	while (current != nullptr and current->data != data) {
		if (data > current->data)
			current = current->right;
		else
			current = current->left;
	}
	if (current != nullptr) {
		return true;
	}
	return false;
}

template<class classType>
inline void multiset<classType>::remove(classType data)
{	
	remove(data, this->root);
}

template<class classType>
inline int multiset<classType>::elemCount()
{
	int counter = 0;
	classType lastSeen = NULL;
	SRDcount(this->root, counter, lastSeen);
	return counter;
}

template<class classType>
std::ostream& operator<< (std::ostream& output, const multiset<classType> & obj) {

	output << "SRD: ";
	obj.SRDcout(obj.root, output);
	output << "\nRSD: ";
	obj.RSDcout(obj.root, output);
	output << "\n";
	return output;
}

template<class classType>
template<class Func>
inline void multiset<classType>::SRDfunctor(node*& current, Func &functor)
{
	if (current != nullptr) {
		
		current->data = functor(current->data);

		SRDfunctor(current->left, functor);

		SRDfunctor(current->right, functor);
	}
}

template<class classType>
template<class Func>
inline void multiset<classType>::modify(Func &functor)
{
	SRDfunctor(this->root, functor);
}
