// xna++.cpp : Defines the entry point for the application.
//

#include "xna++.h"
#include "csharp/nullable.hpp"
#include "basic-structs.hpp"
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
#include "utilities/stringhelper.hpp"
#include "input/keyboard.hpp"

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
	for (int i = 8; i < 255; ++i) {

		auto key = static_cast<Keys>(i);
		cout << (int)key << endl;

		if (i == 9)
			i = 12;
		else if (i == 13)
			i == 19;
		else if (i == 20)
			i == 26;
		else if (i == 26)
			i == 31;
	}
	

	return 0;
}
