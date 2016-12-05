#pragma once

#include "std_.h"

// general utilities
namespace tests{
	template<typename RanIt>
	auto distance_impl(RanIt begin, RanIt end, tests::random_access_iterator_tag) {
		return end - begin;
	}

	template<typename InputIt>
	auto distance_impl(InputIt begin, InputIt end, tests::input_iterator_tag) {
		tests::iterator_traits<InputIt>::difference_type dist = 0;
		for (; begin != end; ++begin, ++dist);
		return dist;
	}

	template<typename InputIt>
	auto distance(InputIt begin, InputIt end) {
		return distance_impl(begin, end, tests::iterator_traits<InputIt>::iterator_category{});
	}

	template<typename RanIt, typename diff_type>
	void advance_impl(RanIt& it, const diff_type& n, tests::random_access_iterator_tag) {
		it += n;
	}

	template<typename InputIt, typename diff_type>
	void advance_impl(InputIt& it, const diff_type& n, tests::input_iterator_tag) {
		for (diff_type i = 0; i < n; ++i, ++it);
	}

	template<typename InputIt, typename diff_type>
	void advance(InputIt& it, const diff_type& n) {
		advance_impl(it, n, tests::iterator_traits<InputIt>::iterator_category{});
	}

	template<typename ForwardIt,
		typename diff_type = tests::iterator_traits<ForwardIt>::difference_type>
		ForwardIt next(ForwardIt it, diff_type diff = 1) {
		tests::advance(it, diff);
		return it;
	}

	template<typename T>
	struct less {
		constexpr auto operator()(const T& a, const T& b) const {
			return a < b;
		}
	};
}

// algorithms // binary search operations
namespace tests {
	template<typename ForwardIt, typename T, typename Compare>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last,
		const T& key, Compare comp) {
		auto dist = tests::distance(first, last);
		while (dist) {
			ForwardIt mid = first;
			tests::advance(mid, dist / 2);
			if (comp(*mid, key))
				first = tests::next(mid);
			else
				last = mid;

			dist = tests::distance(first, last);
		}
		return first;
	}

	template<typename ForwardIt, typename T>
	ForwardIt lower_bound(ForwardIt begin, ForwardIt end, const T& value) {
		return tests::lower_bound(begin, end, value, tests::less<T>{});
	}

	template<typename ForwardIt, typename T, typename Compare>
	ForwardIt upper_bound(ForwardIt first, ForwardIt last,
		const T& key, Compare comp) {
		auto dist = tests::distance(first, last);
		while (dist) {
			ForwardIt mid = first;
			tests::advance(mid, dist / 2);
			if (comp(key, *mid))
				last = mid;
			else
				first = tests::next(mid);

			dist = tests::distance(first, last);
		}
		return first;
	}

	template<typename ForwardIt, typename T>
	ForwardIt upper_bound(ForwardIt begin, ForwardIt end, const T& value) {
		return tests::upper_bound(begin, end, value, tests::less<T>{});
	}

	template<typename ForwardIt, typename T>
	tests::pair<ForwardIt, ForwardIt> equal_range(ForwardIt begin, ForwardIt end, 
		const T& value) {
		return{
			tests::lower_bound(begin, end, value, tests::less<T>{}),
			tests::upper_bound(begin, end, value, tests::less<T>{}),
		};
	}

	template<typename ForwardIt, typename T>
	bool binary_search(ForwardIt begin, ForwardIt end, const T& value) {
		auto it = tests::lower_bound(begin, end, value, tests::less<T>{});
		if (it == end)
			return false;
		return !(value < *it);
	}
}

// partition algorithms
namespace tests {
	template<typename ForwardIt, typename UnaryPredicate>
	ForwardIt partition_impl(ForwardIt begin, ForwardIt end, UnaryPredicate pred,
		tests::forward_iterator_tag) {
		ForwardIt first = std::find_if_not(begin, end, pred);
		if (first == end)
			return first;

		for (auto it = tests::next(first); it != end; ++it) {
			if (pred(*it)) {
				std::iter_swap(it, first);
				++first;
			}
		}
		return first;
	}

	template<typename BidIt, typename UnaryPredicate>
	BidIt partition_impl(BidIt begin, BidIt end, UnaryPredicate pred,
		tests::bidirectional_iterator_tag) {
		if (begin == end)
			return begin;

		auto it_first = begin;
		auto it_last = --end;
		while (1) {
			while (it_first != it_last && pred(*it_first))
				++it_first;
			while (it_last != it_first && !pred(*it_last))
				--it_last;

			if (it_first == it_last)
				break;

			std::iter_swap(it_first, it_last);
		}

		if (pred(*it_first))
			return tests::next(it_first);
		return it_first;
	}

	template<typename InputIt, typename OutputIt>
	OutputIt merge(InputIt first1, InputIt last1, InputIt first2, InputIt last2,
		OutputIt out_it) {
		for (; first1 != last1 && first2 != last2; ++out_it) {
			if (*first1 < *first2) {
				*out_it = *first1;
				++first1;
			}
			else {
				*out_it = *first2;
				++first2;
			}
		}

		out_it = std::copy(first1, last1, out_it);
		out_it = std::copy(first2, last2, out_it);

		return out_it;
	}

	template<typename ForwardIt>
	void merge_sort(ForwardIt first, ForwardIt last) {
		auto dist = tests::distance(first, last);
		if (dist <= 1)
			return;

		auto mid = tests::next(first, dist / 2);

		merge_sort(first, mid);
		merge_sort(mid, last);

		std::vector<tests::iterator_traits<ForwardIt>::value_type> temp;

		temp.resize(dist);

		tests::merge(first, mid, mid, last, temp.begin());

		std::copy(temp.begin(), temp.end(), first);
	}
}

// algorithms // partition operations
namespace tests {
	template<typename ForwardIt, typename UnaryPredicate>
	ForwardIt partition(ForwardIt begin, ForwardIt end, UnaryPredicate pred) {
		return tests::partition_impl(begin, end, pred,
			tests::iterator_traits<ForwardIt>::iterator_category{});
	}

	template<typename ForwardIt>
	void quick_sort(ForwardIt begin, ForwardIt end) {
		if (begin == end)
			return;

		auto pivot = *tests::next(begin, tests::distance(begin, end) / 2);
		auto middle1 = tests::partition(begin, end, [pivot](const auto& x) {
			return x < pivot; });
		auto middle2 = tests::partition(middle1, end, [pivot](const auto& x) {
			return !(x > pivot);
		});

		quick_sort(begin, middle1);
		quick_sort(middle2, end);
	}

	template<typename ForwardIt, typename UnaryPred>
	bool is_partitioned(ForwardIt begin, ForwardIt end, UnaryPred pred) {
		auto p = tests::partition_point(begin, end, pred);
		return std::find_if(p, end, pred) == end;
	}

	template<typename ForwardIt, typename UnaryPred>
	ForwardIt partition_point(ForwardIt begin, ForwardIt end, UnaryPred pred) {
		return std::find_if_not(begin, end, pred);
	}

	template<typename BidIt, typename UnaryPredicate>
	BidIt stable_partition(BidIt first, BidIt last, UnaryPredicate pred) {

	}
}
