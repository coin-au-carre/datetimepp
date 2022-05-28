#ifndef DATETIME_H
#define DATETIME_H

// The MIT License (MIT)
//
// Copyright (c) 2017 Florian Dang
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Our apologies.  When the previous paragraph was written, lowercase had not yet
// been invented (that would involve another several millennia of evolution).
// We did not mean to shout.

#include "date.h"
#include "tz.h"

#include <iomanip>
#include <cmath>

#include <iostream>

namespace datetime 
{
// TimeDelta
// Warning! normalization of negative values is not the same as in Python
class TimeDelta
{
    date::days                  days_;
    std::chrono::seconds        seconds_;
    std::chrono::microseconds   microseconds_;

public:
    template <class Duration>
    TimeDelta(const Duration& d);

    template<class Duration, class ... Durations>
    TimeDelta(const Duration& d, const Durations&... durations);

    const date::days::rep                   days()          const { return days_.count(); }
    const std::chrono::seconds::rep         seconds()       const { return seconds_.count(); }
    const std::chrono::microseconds::rep    microseconds()  const { return microseconds_.count(); }

    const std::chrono::seconds::rep         total_seconds() const;

private:
    template<class Duration>
    void add_delta(const Duration& d); 

    template<class Duration, class ... Durations>
    void add_delta(const Duration& d, const Durations&... durations); 
};


TimeDelta operator+(const TimeDelta&  x, const TimeDelta& y);
TimeDelta operator-(const TimeDelta&  x, const TimeDelta& y);

template<class Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type* = nullptr>
TimeDelta operator*(Scalar s, const TimeDelta& x);

template<class Scalar, typename std::enable_if<std::is_integral<Scalar>::value>::type* = nullptr>
TimeDelta operator*(Scalar s, const TimeDelta& x);

template<class Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type* = nullptr>
TimeDelta operator*(const TimeDelta& x, Scalar s);

template<class Scalar, typename std::enable_if<std::is_integral<Scalar>::value>::type* = nullptr>
TimeDelta operator*(const TimeDelta& x, Scalar s);

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TimeDelta& td);



class Date 
{
    date::year_month_day ymd_;

public:
    static Date today();

    template <class Rep>
    static Date fromtimestamp(Rep timestamp);

    Date() = default;
    Date(const date::year_month_day& ymd);
    Date(const date::year& y, const date::month& m, const date::day& d);

    const date::year            year()              const;
    const date::month           month()             const;
    const date::day             day()               const;
    const date::year_month_day& year_month_day()    const;

    date::weekday   objweekday()    const;
    unsigned        weekday()       const;
    unsigned        isoweekday()    const;
    std::string     ctime()         const;
    std::string     isoformat()     const;
    std::string     strftime(const std::string& format) const;
};

Date operator+(const Date& d, const TimeDelta& td);
Date operator+(const TimeDelta& td, const Date& d);
Date operator-(const Date& d, const TimeDelta& td);
TimeDelta operator-(const Date& x, const Date& y);

bool operator==(const Date& x, const Date& y);

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Date& date);



template <class Duration = std::chrono::system_clock::duration>
class DateTime
{
    using common_duration = typename std::common_type<Duration, std::chrono::seconds>::type;

    date::zoned_time<common_duration> zt_;

public:
    static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
    today();
    
    static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
    now(const std::string& timezone_name = "");

    static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
    utcnow();
    
    template <class Rep>
    static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
    fromtimestamp(Rep timestamp, const std::string& timezone_name = "");

    template <class Rep>
    static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
    utcfromtimestamp(Rep timestamp);

    static DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
    strptime(const std::string& date_string, const std::string& format);

    DateTime(const date::zoned_time<common_duration>& zt) : zt_(zt) {}

    const date::zoned_time<common_duration>& zoned_time() const { return zt_; }  // extra method : no equivalent in Python

    Date        date()  const;
    const date::year  year()  const;
    const date::month month() const;
    const date::day   day()   const;

    const date::time_zone* time_zone() const; // extra method : no equivalent in Python
    const std::string& tzinfo() const; // differs from Python

    TimeDelta utcoffset() const;

    std::string timestamp() const;

    std::string ctime() const;
    std::string isoformat(const std::string& sep="T") const;
    std::string strftime(const std::string& format) const;


private:
    date::fields<typename std::common_type<Duration, std::chrono::seconds>::type>
    fields_ymd_time() const;
};

template <class Duration>
DateTime<Duration> operator+(const DateTime<Duration>&  x, const TimeDelta& y);

template <class Duration>
DateTime<Duration> operator+(const DateTime<Duration>&  x, const TimeDelta& y);

template <class Duration>
DateTime<Duration> operator-(const TimeDelta& y, const DateTime<Duration>&  x);

// TODO: allow two different Duration template params ?
template <class Duration>
TimeDelta operator-(const DateTime<Duration>&  x, const DateTime<Duration>& y);

template <class Duration>
bool operator<(const DateTime<Duration>&  x, const DateTime<Duration>& y);

template<class CharT, class Traits, class Duration>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const DateTime<Duration>& date);



class Time 
{
    date::time_of_day<std::chrono::system_clock::duration> time_of_day_;

public:
    template<class Duration>
    Time(const Duration& dur);

    template<class Duration, class ... Durations>
    Time(const Duration& d, const Durations&... durations);

    const date::time_of_day<std::chrono::system_clock::duration>& time_of_day() const;

    std::chrono::hours::rep hour() const;
    std::chrono::minutes::rep minute() const;
    std::chrono::seconds::rep seconds() const;

    std::string isoformat() const;
    std::string strftime(const std::string& format) const;

private:
    template<class Duration>
    Duration add_durations(const Duration& d);

    template<class Duration, class ... Durations>
    auto add_durations(const Duration& d, const Durations&... durations)
    -> typename std::common_type<Duration, Durations...>::type;
};

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Time& time);


// TimeDelta impl

// TODO there is a problem when using date::months
template <class Duration>
inline 
TimeDelta::TimeDelta(const Duration& d) 
    : days_(std::chrono::duration_cast<date::days>(d))
    , seconds_(std::chrono::duration_cast<std::chrono::seconds>(d - days_))
    , microseconds_(std::chrono::duration_cast<std::chrono::microseconds>(d - days_ - seconds_))
    {
    }


template <class Duration, class ... Durations>
inline 
TimeDelta::TimeDelta(const Duration& d, const Durations&... durations) 
    : TimeDelta(d) // use delegate ctor here
    {
        add_delta(durations...);
    }

template<class Duration>
inline
void TimeDelta::add_delta(const Duration& d)
{
    auto total_duration = days_ + seconds_ + microseconds_ + d;
    days_           = std::chrono::duration_cast<date::days>(total_duration);
    seconds_        = std::chrono::duration_cast<std::chrono::seconds>(total_duration - days_);
    microseconds_   = std::chrono::duration_cast<std::chrono::microseconds>(total_duration - days_ - seconds_);
}

template<class Duration, class ... Durations>
inline
void TimeDelta::add_delta(const Duration& d, const Durations& ... durations)
{
    add_delta(d);
    add_delta(durations...);
}


inline
const std::chrono::seconds::rep TimeDelta::total_seconds() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(days_ + seconds_).count();
}


inline
TimeDelta operator+(const TimeDelta& x, const TimeDelta& y)
{
    return 
    {
        date::days(x.days() + y.days()), 
        std::chrono::seconds(x.seconds() + y.seconds()), 
        std::chrono::microseconds(x.microseconds() + y.microseconds())
    };
}

inline
TimeDelta operator-(const TimeDelta& x, const TimeDelta& y)
{
    return 
    {
        date::days(x.days() - y.days()), 
        std::chrono::seconds(x.seconds() - y.seconds()), 
        std::chrono::microseconds(x.microseconds() - y.microseconds())
    };
}

// float
template<class Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type*>
inline
TimeDelta operator*(Scalar s, const TimeDelta& x)
{
    return 
    {
        std::chrono::seconds(std::lrint(s * x.total_seconds())), // round-half-to-even
        std::chrono::microseconds(std::lrint(s * x.microseconds()))
    };
}

// integer
template<class Scalar, typename std::enable_if<std::is_integral<Scalar>::value>::type*>
inline
TimeDelta operator*(Scalar s, const TimeDelta& x)
{
    return 
    {
        date::days(s * static_cast<unsigned>(x.days())), 
        std::chrono::seconds(s * x.seconds()), 
        std::chrono::microseconds(s * x.microseconds())
    };
}

template<class Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type*>
inline
TimeDelta operator*(const TimeDelta& x, Scalar s)
{
    return s * x;
}

template<class Scalar, typename std::enable_if<std::is_integral<Scalar>::value>::type*>
inline
TimeDelta operator*(const TimeDelta& x, Scalar s)
{
    return s * x;
}


template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TimeDelta& td)
{
    // return os << '(' << td.days() << " days, " << td.seconds() << " s, " << td.microseconds() << " µs)";
    using namespace std::chrono;
    return os << td.days() << " days, " << date::make_time(seconds(td.seconds()) + microseconds(td.microseconds()));
}



// Date impl

inline
Date Date::today() 
{
    return { date::floor<date::days>(std::chrono::system_clock::now()) };
}


template<class Rep>
inline
Date Date::fromtimestamp(Rep timestamp) 
{
    using namespace std::chrono;
    // system_clock::time_point dt{seconds{timestamp}}; // compatible with old compilers ?
    seconds dur(timestamp);
    time_point<system_clock> dt(dur);

    return { date::floor<date::days>(dt) }; // convert time_point to sys_days
}

inline
Date::Date(const date::year_month_day& ymd) 
    : ymd_(ymd) 
    {}

inline
Date::Date(const date::year& y, const date::month& m, const date::day& d) 
    : ymd_(y, m, d) 
    {}

inline
const date::year_month_day& Date::year_month_day() const 
{
    return ymd_;
}

inline
const date::year Date::year() const
{
    return ymd_.year();
}

inline
const date::month Date::month() const
{
    return ymd_.month();
}

inline
const date::day Date::day() const
{
    return ymd_.day();
}

inline
std::string Date::ctime() const
{
    return strftime("%c");
}

inline
date::weekday Date::objweekday() const
{
    return date::weekday(ymd_);
}

inline
unsigned Date::weekday() const
{
    return (static_cast<unsigned>(objweekday()) - 1) % 7; // can do better
}

inline
unsigned Date::isoweekday() const
{
    return 1 + weekday();
}


inline
std::string Date::isoformat() const
{
    return strftime("YYYY-MM-DD");
}

inline
std::string Date::strftime(const std::string& format) const
{
    return date::format(format.c_str(), ymd_);
}



inline
Date operator+(const Date& d, const TimeDelta& td)
{
    return { date::sys_days(d.year_month_day()) + date::days(td.days()) };
}

inline
Date operator+(const TimeDelta& td, const Date& d)
{
    return d + td;
}

inline
Date operator-(const Date& d, const TimeDelta& td)
{
    return { date::sys_days(d.year_month_day()) - date::days(td.days()) };
}

inline
TimeDelta operator-(const Date& x, const Date& y)
{
    return { date::sys_days(x.year_month_day()) - date::sys_days(y.year_month_day()) };
}

inline
bool operator==(const Date& x, const Date& y)
{
    return x.year_month_day() == y.year_month_day();
}


template<class CharT, class Traits>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Date& date)
{
    return os << date.year_month_day();
}



// DateTime impl

template <class Duration>
inline
DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
DateTime<Duration>::today()
{
    return date::make_zoned(date::current_zone(), date::floor<Duration>(std::chrono::system_clock::now()));
}


template <class Duration>
inline
DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
DateTime<Duration>::now(const std::string& timezone_name)
{
    if (timezone_name == "")
    {
        return today();
    }
    return date::make_zoned(timezone_name, date::floor<Duration>(std::chrono::system_clock::now()));
}


template <class Duration>
inline
DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
DateTime<Duration>::utcnow()
{
    return { date::floor<Duration>(std::chrono::system_clock::now()) };
}

template<class Duration>
template<class Rep>
inline
DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
DateTime<Duration>::fromtimestamp(Rep timestamp, const std::string& timezone_name) 
{

    auto nanos = static_cast<unsigned>(1e9*std::fmod(timestamp,1)); // might loose precision here
    auto tp = std::chrono::system_clock::from_time_t(timestamp) + std::chrono::nanoseconds(nanos);
    if (timezone_name == "") 
    {
        return { date::make_zoned(date::current_zone(), tp) };
    }
    else 
    {
        return { date::make_zoned(timezone_name, tp) };
    }
}

template<class Duration>
template<class Rep>
inline
DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
DateTime<Duration>::utcfromtimestamp(Rep timestamp) 
{
    auto nanos = static_cast<unsigned>(1e9*std::fmod(timestamp,1)); // might loose precision here
    auto tp = std::chrono::system_clock::from_time_t(timestamp) + std::chrono::nanoseconds(nanos);

    return { tp };
}

template <class Duration>
inline
DateTime<typename std::common_type<Duration, std::chrono::seconds>::type> 
DateTime<Duration>::strptime(const std::string& date_string, const std::string& format)
{
    using namespace std::chrono;

    date::local_seconds tp;
    std::istringstream ss(date_string);
    ss >> date::parse(format, tp);
    auto zt = date::make_zoned(date::current_zone(), tp);
    return { zt };
}


template <class Duration>
inline
date::fields<typename std::common_type<Duration, std::chrono::seconds>::type> 
DateTime<Duration>::fields_ymd_time() const
{
    using CT = typename std::common_type<Duration, std::chrono::seconds>::type;
    auto tp = zoned_time().get_local_time();
    auto ld = date::floor<date::days>(tp);
    date::fields<CT> fds{date::year_month_day{ld}, date::time_of_day<CT>{tp-ld}};
    return fds;
}

template <class Duration>
inline
Date DateTime<Duration>::date() const
{
    return fields_ymd_time().ymd;
}

template <class Duration>
inline
const date::year DateTime<Duration>::year() const
{
    return date().year();
}

template <class Duration>
inline
const date::month DateTime<Duration>::month() const
{
    return date().month();
}

template <class Duration>
inline
const date::day DateTime<Duration>::day() const
{
    return date().day();
}

template <class Duration>
inline
const date::time_zone* DateTime<Duration>::time_zone() const
{
    return zt_.get_time_zone();
}

template <class Duration>
inline
const std::string& DateTime<Duration>::tzinfo() const
{
    return zt_.get_time_zone()->name();
}

template <class Duration>
inline
TimeDelta DateTime<Duration>::utcoffset() const
{
    auto offset = zoned_time().get_info().offset;
    return { std::chrono::seconds{offset} };
}

template <class Duration>
inline
std::string DateTime<Duration>::timestamp() const
{
    std::stringstream ss;
    ss << std::fixed << zoned_time().get_sys_time().time_since_epoch().count() / 1000000000.0;
    return ss.str();
}

template <class Duration>
inline
std::string DateTime<Duration>::ctime() const
{
    return strftime("%c");
}

template <class Duration>
inline
std::string DateTime<Duration>::isoformat(const std::string& sep) const
{
    return strftime("%Y%m%d"+sep+"%H:%M:%S");
}

template <class Duration>
inline
std::string DateTime<Duration>::strftime(const std::string& format) const
{
    return date::format(format.c_str(), zt_);
}

template <class Duration>
inline
DateTime<Duration> operator+(const DateTime<Duration>& x, const TimeDelta& y)
{
    // TODO make this work for non default Duration
    using namespace std::chrono;
    auto add = x.zoned_time().get_sys_time() + seconds(y.total_seconds()) + microseconds(y.microseconds());
    return { date::make_zoned(x.zoned_time().get_time_zone(), add) };
}

template <class Duration>
inline
DateTime<Duration> operator+(const TimeDelta& y, const DateTime<Duration>& x)
{
    return x + y;
}

template <class Duration>
inline
DateTime<Duration> operator-(const DateTime<Duration>&  x, const TimeDelta& y)
{
    using namespace std::chrono;
    auto diff = x.zoned_time().get_sys_time() - seconds(y.total_seconds()) - microseconds(y.microseconds());
    return { date::make_zoned(x.zoned_time().get_time_zone(), diff) };
}

template <class Duration>
inline
TimeDelta operator-(const DateTime<Duration>&  x, const DateTime<Duration>& y)
{
    return { x.zoned_time().get_sys_time() - y.zoned_time().get_sys_time() };
}

template <class Duration>
inline
bool operator<(const DateTime<Duration>&  x, const DateTime<Duration>& y) 
{
    return x.zoned_time().get_sys_time() < y.zoned_time().get_sys_time();
}

template<class CharT, class Traits, class Duration>
inline
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const DateTime<Duration>& date)
{
    return os << date.zoned_time();
}


// Time impl

template <class Duration>
inline
Time::Time(const Duration& dur) 
    : time_of_day_(
        date::make_time(std::chrono::duration_cast<std::chrono::system_clock::duration>(dur))
    )
    {}


template<class Duration, class ... Durations>
inline
Time::Time(const Duration& d, const Durations& ... durations)
    : time_of_day_(
        date::make_time(std::chrono::duration_cast<std::chrono::system_clock::duration>(add_durations(d, durations...)))
    )
    {}


template<class Duration>
inline
Duration Time::add_durations(const Duration& d)
{
    return d;
}

template<class Duration, class ... Durations>
inline
auto Time::add_durations(const Duration& d, const Durations& ... durations)
-> typename std::common_type<Duration, Durations...>::type
{
    return d + add_durations(durations...);
}


inline
const date::time_of_day<std::chrono::system_clock::duration>& Time::time_of_day() const
{
    return time_of_day_;
}


inline
std::chrono::hours::rep Time::hour() const
{
    return time_of_day().hours().count();
}

inline
std::chrono::minutes::rep Time::minute() const
{
    return time_of_day().minutes().count();
}

inline
std::chrono::seconds::rep Time::seconds() const
{
    return time_of_day().seconds().count();
}


inline
std::string Time::isoformat() const
{
    return strftime("%H:%M:%S");
}

inline
std::string Time::strftime(const std::string& format) const
{
    std::tm tm;
    tm.tm_sec = static_cast<int>(seconds());
    tm.tm_min = static_cast<int>(minute());
    tm.tm_hour = static_cast<int>(hour());
    // no milli nor microseconds :(
    std::stringstream buffer;
    buffer << std::put_time(&tm, format.c_str());
    return buffer.str();
}

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Time& time)
{
    return os << time.time_of_day();
}


} // namespace datetime

#endif // DATETIME_H
