#include "Header.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <list>
#include <forward_list>
#include <random>
#include <sstream>

template<typename C, typename T>
void check(const C& c, const T& val_begin, const T& val_end) {
	for (T x = val_begin; x < val_end; ++x) {
		assert(std::lower_bound(c.begin(), c.end(), x)
			== tests::lower_bound(c.begin(), c.end(), x));

		assert(std::upper_bound(c.begin(), c.end(), x)
			== tests::upper_bound(c.begin(), c.end(), x));

		assert(std::equal_range(c.begin(), c.end(), x)
			== tests::equal_range(c.begin(), c.end(), x));

		assert(std::binary_search(c.begin(), c.end(), x)
			== tests::binary_search(c.begin(), c.end(), x));
	}

	C test1(c);
	C test2(c);

	auto pred = [](auto x) {return x % 2; };
	auto x1 = std::distance(std::partition(test1.begin(), test1.end(), pred), test1.end());
	auto x2 = std::distance(tests::partition(test2.begin(), test2.end(), pred), test2.end());
	assert(x1 == x2);
	// only partially ordered...
	//assert(test1 == test2);
}

template<typename C, typename T>
struct add_impl {
	static void add(C& c, T&& item, bool sort_items) {
		static_assert(false, "No generic container implementation");
	}
};

template<typename T>
struct add_impl<std::vector<T>, T> {
	static void add(std::vector<T>& c, T&& item) {
		c.push_back(std::forward<T>(item));
	}
	static void sort(std::vector<T>& c, T&& item) {
		std::sort(c.begin(), c.end());
	}
};

template<typename T>
struct add_impl<std::list<T>, T> {
	static void add(std::list<T>& c, T&& item) {
		c.push_back(std::forward<T>(item));
	}
	static void sort(std::list<T>& c, T&& item) {
		c.sort();
	}
};

template<typename T>
struct add_impl<std::forward_list<T>, T> {
	static void add(std::forward_list<T>& c, T&& item) {
		c.push_front(std::forward<T>(item));
	}
	static void sort(std::forward_list<T>& c, T&& item) {
		c.sort();
	}
};

template<typename C, typename T>
void add(C& c, T&& item, bool sort_items = true) {
	add_impl<C, T>::add(c, std::forward<T>(item));
	if(sort_items)
		add_impl<C, T>::sort(c, std::forward<T>(item));
}

template<typename C, typename T>
void run_tests() {
	C v;

	check(v, T(-2), T(2));

	add(v, T(1));

	check(v, T(-5), T(5));

	add(v, T(1));

	check(v, T(-5), T(5));

	for (int i = 0; i < 10; ++i)
		add(v, T(1));

	check(v, T(-5), T(5));

	for (int i = 0; i < 10; ++i)
		add(v, T(2));

	check(v, T(-5), T(5));

	for (int i = 0; i < 100; ++i)
		add(v, T(rand() % 200));

	check(v, T(-5), T(300));
}

struct test_type {
	std::string s;
	int d;

	test_type(int _d) : s("default"), d(_d) {}
	test_type& operator++() {
		++d;
		return *this;
	}

	int operator%(int x) {
		return d % x;
	}
};

bool operator<(const test_type& a, const test_type& b) {
	return a.d < b.d;
}

bool operator==(const test_type& a, const test_type& b) {
	return a.d == b.d;
}

template<typename C>
void sort_test(int size){
	{
		C v;

		for (int i = 0; i < size; ++i) {
			add(v, rand(), 0);
			tests::quick_sort(v.begin(), v.end());
			assert(std::is_sorted(v.begin(), v.end()));
		}
	}

	{
		C v;

		for (int i = 0; i < size; ++i) {
			add(v, rand(), 0);
			tests::merge_sort(v.begin(), v.end());
			assert(std::is_sorted(v.begin(), v.end()));
		}
	}
}

template<typename C>
void merge_test() {
	C v1, v2;

	int size1 = rand() % 30;
	for (int i = 0; i < size1; ++i)
		add(v1, rand());

	int size2 = rand() % 30;
	for (int i = 0; i < size2; ++i)
		add(v2, rand());

	tests::quick_sort(v1.begin(), v1.end());
	tests::quick_sort(v2.begin(), v2.end());

	std::vector<int> res1, res2;

	std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(res1));
	tests::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(res2));

	assert(res1 == res2);
}

void merge_test(std::input_iterator_tag) {
	std::string a{"123456789"};

	std::stringstream res1;
	std::stringstream res2;

	{
		std::stringstream ss1{ a };
		std::stringstream ss2{ a };

		std::merge(
			std::istream_iterator<char>{ss1}, std::istream_iterator<char>{},
			std::istream_iterator<char>{ss2}, std::istream_iterator<char>{},
			std::ostream_iterator<char>{res1});
	}
	{
		std::stringstream ss1{ a };
		std::stringstream ss2{ a };

		tests::merge(
			std::istream_iterator<char>{ss1}, std::istream_iterator<char>{},
			std::istream_iterator<char>{ss2}, std::istream_iterator<char>{},
			std::ostream_iterator<char>{res2});
	}

	std::string a1 = res1.str();
	std::string b1 = res2.str();

	assert(res1.str() == res2.str());
}

void run_tests() {
	run_tests<std::vector<int>, int>();
	run_tests<std::list<int>, int>();
	run_tests<std::forward_list<int>, int>();

	run_tests<std::vector<test_type>, test_type>();
	run_tests<std::list<test_type>, test_type>();
	run_tests<std::forward_list<test_type>, test_type>();

	merge_test<std::vector<int>>();
	merge_test<std::list<int>>();
	merge_test<std::forward_list<int>>();
	merge_test(std::input_iterator_tag{});

	sort_test<std::vector<int>>(1000);
	sort_test<std::list<int>>(500);
	sort_test<std::forward_list<int>>(500);
}

int main() {
	run_tests();
}
