#pragma once

#include <iterator>

namespace tests {
	using forward_iterator_tag = std::forward_iterator_tag;
	using random_access_iterator_tag = std::random_access_iterator_tag;
	using bidirectional_iterator_tag = std::bidirectional_iterator_tag;
	using input_iterator_tag = std::input_iterator_tag;
	template<typename T>
	using iterator_traits = std::iterator_traits<T>;
	template<typename T, typename U>
	using pair = std::pair<T, U>;
}
