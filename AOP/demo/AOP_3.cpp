#include <iostream>
#include <memory>
#include <functional>

template<typename T, typename Derived>
class Aspect {
private:
	T* ptr;
	Derived* cast() {
		return static_cast<Derived*>(this);
	}
public:
	explicit Aspect(T *_ptr) : ptr(_ptr) {}

	void before_wrapper(T *p) {

		// pre 

		this->cast()->before(p);

		// then
	}

	void after_wrapper(T *p) {

		// pre

		this->cast()->after(p);

		// then
	}

	std::shared_ptr<T> operator->() {

		before_wrapper(ptr);

		return std::shared_ptr<T>(ptr, [this](T *p){ this->after_wrapper(p); }); 
	}
};

template<template <typename> class aspect, typename T>
inline aspect<T> make_aspect(T *ptr) {
	return aspect<T>(ptr);
}

template<typename T>
class log_aspect : public Aspect<T, log_aspect<T> > {

	using base_type = Aspect<T, log_aspect<T> >;
public:

	log_aspect(T *ptr) : base_type(ptr){}

	void before(T *ptr) {
		std::cout << "before.....\n\n";
	}
	
	void after(T *ptr) {
		std::cout << "after......\n\n";
	}
};

int main() {

	struct example {
		void test() {
			std::cout << "---------------\n\n";
		}
	} ;

	std::shared_ptr<example> ptr(new example());
	make_aspect<log_aspect>(ptr.get())->test();
	return 0;
}