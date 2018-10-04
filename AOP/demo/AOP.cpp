#include <iostream>
#include <string>
#include <functional>
#include <boost/noncopyable.hpp>

namespace AOP {

	class Aspect : boost::noncopyable {
	private:
		std::function<void()> fun;
	public:
		template<typename F>
		Aspect(const F& _fun) : fun(_fun)
			{}

		template<typename T>
		void invoke(T&& object) {

			object.before();

			fun();

			object.after();
		}

		template<typename Head, typename... Tail>
		void invoke(Head&& head, Tail&&... tail) {

			head.before();

			invoke(std::forward<Tail>(tail)...);

			head.after();
		}
	};

	template<typename... AP>
	inline void invoke(const std::function<void()> fun) {
		Aspect(fun).invoke(AP()...);
	}

	template<typename... AP>                  // typename T, 
	inline void invoke(const std::function<int()> fun) {  // T()
		Aspect(fun).invoke(AP()...);
	}

	template<typename... AP>                  // typename T, 
	inline void invoke(const std::function<std::string()> fun) {  // T()
		Aspect(fun).invoke(AP()...);
	}

	template<typename... AP>
	inline void invoke(const std::function<void(int)> fun, const int arg) {
		auto _fun = std::bind(&fun, arg);
		Aspect(_fun).invoke(AP()...);
	}

	template<typename... AP>
	inline void invoke(const std::function<void(std::string)> fun, const std::string arg) {
		auto _fun = std::bind(&fun, arg);
		Aspect(_fun).invoke(AP()...);
	}

}

namespace test {

	struct A {
		void before() {
			std::cout << "before  in  A\n\n";
		}
		void after() {
			std::cout << "after  in  A\n\n";
		}
	};

	struct B {
		void before() {
			std::cout << "before  in  B\n\n";
		}
		void after() {
			std::cout << "after  in  B\n\n";
		}
	};

	struct C {
		void before() {
			std::cout << "before  in  C\n\n";
		}
		void after() {
			std::cout << "after  in  C\n\n";
		}
	};

	struct D {
		bool flag = false;
		void turnFlag() {
			flag = !flag;
			std::cout << std::boolalpha;
			std::cout << "flag  :  " << flag << "\n\n";
		}

		void print(const std::string& message) {
			std::cout << message << std::endl;
		}
	};
} 

int main() {

	using namespace test;

	std::cout << "-------------- 1 -------------\n\n";
	AOP::invoke<A, B>([]{ std::cout << "-------\n\n"; });

	std::cout << "-------------- 2 -------------\n\n";
	D d;
	AOP::invoke<A>([&](){ d.turnFlag(); });

	std::cout << "-------------- 3 -------------\n\n";

	std::function<void()> fun_obj = std::bind(&D::turnFlag, d);

	AOP::invoke<A>([&fun_obj] { fun_obj(); });

	std::cout << "-------------- 4 -------------\n\n";

	std::function<void(std::string)> fun_obj2 = std::bind(&D::print, d, std::placeholders::_1);

	std::string message = "AOP 面向切面编程\n";

	AOP::invoke<A, B, C>([&message, &fun_obj2](){ fun_obj2(message); });

	std::cout << "-------------- 5 -------------\n\n";

	AOP::invoke<B, C>([&d, &message]{d.print(message); });

	std::cout << "-------------- 6 -------------\n\n";

	AOP::invoke<B, C>([](int arg = 10){ std::cout << "arg = " << arg << "\n\n"; });

	// std::cout << "-------------- 7 -------------\n\n";

	// AOP::invoke<B, C>([&]{ return std::bind(&D::print, d, std::placeholders::_1); }, message);
	return 0;
}