#ifndef CS_TIMESPAN_HPP
#define CS_TIMESPAN_HPP

#include <stdint.h>
#include <limits>
#include "integralnumeric.hpp"

namespace cs {
	struct TimeSpan {
		static constexpr cslong TicksPerMillisecond{ 10000 };
		static constexpr cslong TicksPerSecond{ TicksPerMillisecond * 1000 };
		static constexpr cslong TicksPerMinute{ TicksPerSecond * 60	};
		static constexpr cslong TicksPerHour{ TicksPerMinute * 60 };
		static constexpr cslong TicksPerDay{ TicksPerHour * 24 };				

		constexpr TimeSpan() = default;
		constexpr TimeSpan(cslong ticks) :
			_ticks(ticks) {}
		constexpr TimeSpan(csint hours, csint minutes, csint seconds) :
			_ticks(TimeToTicks(hours, minutes, seconds)) {}
		constexpr TimeSpan(csint days, csint hours, csint minutes, csint seconds) :
			_ticks(DayToTicks(days, hours, minutes, seconds, 0)) {}
		constexpr TimeSpan(csint days, csint hours, csint minutes, csint seconds, csint milliseconds) :
			_ticks(DayToTicks(days, hours, minutes, seconds, milliseconds)) {}

		constexpr TimeSpan operator -(TimeSpan const& t) {
			return TimeSpan(-t._ticks);
		}

		constexpr TimeSpan operator +(TimeSpan const& t) {
			return t;
		}

		constexpr friend TimeSpan operator +(TimeSpan const& t1, TimeSpan const& t2) {
			return t1.Add(t2);
		}

		constexpr friend TimeSpan operator -(TimeSpan const& t1, TimeSpan const& t2) {
			return t1.Subtract(t2);
		}

		constexpr friend bool operator ==(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks == t2._ticks;
		}

		constexpr friend bool operator !=(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks != t2._ticks;
		}

		constexpr friend bool operator <(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks < t2._ticks;
		}

		constexpr friend bool operator <=(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks <= t2._ticks;
		}

		constexpr friend bool operator >(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks > t2._ticks;
		}

		constexpr friend bool operator >=(TimeSpan const& t1, TimeSpan const& t2) {
			return t1._ticks >= t2._ticks;
		}

		static constexpr TimeSpan Zero() { 
			return TimeSpan(); 
		}

		static constexpr TimeSpan MaxValue() { 
			return TimeSpan(long_max); 
		}

		static constexpr TimeSpan MinValue() {
			return TimeSpan(long_min);
		}

		static constexpr csint Compare(TimeSpan const& t1, TimeSpan const& t2) {
			if (t1._ticks > t2._ticks)
				return 1;

			if (t1._ticks < t2._ticks)
				return -1;

			return 0;
		}

		static constexpr TimeSpan FromDays(double value) {
			return Interval(value, MillisPerDay);
		}

		static constexpr TimeSpan FromHours(double value) {
			return Interval(value, MillisPerHour);
		}

		static constexpr TimeSpan Interval(double value, csint scale) {
			double tmp = value * scale;
			double millis = tmp + (value >= 0 ? 0.5 : -0.5);

			return TimeSpan(static_cast<cslong>(millis) * TicksPerMillisecond);
		}

		static constexpr TimeSpan FromMilliseconds(double value) {
			return Interval(value, 1);
		}

		static constexpr TimeSpan FromMinutes(double value) {
			return Interval(value, MillisPerMinute);
		}

		static constexpr TimeSpan FromSeconds(double value) {
			return Interval(value, MillisPerSecond);
		}

		static constexpr TimeSpan FromTicks(cslong value) {
			return TimeSpan(value);
		}

		constexpr cslong Ticks() const { return _ticks; }
		constexpr csint Days() const { return static_cast<int32_t>(_ticks / TicksPerDay); }
		constexpr csint Hours() const { return static_cast<int32_t>((_ticks / TicksPerHour) % 24); }
		constexpr csint Milliseconds() const { return static_cast<int32_t>((_ticks / TicksPerMillisecond) % 1000); }
		constexpr csint Minutes() const { return static_cast<int32_t>((_ticks / TicksPerMinute) % 60); }
		constexpr csint Seconds() const { return static_cast<int32_t>((_ticks / TicksPerSecond) % 60); }
		constexpr double TotalDays() const { return static_cast<double>(_ticks) * DaysPerTick; }
		constexpr double TotalHours() const { return static_cast<double>(_ticks) * HoursPerTick; }
		
		constexpr double TotalMilliseconds() const {
			double temp = static_cast<double>(_ticks) * MillisecondsPerTick;

			if (temp > MaxMilliSeconds)
				return  static_cast<double>(MaxMilliSeconds);

			if (temp < MinMilliSeconds)
				return  static_cast<double>(MinMilliSeconds);

			return temp;
		}

		constexpr double TotalMinutes() const { return static_cast<double>(_ticks) * MinutesPerTick; }
		constexpr double TotalSeconds() const { return static_cast<double>(_ticks) * SecondsPerTick; }

		constexpr TimeSpan Add(TimeSpan const& ts) const {
			cslong result = _ticks + ts._ticks;			

			if ((_ticks >> 63 == ts._ticks >> 63) && (_ticks >> 63 != result >> 63)) {		
				return TimeSpan(result, true);
			}				

			return TimeSpan(result);
		}

		constexpr TimeSpan Duration() const {		
			if (Ticks() == TimeSpan::MinValue().Ticks()) {
				return TimeSpan(Ticks(), true);
			}				

			return TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
		}

		constexpr bool Equals(TimeSpan other) const {
			return _ticks == other._ticks;
		}

		constexpr TimeSpan Negate() const {
			if (Ticks() == TimeSpan::MinValue().Ticks()) {
				return TimeSpan(Ticks(), true);
			}

			return TimeSpan(-_ticks);
		}

		constexpr TimeSpan Subtract(TimeSpan const& ts) const {
			int64_t result = _ticks - ts._ticks;

			if ((_ticks >> 63 != ts._ticks >> 63) && (_ticks >> 63 != result >> 63)) {
				return TimeSpan(result, true);
			}				

			return TimeSpan(result);
		}

		constexpr bool HasOverflowException() {
			return hasOverflowException;
		}

	private:
		constexpr TimeSpan(cslong ticks, bool overflow) :
			_ticks(ticks),
			hasOverflowException(overflow){
		}

		static constexpr double MillisecondsPerTick = 1.0 / TicksPerMillisecond;
		static constexpr double SecondsPerTick = 1.0 / TicksPerSecond;
		static constexpr double MinutesPerTick = 1.0 / TicksPerMinute;
		static constexpr double HoursPerTick = 1.0 / TicksPerHour;
		static constexpr double DaysPerTick = 1.0 / TicksPerDay;
		static constexpr csint MillisPerSecond = 1000;
		static constexpr csint MillisPerMinute = MillisPerSecond * 60;
		static constexpr csint MillisPerHour = MillisPerMinute * 60;
		static constexpr csint MillisPerDay = MillisPerHour * 24;
		static constexpr cslong MaxSeconds = std::numeric_limits<cslong>::max() / TicksPerSecond;
		static constexpr cslong MinSeconds = std::numeric_limits<cslong>::min() / TicksPerSecond;
		static constexpr cslong MaxMilliSeconds = std::numeric_limits<cslong>::max() / TicksPerMillisecond;
		static constexpr cslong MinMilliSeconds = std::numeric_limits<cslong>::min() / TicksPerMillisecond;
		static constexpr cslong TicksPerTenthSecond = TicksPerMillisecond * 100;

		bool hasOverflowException{ false };
		cslong _ticks{ 0 };

		constexpr static cslong TimeToTicks(csint const& hour, csint const& minute, csint const& second) {
			cslong totalSeconds =
				static_cast<cslong>(hour) * 3600 +
				static_cast<cslong>(minute) * 60 +
				static_cast<cslong>(second);

			return totalSeconds * TicksPerSecond;
		}

		constexpr static cslong DayToTicks(csint days, csint hours, csint minutes, csint seconds, csint milliseconds) {
			cslong totalMilliSeconds =
				(static_cast<cslong>(days) * 3600 * 24 +
					static_cast<cslong>(hours) * 3600 +
					static_cast<cslong>(minutes) * 60 + seconds) * 1000 + milliseconds;

			return totalMilliSeconds * TicksPerMillisecond;
		}
	};
}

#endif
