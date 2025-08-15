#pragma once
#include <cinttypes>

class gpu_arithmatic_type {
	int32_t variable_id;
};

class gpu_bool : public gpu_arithmatic_type {
	friend gpu_expression<gpu_bool> operator+(gpu_expression<gpu_bool> right, gpu_expression<gpu_bool> left);

};

class gpu_int32_t : public gpu_arithmatic_type {
};

class gpu_float : public gpu_arithmatic_type {
};

template<typename return_type>
class gpu_expression {
public:
	gpu_expression(const return_type& variable);

	int32_t expression_id_left;
	int32_t expression_id_right;
	int32_t expression_operator;
};

template<typename t>
gpu_expression<t> operator+(gpu_expression<t> right, gpu_expression<t> left);

class _gpu_if {
public:
	_gpu_if(gpu_expression<gpu_bool> expression);
	_gpu_if();
};

#define gpu_if(x) _gpu_if(x);

class gpu_main_function {

};

class gpu_function {

};

class _gpu_return {

};

#define gpu_return(x) _gpu_return(x); return x

class gpu_kernel {

};

gpu_bool func(gpu_float a, gpu_float b) {
	gpu_function function;

	gpu_float result = a + b;

	gpu_return(result);
}

void compute(float cpu_var0, float cpu_var1) {
	gpu_kernel kernel;

	gpu_float a = cpu_var0;
	gpu_float b = cpu_var1;

	gpu_main_function main; {

		gpu_bool equals = func(a, b);

		gpu_if(equals) {

		}
	}
}



namespace gpu {
	
	class Library {
	public:
		Library& get();
		void release();

	private:
		Library();
	};
}