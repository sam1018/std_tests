#include <iostream>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <list>
#include <forward_list>

using namespace std;

vector<int> v2;


struct a_struct {
	int x;
	int y;
	a_struct(int _x, int _y) : x(_x), y(_y) {}
};

bool operator==(const a_struct& a, const a_struct& b) {
	return a.x == b.x && a.y == b.y;
}

namespace tests {
	template<typename ForwardIt, typename UnaryPredicate>
	ForwardIt partition_impl(ForwardIt first, ForwardIt last, UnaryPredicate pred, std::forward_iterator_tag) {
		auto it1 = std::find_if_not(first, last, pred);

		if (it1 == last)
			return last;

		for (auto it2 = std::next(it1); it2 != last; ++it2) {
			if (pred(*it2)) {
				std::iter_swap(it1, it2);
				++it1;
			}
		}

		return it1;
	}

	template<typename InputIt, typename UnaryPredicate>
	InputIt partition(InputIt first, InputIt last, UnaryPredicate pred) {
		return tests::partition_impl(first, last, pred, std::iterator_traits<InputIt>::iterator_category{});
	}

	template<typename BidIt, typename UnaryPredicate>
	BidIt stable_partition(BidIt begin, BidIt end, UnaryPredicate pred) {

	}

	//template<typename RandIt>
	//void rotate(RandIt first, RandIt n_first, RandIt last) {
	//	auto first_stored = first;
	//	auto n_last = std::prev(n_first);
	//	for (; n_first != n_last; ) {
	//		std::iter_swap(first, n_first);
	//		++first;
	//		++n_first;
	//		if (n_first == last)
	//			n_first = first_stored;
	//	}
	//}

	void rotate(int first, int n_first, int last)
	{
		int next = n_first;
		while (first != next) {
			std::iter_swap(&v2[first++], &v2[next++]);
			if (next == last) {
				next = n_first;
			}
			else if (first == n_first) {
				n_first = next;
			}
		}
	}
}

template<typename C>
void add(C& v, const a_struct& x) {
	v.push_back(x);
}

void add(forward_list<a_struct>& c, const a_struct& x) {
	c.push_front(x);
}

template<typename C>
void partition_test() {
	C v1, v2;
	for (int i = 0; i < 1000; ++i) {
		int x = rand(), y = rand();
		add(v1, { x, y });
		add(v2, { x, y });
	}

	auto pred = [](const auto& a) {return a.x % 2; };

	tests::partition(v1.begin(), v1.end(), pred);

	assert(std::is_partitioned(v1.begin(), v1.end(), pred));
}

template<typename C>
void stable_partition_test() {
	C v1, v2;
	for (int i = 0; i < 1000; ++i) {
		int x = rand(), y = rand();
		add(v1, { x, y });
		add(v2, { x, y });
	}

	auto pred = [](const auto& a) {return a.x % 2; };

	std::stable_partition(v2.begin(), v2.end(), pred);
}

int main() {
	partition_test<vector<a_struct>>();
	partition_test<list<a_struct>>();
	partition_test<forward_list<a_struct>>();

	stable_partition_test<vector<a_struct>>();

	vector<int> v1;
	for (int i = 0; i < 11; ++i) {
		int x = i;
		v1.push_back(x);
		v2.push_back(x);
	}

	std::rotate(v1.begin(), v1.begin() + 1, v1.end());
	tests::rotate(0, 1, v2.size());

	for (int i = 0; i < v1.size(); ++i)
		assert(v1[i] == v2[i]);

	assert(v1 == v2);
}
