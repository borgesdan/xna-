#ifndef XNA_GAMECLOCK_HPP
#define XNA_GAMECLOCK_HPP

#include "csharp/integralnumeric.hpp"
#include "csharp/timespan.hpp"
#include "csharp/stopwatch.hpp"

namespace xna {
	class GameClock {
	public:
		constexpr GameClock() {
			Reset();
		}

		constexpr void Reset() {
			currentTimeBase = cs::TimeSpan::Zero();
			currentTimeOffset = cs::TimeSpan::Zero();
			baseRealTime = Counter();
			lastRealTimeValid = false;
		}

		constexpr void UpdateElapsedTime() {
			cslong counter = Counter();

			if (!lastRealTimeValid) {
				lastRealTime = counter;
				lastRealTimeValid = true;
			}

			currentTimeOffset = CounterToTimeSpan(counter - baseRealTime);

			if (currentTimeOffset.HasOverflowException()) {
				currentTimeBase = currentTimeBase + currentTimeOffset;
				baseRealTime = lastRealTime;

				currentTimeOffset = CounterToTimeSpan(counter - baseRealTime);

				if (currentTimeOffset.HasOverflowException()) {
					baseRealTime = counter;
					currentTimeOffset = cs::TimeSpan::Zero();
				}
			}

			elapsedTime = CounterToTimeSpan(counter - lastRealTime);

			if (elapsedTime.HasOverflowException()) {
				elapsedTime = cs::TimeSpan::Zero();
			}

			cslong num = lastRealTime + timeLostToSuspension;
			elapsedAdjustedTime = CounterToTimeSpan(counter - num);

			if (elapsedAdjustedTime.HasOverflowException()) {
				elapsedAdjustedTime = cs::TimeSpan::Zero();
			}

			lastRealTimeCandidate = counter;
		}

		constexpr void AdvanceFrameTime() {
			lastRealTime = lastRealTimeCandidate;
			timeLostToSuspension = 0L;
		}

		constexpr void Suspend() {
			++suspendCount;

			if (suspendCount != 1)
				return;

			suspendStartTime = Counter();
		}

		constexpr void Resume() {
			--suspendCount;

			if (suspendCount > 0)
				return;

			timeLostToSuspension += Counter() - suspendStartTime;
			suspendStartTime = 0L;
		}

		static constexpr cslong Frequency() {
			return cs::Stopwatch::Frequency;
		}

		constexpr cs::TimeSpan CurrentTime() const {
			return currentTimeBase + currentTimeOffset;
		}

		constexpr cs::TimeSpan ElapsedTime() const {
			return elapsedTime;
		}

		constexpr cs::TimeSpan ElapsedAdjustedTime() const {
			return elapsedAdjustedTime;
		}

		static constexpr cslong Counter() {
			return cs::Stopwatch::GetTimestamp();
		}

		static constexpr cs::TimeSpan CounterToTimeSpan(cslong delta) {
			cslong num = 10000000;
			return cs::TimeSpan::FromTicks((delta * num) / Frequency());
		}

	private:
		cslong baseRealTime{ 0 };
		cslong lastRealTime{ 0 };
		bool lastRealTimeValid{ false };
		csint suspendCount{ 0 };
		cslong suspendStartTime{ 0 };
		cslong timeLostToSuspension{ 0 };
		cslong lastRealTimeCandidate{ 0 };
		cs::TimeSpan currentTimeOffset;
		cs::TimeSpan currentTimeBase;
		cs::TimeSpan elapsedTime;
		cs::TimeSpan elapsedAdjustedTime;
	};

	struct GameTime {
		cs::TimeSpan TotalGameTime{ cs::TimeSpan::Zero() };
		cs::TimeSpan ElapsedGameTime{ cs::TimeSpan::Zero() };
		bool IsRunningSlowly{ false };

		constexpr GameTime() = default;

		constexpr GameTime(cs::TimeSpan totalGameTime, cs::TimeSpan elapsedGameTime, bool isRunningSlowly = false) :
			TotalGameTime(totalGameTime),
			ElapsedGameTime(elapsedGameTime),
			IsRunningSlowly(isRunningSlowly) {
		}
	};
}

#endif