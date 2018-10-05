#include <iostream>
#include <string>
#include <functional>
#include <boost/noncopyable.hpp>

namespace AOP {

	template<typename F>
	class Aspect : boost::noncopyable {
	private:
		F fun;
	public:
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

	template<typename F, typename... AP>
	inline void invoke(const F& fun) {
		Aspect<F>(fun).invoke(AP()...);
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
	AOP::invoke<std::function<void()>, A, B>([]{ std::cout << "-------\n\n"; });

	std::cout << "-------------- 2 -------------\n\n";
	D d;
	AOP::invoke<std::function<void()>, A>([&](){ d.turnFlag(); });

	std::cout << "-------------- 3 -------------\n\n";

	std::function<void()> fun_obj = std::bind(&D::turnFlag, d);

	AOP::invoke<std::function<void()>, A>([&fun_obj] { fun_obj(); });

	std::cout << "-------------- 4 -------------\n\n";

	std::function<void(std::string)> fun_obj2 = std::bind(&D::print, d, std::placeholders::_1);

	std::string message = "AOP 面向切面编程\n";

	AOP::invoke<std::function<void()>, A, B, C>([&message, &fun_obj2](){ fun_obj2(message); });

	std::cout << "-------------- 5 -------------\n\n";

	AOP::invoke<std::function<void()>, B, C>([&d, &message]{d.print(message); });

	
	return 0;
}