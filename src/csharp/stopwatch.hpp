#ifndef CS_STOPWATCH_HPP
#define CS_STOPWATCH_HPP

#include "integralnumeric.hpp"
#include "timespan.hpp"
#include <chrono>

namespace cs {
	class Stopwatch {
	
	public:
		constexpr Stopwatch() {
			Reset();
		}

		constexpr void Start() {			
			if (!isRunning) {
				startTimeStamp = GetTimestamp();
				isRunning = true;
			}
		}

		static Stopwatch StartNew() {
			Stopwatch s;
			s.Start();
			return s;
		}

		constexpr void Stop() {						
			if (isRunning) {
				const auto endTimeStamp = GetTimestamp();
				const cslong elapsedThisPeriod = endTimeStamp - startTimeStamp;
				elapsed += elapsedThisPeriod;
				isRunning = false;

				if (elapsed < 0) {
					elapsed = 0;
				}
			}
		}

		constexpr void Reset() {
			elapsed = 0;
			isRunning = false;
			startTimeStamp = 0;
		}

		constexpr void Restart() {
			elapsed = 0;
			startTimeStamp = GetTimestamp();
			isRunning = true;
		}

		bool IsRunning() const { return isRunning; }
		cs::TimeSpan Elapsed() const { return cs::TimeSpan(GetElapsedDateTimeTicks()); }
		long ElapsedMilliseconds() const { return GetElapsedDateTimeTicks() / TicksPerMillisecond; }
		long ElapsedTicks() const { return GetRawElapsedTicks(); }

		static constexpr cslong GetTimestamp() {			
			//TODO: verificar Ticks
			//return DateTime.UtcNow.Ticks;					
			return std::chrono::utc_clock::now().time_since_epoch().count();
		}

		constexpr cslong GetRawElapsedTicks() const {
			cslong timeElapsed = elapsed;

			if (isRunning) {				
				const auto currentTimeStamp = GetTimestamp();
				const auto elapsedUntilNow = currentTimeStamp - startTimeStamp;
				timeElapsed += elapsedUntilNow;
			}

			return timeElapsed;
		}

		constexpr cslong GetElapsedDateTimeTicks() const {
			const auto rawTicks = GetRawElapsedTicks();		
			return rawTicks;
		}

	private:
		static constexpr cslong TicksPerMillisecond{ 1000 };
		static constexpr cslong TicksPerSecond{ TicksPerMillisecond * 1000 };

		cslong elapsed{ 0 };
		cslong startTimeStamp{ 0 };
		bool isRunning{ 0 };	

		static constexpr double tickFrequency{ 1 };

	public:
		static constexpr cslong Frequency{ TicksPerSecond };
		static constexpr bool IsHighResolutions { false };
	};
}

#endif