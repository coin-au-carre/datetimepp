datetime ++
===========

An attempt to port [Python Datetime](https://docs.python.org/3/library/datetime.html) in C++.

It consists of a header-only file [datetime.h](/datetime.h) which relies on [Howard Hinnant's date libraries](https://github.com/HowardHinnant/date), precisely [date](https://howardhinnant.github.io/date/date.html) and [tz](https://howardhinnant.github.io/date/tz.html).

This is an ongoing work! So expect mistakes for the moment and changes.


### A little showcase

You can get the [C++ source code](/examples/00-showcase.cpp) and the equivalent [Python code](/examples/00-showcase.py).

```c++
auto x1 = Date::today();
cout << "Today's date is " << x1 << endl;

auto x2 = DateTime<>::now(); // same result as DateTime<>::today()
cout << "Today's datetime is " << x2 << endl;

auto time_delta = TimeDelta(date::weeks(1), date::days(2), hours(3), seconds(240), microseconds(10000)); // arguments order does not matter
cout << "Here the delta time we want to add " << time_delta << endl;

cout << "Here the added result " << x2 + time_delta << endl;

// The C++ version can allow seconds only precision and can directly take a timezone string
auto x3 = DateTime<std::chrono::seconds>::now("Asia/Tehran");
cout << "Time in " << x3.tzinfo() << " is " << x3 << endl;

auto x4 = DateTime<>::strptime("21/11/92 16:30", "%d/%m/%y %H:%M");
cout << "datime from strptime is " << x4 << endl;

auto x5 = DateTime<>::utcfromtimestamp(1497252490.0282006);
std::cout << "UTC datetime from timestamp is " << x5 << ". Check timestamp back: " << x5.timestamp() << std::endl;

std::cout << "You can substract two datetimes into a timedelta " << x5 - x4 << std::endl;
```

will output 

> Today's date is 2017-06-14  
> Today's datetime is 2017-06-14 10:12:35.651158550 CEST  
> Here the delta time we want to add 9 days, 03:04:00.010000  
> Here the added result 2017-06-23 13:16:35.661158550 CEST  
> Time in Asia/Tehran is 2017-06-14 12:42:36 +0430  
> datime from strptime is 1992-11-21 16:30:00.000000000 UTC  
> UTC datetime from timestamp is 2017-06-12 07:28:10.028200626 UTC. Check timestamp back: 1497252490.028201  
> You can substract two datetimes into a timedelta 8968 days, 14:58:10.028200  



### Class `datetime::TimeDelta` public interface

__Constructors__

```c++
    template <class Duration>
    TimeDelta(const Duration& d);

    template<class Duration, class ... Durations>
    TimeDelta(const Duration& d, const Durations&... durations);
```

__Member functions__

```c++
    const date::days::rep                   days()          const
    const std::chrono::seconds::rep         seconds()       const
    const std::chrono::microseconds::rep    microseconds()  const
    const std::chrono::seconds::rep         total_seconds() const
```

__Non member functions__

```c++
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
```

_Not implemented (yet)_

`operator/`, 
`operator%`, 
`divmod`, 
`min`,
`max`,
`resolution`



__Remarks__

+ Currently, negative timedeltas do not follow Python unusual behaviour. For instance, in Python, `timedelta(hours=-1)` gives `-1 day, 23:00:00` whereas in C++, `auto x = TimeDelta(std::chrono::hours(-1))` will output `0 days, -01:00:00.00`



### Class `datetime::Date` public interface


__Constructors__

```c++
    Date() = default;
    Date(const date::year_month_day& ymd);
    Date(const date::year& y, const date::month& m, const date::day& d);
```

__Member functions__

```c++
    const date::year            year()              const;
    const date::month           month()             const;
    const date::day             day()               const;
    const date::year_month_day& year_month_day()    const;

    date::weekday   objweekday()    const; // extra method : no equivalent in Python
    unsigned        weekday()       const;
    unsigned        isoweekday()    const;
    std::string     ctime()         const;
    std::string     isoformat()     const;
    std::string     strftime(const std::string& format) const;
```

__Non member functions__

```c++
    Date operator+(const Date& d, const TimeDelta& td);
    Date operator+(const TimeDelta& td, const Date& d);
    Date operator-(const Date& d, const TimeDelta& td);
    TimeDelta operator-(const Date& x, const Date& y);

    template<class CharT, class Traits>
    std::basic_ostream<CharT, Traits>&
    operator<<(std::basic_ostream<CharT, Traits>& os, const Date& date);
```

_Not implemented (yet)_

`fromordinal`, 
`replace`,
`timetuple`,
`toordinal`,
`isocalendar`,
`min`,
`max`,
`resolution`


__Remarks__

+ `objweekday()` returns a date::weekday object


### Class `datetime::DateTime` public interface


__Constructor__

```c++
DateTime(const date::zoned_time<common_duration>& zt)
```


__Member functions__

```c++
    const date::zoned_time<common_duration>& zoned_time() const;  // extra method : no equivalent in Python

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
```


__Non member functions__

```c++
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

    template <class Duration>
    DateTime<Duration> operator+(const DateTime<Duration>&  x, const TimeDelta& y);

    template <class Duration>
    DateTime<Duration> operator+(const DateTime<Duration>&  x, const TimeDelta& y);

    template <class Duration>
    DateTime<Duration> operator-(const TimeDelta& y, const DateTime<Duration>&  x);

    template <class Duration>
    TimeDelta operator-(const DateTime<Duration>&  x, const DateTime<Duration>& y);

    template <class Duration>
    bool operator<(const DateTime<Duration>&  x, const DateTime<Duration>& y);

    template<class CharT, class Traits, class Duration>
    std::basic_ostream<CharT, Traits>&
    operator<<(std::basic_ostream<CharT, Traits>& os, const DateTime<Duration>& date);
```



_Not implemented (yet)_

`fromordinal`, 
`combine`,
`fold`,
`replace`,
`utcoffset`,
`dst`,
`timetuple`,
`utctimetuple`,
`toordinal`,
`min`,
`max`,
`resolution`,
`tzset`


__Remarks__

+ The C++ version allows an optional `Duration` template parameter which will represents the time precision (e.g. `std::chrono::milliseconds`)
+ The C++ version of `tzinfo` returns a string which is the time zone name
+ `time_zone()` method gives `time_zone*` object from [tz](https://howardhinnant.github.io/date/tz.html#time_zone).


### To work on

* Time class
* Add Tests
* Constexpr compile time possibilities
* No exception safety?

