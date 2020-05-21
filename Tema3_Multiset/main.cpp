#include "multiset.h"

int main()
{
	multiset<char> obj;
	obj.insert('a');
	obj.insert('b');
	obj.insert('c');
	multiset<char> copy(obj);
	copy.insert('a');
	copy.insert('c');
	copy.insert('d');
	obj = copy;
	std::cout << obj;
	obj.remove('a');
	std::cout << obj;
	std::cout << "count: "<<obj.elemCount()<<"\n";
	std::cout << "has 'a': " << obj.has('a') << "\n";
	std::cout << "has 'c': " << obj.has('c') << "\n";
	std::cout << "nr. of 'c': "<< obj.multiplicity('c')<< "\n";
	obj.pop('c');
	std::cout << obj;
	functorAdd<int> funct1(33);
	functorSub<char> funct2('!');
	obj.modify(funct1);
	std::cout << obj;
	obj.modify(funct2);
	std::cout << obj;
}
