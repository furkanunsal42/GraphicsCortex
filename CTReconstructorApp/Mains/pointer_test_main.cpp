#include <memory>
#include <iostream>

class my_class {
public:
	my_class() {
		std::cout << "Contructor" << std::endl;
	}

	~my_class() {
		std::cout << "Destructor" << std::endl;
	}
};

int main() {
	
	{
		my_class* variable = new my_class();

		std::shared_ptr<my_class> shared_variable(variable);
	}


	std::cin.get();
}