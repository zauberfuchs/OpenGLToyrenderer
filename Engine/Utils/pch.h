#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

#include <thread>
#include <mutex>
#include <chrono>
#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <optional>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <span>
#include <any>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}