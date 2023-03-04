#pragma once
#include "Config.h"

#include <memory>
#include <type_traits>

template<typename T>
class SharedPtr{
public:

	SharedPtr(std::shared_ptr<T> other_obj) :
		obj(other_obj) {}

	explicit SharedPtr(T& other) :
		obj(std::make_shared<T>(other)) {}

	explicit SharedPtr(T&& other) :
		obj(std::make_shared<T>(other)) {}

	T* operator->() const { return obj.get();  }

	std::shared_ptr<T> obj;
};

#define _SHARED_POINTER_DEFINITIONS(type)				\
	type##_s(std::shared_ptr<type> other_obj) :			\
		obj(other_obj) {}								\
														\
	explicit type##_s(type& other) :					\
		obj(std::make_shared<type>(other)) {}			\
														\
	explicit type##_s(type&& other) :					\
		obj(std::make_shared<type>(other)) {}			\
														\
	type* operator->() const { return obj.get(); }		\
														\
	std::shared_ptr<type> obj;			


class PxRigidActor;									// physx		
typedef SharedPtr<PxRigidActor> PxRigidActor_s;		// physx

