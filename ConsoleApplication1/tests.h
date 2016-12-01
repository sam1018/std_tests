#include <windows.h>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <class Function>
__int64 time_call(Function&& f)
{
	__int64 begin = GetTickCount();
	f();
	return GetTickCount() - begin;
}

namespace tests {
	template<typename iter_type, typename key_type>
	iter_type lower_bound_rec(iter_type begin, iter_type end, const key_type& key)
	{
		auto dist = distance(begin, end);

		if (dist <= 0)
			return begin;

		iter_type mid = begin;
		advance(mid, dist / 2);

		if (*mid < key)
			return tests::lower_bound(mid + 1, end, key);

		return tests::lower_bound(begin, mid, key);
	}

	template<typename iter_type, typename key_type>
	iter_type lower_bound(iter_type begin, iter_type end, const key_type& key)
	{
		while (distance(begin, end) > 0)
		{
			iter_type mid = begin + distance(begin, end) / 2;

			if (*mid < key)
				begin = mid + 1;
			else
				end = mid;
		}

		return begin;
	}

	template<typename iter_type, typename key_type>
	iter_type upper_bound(iter_type begin, iter_type end, const key_type& key)
	{
		while (distance(begin, end) > 0)
		{
			iter_type mid = begin + distance(begin, end) / 2;

			if (key < *mid)
				end = mid;
			else
				begin = mid + 1;
		}

		return begin;
	}

	template<typename iter_type, typename key_type>
	iter_type upper_bound_rec(iter_type begin, iter_type end, const key_type& key)
	{
		auto dist = distance(begin, end);

		if (dist <= 0)
			return begin;

		iter_type mid = begin;
		advance(mid, dist / 2);

		if (key < *mid)
			return tests::upper_bound(begin, mid, key);

		return tests::upper_bound(mid + 1, end, key);
	}

	template<typename iter_type, typename key_type>
	bool binary_search(iter_type begin, iter_type end, const key_type& key)
	{
		iter_type res = tests::lower_bound(begin, end, key);

		if (res == end)
			return false;

		return !(key < *res);
	}
}

struct res
{
	vector<int>::iterator iter1;
	vector<int>::iterator iter2;
	bool search_res;

	res(vector<int>::iterator _iter1, vector<int>::iterator _iter2, bool _search_res) :
		iter1(_iter1), iter2(_iter2), search_res(_search_res)
	{}
};

bool operator==(const res& a, const res& b)
{
	bool ret = a.iter1 == b.iter1 && a.iter2 == b.iter2 && a.search_res == b.search_res;

	if (!ret)
	{
		int x = 10;
	}

	return ret;
}

int main()
{
	vector<int> v;
	int vals_count = 100'000;
	int queries_count = 10'000'000;

#ifdef _DEBUG
	vals_count /= 1000;
	queries_count /= 1000;
#endif

	for (int i = 0; i < vals_count; ++i)
		v.push_back(rand() % queries_count);

	sort(v.begin(), v.end());

	vector<int> queries;

	for (int i = 0; i < queries_count; ++i)
		queries.push_back(rand() % queries_count);

	vector<res> std_res;

	auto std_test = [&queries, &std_res, &v]() {
		for_each(queries.begin(), queries.end(), [&std_res, &v](int x) {
			std_res.emplace_back(
				lower_bound(v.begin(), v.end(), x),
				upper_bound(v.begin(), v.end(), x),
				binary_search(v.begin(), v.end(), x));
		});
	};

	auto std_time = time_call(std_test);

	cout << "std_time: " << std_time << "ms\n";



	vector<res> tests_res;

	auto tests_test = [&queries, &tests_res, &v]() {
		for_each(queries.begin(), queries.end(), [&tests_res, &v](int x) {
			tests_res.emplace_back(
				tests::lower_bound(v.begin(), v.end(), x),
				tests::upper_bound(v.begin(), v.end(), x),
				tests::binary_search(v.begin(), v.end(), x));
		});
	};

	auto tests_time = time_call(tests_test);

	cout << "std_time: " << tests_time << "ms\n";

	if (std_res == tests_res)
		cout << "OK...";
	else
		cout << "Failed...";

	auto matches = count_if(std_res.begin(), std_res.end(), [](auto& x) {
		return x.search_res == true;
	});

	cout << "\nMatch: " << matches << ", no match: " << std_res.size() - matches << "\n";

	auto test_matches = count_if(tests_res.begin(), tests_res.end(), [](auto& x) {
		return x.search_res == true;
	});

	cout << "\nMatch: " << test_matches << ", no match: " << tests_res.size() - test_matches << "\n";
}
