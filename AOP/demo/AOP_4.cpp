#include <iostream>
#include <string>
#include <functional>
#include <boost/noncopyable.hpp>

namespace SFINAE {
    // 定义判断类型
    typedef char yes ;
    typedef struct{ char str[2] ; } no ;

    // 判断 T 是否有 bofore
    template<typename T, typename returnValue, typename ...Args>
    struct has_before {
        template<typename other, returnValue (other::*)(Args...)>
        struct help ;
        template<typename other>
        static yes judge(help<other, &other::before> *) ;
        template<typename other>
        static no judge(...) ;
        const static bool value = sizeof(judge<T>(0)) == sizeof(yes) ;
    } ;

    // 判断 T 是否有 after
    template<typename T, typename returnValue, typename ...Args>
    struct has_after {
        template<typename other, returnValue (other::*)(Args...)>
        struct help ;
        template<typename other>
        static yes judge(help<other, &other::after> *) ;
        template<typename other>
        static no judge(...) ;
        const static bool value = sizeof(judge<T>(0)) == sizeof(yes) ;
    } ;
}

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

			if(SFINAE::has_before<T, void>::value == true)
				object.before();

			fun();

			if(SFINAE::has_after<T, void>::value == true)
				std::cout << "yes\n";
				// object.after();
			else std::cout << "no\n";
		}

		template<typename Head, typename... Tail>
		void invoke(Head&& head, Tail&&... tail) {

			if(SFINAE::has_before<Head, void>::value == true)
				head.before();

			invoke(std::forward<Tail>(tail)...);

			if(SFINAE::has_after<Head, void>::value == true)
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

	struct E {
		void before() {
			std::cout << "before  in  E\n\n";
		}
	};
} 

int main() {

	using namespace test;

	AOP::invoke<std::function<void()>, E>([]{ std::cout << "AOP\n\n"; });


	// std::cout << "-------------- 1 -------------\n\n";
	// AOP::invoke<std::function<void()>, A, B>([]{ std::cout << "-------\n\n"; });

	// std::cout << "-------------- 2 -------------\n\n";
	// D d;
	// AOP::invoke<std::function<void()>, A>([&](){ d.turnFlag(); });

	// std::cout << "-------------- 3 -------------\n\n";

	// std::function<void()> fun_obj = std::bind(&D::turnFlag, d);

	// AOP::invoke<std::function<void()>, A>([&fun_obj] { fun_obj(); });

	// std::cout << "-------------- 4 -------------\n\n";

	// std::function<void(std::string)> fun_obj2 = std::bind(&D::print, d, std::placeholders::_1);

	// std::string message = "AOP 面向切面编程\n";

	// AOP::invoke<std::function<void()>, A, B, C>([&message, &fun_obj2](){ fun_obj2(message); });

	// std::cout << "-------------- 5 -------------\n\n";

	// AOP::invoke<std::function<void()>, B, C>([&d, &message]{d.print(message); });

	
	return 0;
}