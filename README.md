# Pendulum C++

[Pendulum] is beautiful datetime library implemented by Python. I was ported it to C++!

## Requirements

- C++11 or above
- [cctz]

## Usage

### Include

```cpp
#include <pendulum/pendulum.h>
```

### Build

```
$ g++ -std=c++11 -I<INCLUDE_PATH> -L<LIBRARY_PATH> <SOURCES> -lcctz
```

### Instantiation

```cpp
const auto& dt = pendulum::datetime(2020, 4, 3);

// UTC
dt.timezone_name();
```

```cpp
const auto& dt = pendulum::datetime(2020, 4, 3, "Asia/Tokyo");

// Asia/Tokyo
dt.timezone_name();
```

```cpp
const auto& now = pendulum::now();              // 2020-04-05T13:21:37+09:00
const auto& today = pendulum::today();          // 2020-04-05T00:00:00+09:00
const auto& yesterday = pendulum::yesterday();  // 2020-04-04T00:00:00+09:00
const auto& tomorrow = pendulum::tomorrow();    // 2020-04-06T00:00:00+09:00
```

### Parsing

```cpp
// 2020-04-03T00:00:00+09:00
const auto& dt = pendulum::from_format("2020-04-03", "%Y-%m-%d", "Asia/Tokyo");

// 2015-06-10T00:00:00+09:00
const auto& dt = pendulum::parse("2015-06-10", "Asia/Tokyo");

// 2000-01-05T00:00:00+09:00
const auto& dt = pendulum::parse("20000105", "Asia/Tokyo");
```

### Attributes

```cpp
const auto& dt = pendulum::datetime(2020, 4, 3, 21, 54, 13, "Asia/Tokyo");

dt.year();          // 2020
dt.month();         // 4
dt.day();           // 3
dt.hour();          // 21
dt.minute();        // 54
dt.second();        // 13
dt.day_of_week();   // 4
dt.day_of_year();   // 94
dt.week_of_month(); // 1
dt.timestamp();     // 1585918453
dt.timezone_name(); // Asia/Tokyo
dt.offset();        // 32400
dt.offset_hours();  // 9.0
```

### Fluent helpers

```cpp
auto dt = pendulum::datetime(2020, 4, 3, 22, 18, 26, "Asia/Tokyo");

dt = dt.year(2021);                 // 2021-04-03T22:18:26+09:00
dt = dt.minute(14);                 // 2020-04-03T22:14:26+09:00
dt = dt.on(2021, 5, 10);            // 2020-05-10T22:18:26+09:00
dt = dt.at(10, 25, 16);             // 2020-04-03T10:25:16+09:00
dt = dt.timezone("US/Hawaii");      // 2020-04-03T22:18:26-10:00
dt = dt.in_timezone("US/Hawaii");   // 2020-04-03T03:18:26-10:00
dt = dt.offset_hours(4);            // 2020-04-03T22:18:26+04:00
dt = dt.in_offset_hours(4);         // 2020-04-03T17:18:26+04:00
```

### String formatting

```cpp
const auto& dt = pendulum::datetime(2020, 4, 3, 22, 18, 26, "Asia/Tokyo");

std::cout << dt;                                        // 2020-04-03T22:18:26+09:00
std::cout << dt.to_date_string();                       // 2020-04-03
std::cout << dt.to_datetime_string();                   // 2020-04-03 22:18:26
std::cout << dt.to_iso8601_string();                    // 2020-04-03T22:18:26+09:00
std::cout << dt.format("at %H:%M:%S on %d/%m, %Y");     // at 22:18:26 on 03/04, 2020
```

### Additions

```cpp
auto dt = pendulum::datetime(2020, 4, 3, 22, 18, 26, "Asia/Tokyo");

dt = dt.add_years(3);       // 2023-04-03 22:18:26
dt = dt.add_months(1);      // 2020-05-03 22:18:26
dt = dt.add_days(30);       // 2020-05-03 22:18:26
dt = dt.add_hours(2);       // 2020-04-04 00:18:26
dt = dt.add_minutes(73);    // 2020-04-03 23:31:26
dt = dt.add_seconds(92);    // 2020-04-03 22:19:58
```

### Modifiers

```cpp
const auto& dt = pendulum::datetime(2020, 4, 3, 15, 0, 0);

dt.start_of("year");    // 2020-01-01 00:00:00
dt.start_of("month");   // 2020-04-01 00:00:00
dt.start_of("day");     // 2020-04-03 00:00:00

dt.start_of("week");    // 2020-03-30 00:00:00 (Monday)
pendulum::week_starts_at(pendulum::kSunday);
dt.start_of("week");    // 2020-03-29 00:00:00 (Sunday)

dt.next(pendulum::kWednesday);  // 2020-04-08 00:00:00
dt.next();                      // 2020-04-10 00:00:00
dt.next(/* keep_time= */true);  // 2020-04-10 15:00:00

dt.previous(pendulum::kSunday);     // 2020-03-29 00:00:00
dt.previous();                      // 2020-03-27 00:00:00
dt.previous(/* keep_time= */true);  // 2020-03-27 15:00:00
```

### Period

```cpp
const auto& start = pendulum::datetime(2020, 1, 1);
const auto& stop = pendulum::datetime(2025, 12, 31);
const auto& period = pendulum::period(start, stop);

for (const auto& dt : period.range("years")) {
    std::cout << dt.to_date_string();   // 2020-01-01, 2021-01-01, ...
}

for (const auto& dt : period.range("months")) {
    std::cout << dt.to_date_string();   // 2020-01-01, 2020-02-01, ...
}

for (const auto& dt : period.range("days")) {
    std::cout << dt.to_date_string();   // 2020-01-01, 2020-01-02, ...
}
```

### Testing

```cpp
const auto& cpp98 = pendulum::datetime(1998, 9, 1);

// Set the mock
pendulum::set_test_now(cpp98);
pendulum::now();        // 1998-09-01T00:00:00+00:00

// Clear the mock
pendulum::set_test_now();
pendulum::now();        // 2020-04-04T23:30:52+09:00

// Scoped mock
pendulum::test(cpp98, [&]() {
    pendulum::now();    // 1998-09-01T00:00:00+00:00
});
pendulum::now();        // 2020-04-04T23:30:52+09:00
```

[Pendulum]:https://pendulum.eustace.io/
[cctz]:https://github.com/google/cctz
