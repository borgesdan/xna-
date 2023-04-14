// xna++.cpp : Defines the entry point for the application.
//

#include "xna++.h"
#include "csharp/nullable.hpp"
#include "structs.hpp"
#include <array>
#include <cmath>
#include "mathhelper.hpp"
#include <algorithm>
#include "graphics/packedvector.hpp"
#include <chrono>
#include <ctime>
#include <any>
#include <type_traits>
#include "csharp/type.hpp"


using namespace std;
using namespace xna;
using namespace cs;

class Reader {
public:
	virtual any Read(any const& obj) {
		return obj;
	}
};

template <typename T>
class ReaderT : public Reader {
public:
	virtual any Read(any const& obj) override {
		auto t = any_cast<T>(obj);
		return Read(t);
	}

	virtual T Read(T const& obj) {
		return obj;
	}
};

class Ximbica : public ReaderT<int> {
public:
	virtual int Read(int const& obj) override {
		return obj * 10;
	}
};

int main()
{
	shared_ptr<Reader> r = make_shared<Ximbica>();

	int bilulu = 9865;
	auto bufa = any_cast<int>(r->Read(bilulu));	

	//std::is_array<int>::value;
	char* vec = new char[100];

	auto isArray = std::is_array<char[]>::value;

	auto tp = Type(vec);
	isArray = tp.IsArray();
	auto isPointer = tp.IsPointer();

	auto to = typeof<int[]>();
	isArray = to.IsArray();

	delete[] vec;

	return 0;
}
