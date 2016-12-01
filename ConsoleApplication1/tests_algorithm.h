#include <assert.h>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

namespace tests {
	template<typename InputIt, typename UnaryPredicate>
	bool all_of(InputIt begin, InputIt end, UnaryPredicate pred) {
		for (InputIt i = begin; i != end; ++i)
			if (!pred(*i))
				return false;
		return true;
	}

	template<typename InputIt, typename UnaryPredicate>
	bool any_of(InputIt begin, InputIt end, UnaryPredicate pred) {
		for (InputIt i = begin; i != end; ++i)
			if (pred(*i))
				return true;
		return false;
	}

	template<typename InputIt, typename UnaryPredicate>
	bool none_of(InputIt begin, InputIt end, UnaryPredicate pred) {
		for (InputIt i = begin; i != end; ++i)
			if (!pred(*i))
				return false;
		return true;
	}

	template<typename InputIt, typename UnaryFunction>
	void for_each(InputIt begin, InputIt end, UnaryFunction f) {
		for (InputIt it = begin; it != end; ++it)
			f(*it);
	}

	template<typename InputIt, typename UnaryFunction>
	void for_each_n(InputIt begin, InputIt end, UnaryFunction f, int count) {
		for (InputIt it = begin; it != end && distance(begin, it) + 1 <= count; ++it)
			f(*it);
	}

	template<typename InputIt, typename value_type>
	typename iterator_traits<InputIt>::difference_type
		count(InputIt begin, InputIt end, const value_type& val) {
		typename iterator_traits<InputIt>::difference_type res = 0;
		for (auto it = begin; it != end; ++it)
			if (static_cast<bool>(*it == val))
				++res;

		return res;
	}

	template<typename InputIt, typename UnaryPredicate>
	typename iterator_traits<InputIt>::difference_type
		count_if(InputIt begin, InputIt end, UnaryPredicate pred) {
		typename iterator_traits<InputIt>::difference_type res = 0;
		for (auto it = begin; it != end; ++it)
			if (pred(*it))
				++res;

		return res;
	}

	template<typename InputIt, typename UnaryOperation>
	void transform(InputIt in_begin, InputIt in_end, InputIt out_begin, 
		UnaryOperation unary_op) {
		for (auto it = in_begin; it != in_end; ++it, ++out_begin)
			*out_begin = unary_op(*it);
	}

	template<typename T, size_t N>
	constexpr T* begin(T (&a)[N]) {
		return a;
	}

	template<typename T, size_t N>
	constexpr T* end(T(&a)[N]) {
		return a + N;
	}

	template<typename C>
	constexpr auto begin(C& c) {
		return c.begin();
	}

	template<typename C>
	constexpr auto end(C& c) {
		return c.end();
	}
}

void tests_algorithm()
{
	vector<int> v;

	for (int i = 0; i < 10; ++i)
		v.push_back(rand());

	auto pred = [](int x) {return x % 2; };

	assert(std::all_of(v.begin(), v.end(), pred) == tests::all_of(v.begin(), v.end(), pred));
	assert(std::any_of(v.begin(), v.end(), pred) == tests::any_of(v.begin(), v.end(), pred));
	assert(std::none_of(v.begin(), v.end(), pred) == tests::none_of(v.begin(), v.end(), pred));

	int x1 = 0, x2 = 0;
	std::for_each(v.begin(), v.end(), [&x1](int x) {x1 += x; });
	tests::for_each(v.begin(), v.end(), [&x2](int x) {x2 += x; });

	assert(x1 == x2);

	x2 = 0;
	vector<int> v2{ 1, 2, 3, 4 };
	tests::for_each_n(v2.begin(), v2.end(), [&x2](int x) {x2 += x; }, 2);
	//cout << x2 << "\n";

	assert(count(v2.begin(), v2.end(), 2) == tests::count(v2.begin(), v2.end(), 2));

	assert(count_if(v2.begin(), v2.end(), pred) == tests::count_if(v2.begin(), v2.end(), pred));

	v2.clear();
	vector<int> v1;
	for (int i = 0; i < 10; ++i) {
		v1.push_back(i);
		v2.push_back(i);
	}

	auto square = [](int x) { return x * x; };

	std::transform(v1.begin(), v1.end(), v1.begin(), square);
	tests::transform(v2.begin(), v2.end(), v2.begin(), square);

	assert(v1 == v2);

	int a[10];

	assert(std::begin(a) == tests::begin(a));
	assert(std::end(a) == tests::end(a));

	assert(std::begin(v) == tests::begin(v));
	assert(std::end(v) == tests::end(v));
}
