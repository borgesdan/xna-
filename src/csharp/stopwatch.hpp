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

		constexpr bool IsRunning() const { 
			return isRunning; 
		}

		constexpr cs::TimeSpan Elapsed() const { 
			return cs::TimeSpan(GetElapsedDateTimeTicks()); 
		}

		constexpr cslong ElapsedMilliseconds() const {
			return GetElapsedDateTimeTicks() / TicksPerMillisecond; 
		}

		cslong ElapsedTicks() const {
			return GetRawElapsedTicks(); 
		}

		static constexpr cslong GetTimestamp() {			
			//TODO: verificar Ticks
			//return DateTime.UtcNow.Ticks;					
			//Em .NET Core retorna (long)Interop.Sys.GetTimestamp();
			return std::chrono::utc_clock::now().time_since_epoch().count();
		}		

		constexpr cslong GetElapsedDateTimeTicks() const {
			const auto rawTicks = GetRawElapsedTicks();		
			return rawTicks;
		}

		static constexpr TimeSpan GetElapsedTime(cslong startingTimestamp) {
			return GetElapsedTime(startingTimestamp, GetTimestamp());
		}

		static constexpr cs::TimeSpan GetElapsedTime(cslong startingTimestamp, cslong endingTimestamp) {
			return TimeSpan(tolong((endingTimestamp - startingTimestamp) * tickFrequency));
		}


	public:
		static constexpr cslong SecondsToNanoSeconds = 1000000000;
		static constexpr cslong Frequency = SecondsToNanoSeconds;
		static constexpr bool IsHighResolutions{ true };

	private:
		static constexpr cslong TicksPerMillisecond{ 1000 };
		static constexpr cslong TicksPerSecond{ TicksPerMillisecond * 1000 };

		cslong elapsed{ 0 };
		cslong startTimeStamp{ 0 };
		bool isRunning{ false };	

		static constexpr double tickFrequency{ static_cast<double>(TicksPerSecond) / Frequency };

		constexpr cslong GetRawElapsedTicks() const {
			cslong timeElapsed = elapsed;

			if (isRunning) {
				const auto currentTimeStamp = GetTimestamp();
				const auto elapsedUntilNow = currentTimeStamp - startTimeStamp;
				timeElapsed += elapsedUntilNow;
			}

			return timeElapsed;
		}

		constexpr cslong GetElapsedDateTimeTicks() {
			return tolong(GetRawElapsedTicks() * tickFrequency);
		}
	
	};
}

#endif