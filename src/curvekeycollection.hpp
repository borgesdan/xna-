#ifndef XNA_CURVEKEYCOLLECTION_HPP
#define XNA_CURVEKEYCOLLECTION_HPP

#include <vector>
#include "curvekey.hpp"
#include <algorithm>

namespace xna {
	class CurveKeyCollection {
	public:

		constexpr CurveKeyCollection() = default;

		constexpr double TimeRange() const { return timeRange; }
		constexpr void TimeRange(double value) { timeRange = value; }
		constexpr double InvTimeRange() const { return invTimeRange; }
		constexpr void InvTimeRange(double value) { invTimeRange = value; }
		constexpr bool IsCacheAvailable() const { return isCacheAvailable; }
		constexpr void IsCacheAvailable(bool value) { isCacheAvailable = value; }

		constexpr size_t IndexOf(CurveKey const& item) const {
			auto it = std::find(keys.begin(), keys.end(), item);
			size_t index = -1;

			if (it != keys.end())
				index = it - keys.begin();

			return index;
		}

		constexpr void RemoveAt(size_t index) {
			keys.erase(keys.begin() + index);
		}		

		constexpr CurveKey& Index(size_t index) { return keys[index]; }
		
		constexpr void Index(size_t index, CurveKey const& value) {
			if (keys[index].Position() == value.Position()) {
				keys[index] = value;
			}
			else {
				RemoveAt(index);
				Add(value);
			}
		}
		
		constexpr void Add(CurveKey const& item) {									
			size_t index = binarySearch(item);

			if (index >= 0)	{
				while (index < keys.size() && item.Position() == keys[index].Position())
					++index;
			}
			else
				index = ~index;

			keys.insert(keys.begin() + index, item);
			isCacheAvailable = false;
		}	

		constexpr void Clear() {
			keys.clear();
			timeRange = invTimeRange = 0.0;
			isCacheAvailable = false;
		}

		constexpr bool Contains(CurveKey const& item) {
			//keys.contains(item);
			auto finder = std::find(keys.begin(), keys.end(), item);
			return finder != keys.end();
		}

		constexpr void CopyTo(std::vector<CurveKey>& array, size_t arrayIndex) {			
			array = keys;
			isCacheAvailable = false;
		}

		constexpr size_t Count() const { return keys.size(); }

		constexpr bool IsReadOnly() const { return false; }

		constexpr bool Remove(CurveKey const& item) {
			isCacheAvailable = false;
			auto it = std::find(keys.begin(), keys.end(), item);
			keys.erase(it);			
			return false;
		}

		constexpr CurveKeyCollection Clone() const {
			CurveKeyCollection collection;
			collection.keys = keys;
			collection.invTimeRange = invTimeRange;
			collection.timeRange = timeRange;
			collection.isCacheAvailable = true;

			return collection;
		}

		constexpr void ComputeCacheValues()	{
			timeRange = invTimeRange = 0.0;
			
			if (keys.size() > 1) {
				timeRange = keys[keys.size() - 1].Position() - keys[0].Position();

				if (timeRange > 1.4012984643248171E-45)
					invTimeRange = 1.0 / timeRange;
			}
			
			isCacheAvailable = false;
		}
		
	private:
		std::vector<CurveKey> keys;
		double timeRange{ 0.0 };
		double invTimeRange{ 0.0 };
		bool isCacheAvailable{ true };

		//https://www.geeksforgeeks.org/binary-search/
		constexpr size_t binarySearch(CurveKey const& To_Find) const {
			size_t lo = 0;
			size_t hi = keys.size() - 1;
			size_t mid = 0;

			while (hi - lo > 1) {
				const auto mid = (hi + lo) / 2;

				if (CurveKey::BinaryCompare(keys[mid], To_Find)) {
					lo = mid + 1;
				}
				else {
					hi = mid;
				}
			}

			if (keys[lo] == To_Find) 
				return lo;
			else if (keys[hi] == To_Find) 
				return hi;
			
			return -1;
		}
	};
}

#endif