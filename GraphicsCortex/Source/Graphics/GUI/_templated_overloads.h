#pragma once
#include "Gui.h"


// StyleAttribute
template<typename T>
std::ostream& operator<<(std::ostream& stream, const StyleAttribute<T>& self) {
	if (self.use_persentage) {
		if (!self.persentage) {
			std::cout << "[Style Attribute Error] StyleAttribute::use_persentage is true but persentage value doesn't exist." << std::endl;
			assert(false);
		}
		stream << self.persentage.value().value << "% of " << Persentage::RespectedAttribute_to_string(self.persentage.value().attribute_type);
	}
	else {
		if (!self.value) {
			std::cout << "StyleAttribute value: NaN" << std::endl;
		}
		stream << self.value.value();
	}
	return stream;
}