datetime 模块详解 -- 基本的日期和时间类型

datetime 模块提供了各种类用于操作日期和时间，该模块侧重于高效率的格式化输出

在 Python 中，与时间处理有关的模块包括：time，datetime 以及 calendar


datetime 模块定义了两个常量：

datetime.MINYEAR - date 和 datetime 对象所能支持的最小年份，object.MINYEAR 的值为 1
datetime.MAXYEAR - date 和 datetime 对象所能支持的最大年份，object.MAXYEAR 的值为 9999


datetime 模块中定义的类（前四个下方有详解）：

datetime.date - 表示日期的类，常用属性：year, month, day
datetime.time - 表示时间的类，常用属性：hour, minute, second, microsecond, tzinfo
datetime.datetime - 表示日期和时间的类，常用属性： year, month, day, hour, minute, second, microsecond, tzinfo
datetime.timedelta - 表示时间间隔，即两个时间点（date，time，datetime）之间的长度
datetime.tzinfo - 表示时区的基类，为上方的 time 和 datetime 类提供调整的基准
datetime.timezone - 表示 UTC 时区的固定偏移，是 tzinfo 基类的实现

注：上边这些类的对象是不可变的

上边这些类的从属关系：
object
    timedelta
    tzinfo
        timezone
    time
    date
        datetime
复制代码





timedelta 对象

timedelta 对象表示两个日期或时间之间的间隔

datetime.timedelta(days=0, seconds=0, microseconds=0, milliseconds=0, minutes=0, hours=0, weeks=0) 
复制代码

以上所有的参数都是可选的（默认为 0），参数的可以是整数或浮点数，正数或负数。

内部的存储单位只有 days（天）、seconds（秒）、microseconds（毫秒），其他单位均先转换后再存储：

1 millisecond -> 1000 microseconds
1 minutes -> 60 seconds
1 hours -> 3600 seconds
1 weeks -> 7 days

而 days、seconds 和 microseconds 为了不产生时间表示上的歧义，将根据以下范围自动“进位”：

0 <= microseconds < 1000000
0 <= seconds < 3600 * 24（1小时的秒数 * 24小时）
-999999999 <= days <= 999999999


timedelta 类属性：

timedelta.min - timedelta 对象负值的极限，timedelta(-999999999)
timedelta.max - timedelta 对象正值的极限，timedelta(days=999999999, hours=23, minutes=59, seconds=59, microseconds=999999)
timedelta.resolution - 两个 timedelta 不相等的对象之间最小的差值，timedelta(microseconds=1)

请注意，在正常情况下，timedelta.max > -timedelta.min，-timedelta.max 无意义。


timedelta 实例属性（只读）：

属性    取值范围
timedelta.days  -999999999 ~ 999999999
timedelta.seconds0   ~ 86399
timedelta.microseconds  0 ~ 999999


timedelta 对象支持的操作：

操作    结果
t1 = t2 + t3    t2 和 t3 的和，随后：t1 - t2 == t3 and t1 - t3 == t2 为 True（注1）
t1 = t2 - t3    t2 和 t3 的差，随后：t1 == t2 - t3 and t2 == t1 + t3 为 True（注1）
t1 = t2 * i 或 t1 = i * t2  对象乘以一个整数，随后：t1 // i == t2 为 true；且 i != 0
t1 = t2 * f 或 t1 = f * t2  对象乘以一个浮点数，结果四舍五入到精度 timedelta.resolution（注1）
f = t2 / t3 t2 和 t3 的商（注3），返回一个 float 对象
t1 = t2 / f 或 t1 = t2 / i  对象除以一个整数或浮点数，结果四舍五入到精度 timedelta.resolution
t1 = t2 // i 或 t1 = t2 // t3   对象地板除一个整数或浮点数，结果舍去小数，返回一个整数（注3）
t1 = t2 % t3t2 和 t3 的余数，返回一个 timedelta 对象（注3）
q, r = divmod(t1, t2)   计算 t1 和 t2 的商和余数，q = t1 // t2（注3），r = t1 % t2，q 是一个整数，r 是一个 timedelta 对象
+t1 返回一个 timedelta 对象，且值相同（注2）
-t1 等同于 timedelta(-t1.days, -t1.seconds, -t1.microseconds)，并且相当于 t1 * -1（注1、4）
abs(t)  当 t.days >= 0 时，等同于 +t；当 t.days < = 时，等同于 -t（注2）
str(t)  返回一个字符串，按照此格式：[D day[ s ], ][H]H:MM:SS[.UUUUUU]
repr(t) 返回一个字符串，按照此格式：datetime.timedelta(D[, S[, U]])

注1：这是准确的，但可能会溢出
注2：这是准确的，并且不会溢出
注3：除数为 0 会引发 ZeroDivisionError 异常
注4：-timedelta.max 是无意义的


timedelta 实例方法：

timedelta.total_seconds()
- 返回 timedelta 对象所包含的总秒数，相当于 td / timedelta(seconds=1)

请注意，对于非常大的时间间隔（在大多数平台上是大于270年），这种方法将失去微秒（microsecond）精度


timedelta 用法示例：

# 爱学习，爱鱼C工作室
>>> from datetime import timedelta
>>> year = timedelta(days=365)
>>> another_year = timedelta(weeks=40, days=84, hours=23,
...                          minutes=50, seconds=600)  # adds up to 365 days
>>> year.total_seconds()
31536000.0
>>> year == another_year
True
>>> ten_years = 10 * year
>>> ten_years, ten_years.days // 365
(datetime.timedelta(3650), 10)
>>> nine_years = ten_years - year
>>> nine_years, nine_years.days // 365
(datetime.timedelta(3285), 9)
>>> three_years = nine_years // 3;
>>> three_years, three_years.days // 365
(datetime.timedelta(1095), 3)
>>> abs(three_years - ten_years) == 2 * three_years + year
True




date 对象

date 对象表示一个日期，在一个理想化的日历里，日期由 year（年）、month（月）、day（日）组成

datetime.date(year, month, day)
复制代码

所有的参数都是必需的，参数可以是整数，并且在以下范围内：

MINYEAR <= year <= MAXYEAR（也就是 1 ~ 9999）
1 <= month <= 12
1 <= day <= 根据 year 和 month 来决定（例如 2015年2月 只有 28 天）


date 类方法（classmethod）：

date.today() - 返回一个表示当前本地日期的 date 对象
date.fromtimestamp(timestamp) - 根据给定的时间戮，返回一个 date 对象
date.fromordinal(ordinal) - 将 Gregorian 日历时间转换为 date 对象（Gregorian Calendar：一种日历表示方法，类似于我国的农历，西方国家使用比较多）


date 类属性：

date.min - date 对象所能表示的最早日期，date(MINYEAR, 1, 1)
date.max - date 对象所能表示的最晚日期，date(MAXYEAR, 12, 31)
date.resolution - date 对象表示日期的最小单位，在这里是 1 天，timedelta(days=1)


date 实例属性（只读）：

属性取值范围date.yearMINYEAR ~ MAXYEAR（1 ~ 9999）date.month1 ~ 12date.day1 ~ 根据 year 和 month 来决定（例如 2015年2月 只有 28 天）


date 对象支持的操作：

操作结果date2 = date1 + timedelta日期加上一个时间间隔，返回一个新的日期对象（注1）date2 = date1 - timedelta日期减去一个时间间隔，相当于 date2 + timedelta == date1（注2）timedelta = date1 - date2（注3）date1 < date2当 date1 的日期在 date2 之前时，我们认为 date1 < date2（注4）

注1：timedelta.day > 0 或 timedelta.day < 0 决定 date2 日期增长的方向；随后，date2 - date1 == timedelta.days；timedelta.seconds 和 timedelta.microseconds 被忽略；如果 date2.year < MINYEAR 或 date2.year > MAXYEAR，引发 OverflowError 异常

注2：这并不等同于 date1 + (-timedelta)，因为单独的 -timedelta 可能会溢出，而 date1 - timedelta 则不会溢出；timedelta.seconds 和 timedelta.microseconds 被忽略

注3：这是准确的，并且不会溢出；timedelta.seconds 和 timedelta.microseconds 都为 0，然后 date2 + timedelta == date1

注4：换句话说，当且仅当 date1.toordinal() < date2.toordinal()，才有 date1 < date2


date 实例方法：

date.replace(year, month, day)
- 生成一个新的日期对象，用参数指定的年、月、日代替原有对象中的属性

date.timetuple()
- 返回日期对应的 time.struct_time 对象（类似于 time 模块的 time.localtime()）

date.toordinal()
- 返回日期对应的 Gregorian Calendar 日期

date.weekday()
- 返回 0 ~ 6 表示星期几（星期一是 0，依此类推）

date.isoweekday()
- 返回 1 ~ 7 表示星期几（星期一是1， 依此类推）

date.isocalendar()
- 返回一个三元组格式 (year, month, day)

date.isoformat()
- 返回一个 ISO 8601 格式的日期字符串，如 "YYYY-MM-DD" 的字符串

date.__str__()
- 对于 date 对象 d 来说，str(d) 相当于 d.isoformat()

date.ctime()
- 返回一个表示日期的字符串，相当于 time 模块的 time.ctime(time.mktime(d.timetuple()))

date.strftime(format)
- 返回自定义格式化字符串表示日期，下面有详解

date.__format__(format)
- 跟 date.strftime(format) 一样，这使得调用 str.format() 时可以指定 data 对象的字符串


以下是计算天数的例子：

# You may say I'm the dreamer. But I'm not the only one! 

>>> import time
>>> from datetime import date
>>> today = date.today()
>>> today
datetime.date(2014, 8, 31)
>>> today == date.fromtimestamp(time.time())
True
>>> my_birthday = date(today.year, 6, 24)
>>> if my_birthday < today:
        my_birthday = my_birthday.replace(year = today.year + 1)

>>> my_birthday
datetime.date(2015, 6, 24)
>>> time_to_birthday = abs(my_birthday - today)
>>> time_to_birthday.days
297
复制代码


关于 date 的综合应用：

# Follow FishC. Follow your dream!

>>> from datetime import date
>>> d = date.fromordinal(735678)  # 自日期 1.1.0001 之后的第 735678 天
>>> d
datetime.date(2015, 3, 21)
>>> t = d.timetuple()
>>> for i in t:
        print(i)
        
2015
3
21
0
0
0
5
80
-1
>>> ic = d.isocalendar()
>>> for i in ic:
        print(i)
        
2015
12
6
>>> d.isoformat()
'2015-03-21'
>>> d.strftime("%d/%m/%y")
'21/03/15'
>>> d.strftime("%A %d. %B %Y")
'Saturday 21. March 2015'
>>> 'The {1} is {0:%d}, the {2} is {0:%B}.'.format(d, "day", "month")
'The day is 21, the month is March.'
复制代码





time 对象

time 对象表示一天中的一个时间，并且可以通过 tzinfo 对象进行调整

datetime.time(hour=0, minute=0, second=0, microsecond=0, tzinfo=None)
复制代码

所有的参数都是可选的；tzinfo 可以是 None 或者 tzinfo 子类的实例对象；其余的参数可以是整数，并且在以下范围内：

0 <= hour < 24
0 <= minute < 60
0 <= second < 60
0 <= microsecond < 1000000

注：如果参数超出范围，将引发 ValueError 异常


time 类属性

time.min - time 对象所能表示的最早时间，time(0, 0, 0, 0)
time.max - time 对象所能表示的最晚时间，time(23, 59, 59, 999999)
time.resolution - time 对象表示时间的最小单位，在这里是 1 毫秒，timedelta(microseconds=1)


time 实例属性（只读）：

属性    取值范围
time.hour   0 ~ 23
time.minute 0 ~ 59
time.second 0 ~ 59
time.microsecond    0 ~ 999999
time.tzinfo通过构造函数的 tzinfo 参数赋值


time 实例方法：

time.replace([hour[, minute[, second[, microsecond[, tzinfo]]]]]) 
- 生成一个新的时间对象，用参数指定时间代替原有对象中的属性

time.isoformat()
- 返回一个 ISO 8601 格式的日期字符串，如 "HH:MM:SS.mmmmmm" 的字符串

time.__str__()
- 对于 time 对象 t 来说，str(t) 相当于 t.isoformat()

time.strftime(format)
- 返回自定义格式化字符串表示时间，下面有详解

time.__format__(format)
- 跟 time.strftime(format) 一样，这使得调用 str.format() 时可以指定 time 对象的字符串

time.utcoffset()
- 如果 tzinfo 属性是 None，则返回 None；否则返回 self.tzinfo.utcoffset(self)

time.dst()
- 如果 tzinfo 属性是 None，则返回 None；否则返回 self.tzinfo.dst(self)

time.tzname()
- 如果 tzinfo 属性是 None，则返回 None；否则返回 self.tzinfo.tzname(self)


关于 time 的综合应用：

# 学编程，到鱼C
>>> from datetime import time, timedelta, tzinfo
>>> class GMT1(tzinfo):
        def utcoffset(self, dt):
                return timedelta(hours=1)
        def dst(self, dt):
                return timedelta(0)
        def tzname(self, dt):
                return "欧洲/布拉格"

>>> t = time(14, 10, 30, tzinfo=GMT1())
>>> t
datetime.time(14, 10, 30, tzinfo=<__main__.GMT1 object at 0x02D7FE90>)
>>> gmt = GMT1()
>>> t.isoformat()
'14:10:30+01:00'
>>> t.dst()
datetime.timedelta(0)
>>> t.tzname()
'欧洲/布拉格'
>>> t.strftime("%H:%M:%S %Z")
'14:10:30 欧洲/布拉格'
>>> 'The {} is {:%H:%M}.'.format("time", t)
'The time is 14:10.'
复制代码





datetime 对象

datetime 对象是 date 对象和 time 对象的结合体，并且包含他们的所有信息

datetime.datetime(year, month, day, hour=0, minute=0, second=0, microsecond=0, tzinfo=None)
复制代码

必须的参数是 year（年）、month（月）、day（日）；tzinfo 可以是 None 或者 tzinfo 子类的实例对象；其余的参数可以是整数，并且在以下范围内：

MINYEAR <= year <= MAXYEAR（也就是 1 ~ 9999）
1 <= month <= 12
1 <= day <= 根据 year 和 month 来决定（例如 2015年2月 只有 28 天）
0 <= hour < 24
0 <= minute < 60
0 <= second < 60
0 <= microsecond < 1000000

注：如果参数超出范围，将引发 ValueError 异常


datetime 类方法（classmethod）：

datetime.today()
- 返回一个表示当前本地时间的 datetime 对象，等同于 datetime.fromtimestamp(time.time())

datetime.now(tz=None)
- 返回一个表示当前本地时间的 datetime 对象；如果提供了参数 tz，则获取 tz 参数所指时区的本地时间

datetime.utcnow()
- 返回一个当前 UTC 时间的 datetime 对象

datetime.fromtimestamp(timestamp, tz=None)
- 根据时间戮创建一个 datetime 对象，参数 tz 指定时区信息

datetime.utcfromtimestamp(timestamp)
- 根据时间戮创建一个 UTC 时间的 datetime 对象

datetime.fromordinal(ordinal)
- 返回对应 Gregorian 日历时间对应的 datetime 对象

datetime.combine(date, time)
- 根据参数 date 和 time，创建一个 datetime 对象

datetime.strptime(date_string, format)
- 将格式化字符串转换为 datetime 对象


datetime 类属性：

datetime.min - datetime 对象所能表示的最早日期，datetime(MINYEAR, 1, 1, tzinfo=None)
datetime.max - datetime 对象所能表示的最晚日期，datetime(MAXYEAR, 12, 31, 23, 59, 59, 999999, tzinfo=None)
datetime.resolution - datetime 对象表示日期的最小单位，在这里是 1 毫秒，timedelta(microseconds=1)


datetime 实例属性（只读）：

属性取值范围datetime.yearMINYEAR ~ MAXYEAR（1 ~ 9999）datetime.month1 ~ 12datetime.day1 ~ 根据 year 和 month 来决定（例如 2015年2月 只有 28 天）datetime.hour0 ~ 23datetime.minute0 ~ 59datetime.second0 ~ 59datetime.microsecond0 ~ 999999datetime.tzinfo通过构造函数的 tzinfo 参数赋值


datetime 对象支持的操作：

操作结果datetime2 = datetime1 + timedelta日期加上一个时间间隔，返回一个新的日期对象（注1）datetime2 = datetime1 - timedelta日期减去一个时间间隔，相当于 datetime2 + timedelta == datetime1（注2）timedelta = datetime1 - datetime2两个日期相减得到一个时间间隔（注3）datetime1 < datetime2当 datetime1 的日期在 datetime2 之前时，我们认为 datetime1 < datetime2

注1：timedelta.day > 0 或 timedelta.day < 0 决定 datetime2 日期增长的方向；计算结果 datetime2 的 tzinfo 属性和 datetime1 相同；如果 date2.year < MINYEAR 或 date2.year > MAXYEAR，引发 OverflowError 异常

注2：计算结果 datetime2 的 tzinfo 属性和 datetime1 相同；这并不等同于 date1 + (-timedelta)，因为单独的 -timedelta 可能会溢出，而 date1 - timedelta 则不会溢出

注3：如果 datetime1 和 datetime2 的 tzinfo 属性一样（指向同一个时区），则 tzinfo 属性被忽略，计算结果为一个 timedelta 对象 t，则 datetime2 + t == datetime1（不用进行时区调整）；如果 datetime1 和 datetime2 的 tzinfo 属性不一样（指向不同时区），则 datetime1 和 datetime2 会先被转换为 UTC 时区时间，在进行计算，(datetime1.replace(tzinfo=None) - datetime1.utcoffset()) - (datetime2.replace(tzinfo=None) - datetime2.utcoffset())


datetime 实例方法：

datetime.date()
- 返回一个 date 对象datetime.time() - 返回一个 time 对象（tzinfo 属性为 None）

datetime.timetz()
- 返回一个 time() 对象（带有 tzinfo 属性）

datetime.replace([year[, month[, day[, hour[, minute[, second[, microsecond[, tzinfo]]]]]]]])
- 生成一个新的日期对象，用参数指定日期和时间代替原有对象中的属性

datetime.astimezone(tz=None)
- 传入一个新的 tzinfo 属性，返回根据新时区调整好的 datetime 对象

datetime.utcoffset()
- 如果 tzinfo 属性是 None，则返回 None；否则返回 self.tzinfo.utcoffset(self)

datetime.dst()
- 如果 tzinfo 属性是 None，则返回 None；否则返回 self.tzinfo.dst(self)

datetime.tzname()
- 如果 tzinfo 属性是 None，则返回 None；否则返回 self.tzinfo.tzname(self)

datetime.timetuple()
-  返回日期对应的 time.struct_time 对象（类似于 time 模块的 time.localtime()）

datetime.utctimetuple()
- 返回 UTC 日期对应的 time.struct_time 对象

datetime.toordinal()
- 返回日期对应的 Gregorian Calendar 日期（类似于 self.date().toordinal()）

datetime.timestamp()
- 返回当前时间的时间戳（类似于 time 模块的 time.time()）

datetime.weekday()
- 返回 0 ~ 6 表示星期几（星期一是 0，依此类推）

datetime.isoweekday() 
- 返回 1 ~ 7 表示星期几（星期一是1， 依此类推）

datetime.isocalendar() 
- 返回一个三元组格式 (year, month, day)

datetime.isoformat(sep='T')
- 返回一个 ISO 8601 格式的日期字符串，如 "YYYY-MM-DD" 的字符串

datetime.__str__()
- 对于 date 对象 d 来说，str(d) 相当于 d.isoformat()

datetime.ctime()
- 返回一个表示日期的字符串，相当于 time 模块的 time.ctime(time.mktime(d.timetuple()))

datetime.strftime(format)
- 返回自定义格式化字符串表示日期，下面有详解

datetime.__format__(format)
- 跟 datetime.strftime(format) 一样，这使得调用 str.format() 时可以指定 data 对象的字符串


关于 datetime 的综合应用：

# I love FishC.com!
>>> from datetime import datetime, date, time

# 使用 datetime.combine()
>>> d = date(2015, 8, 1)
>>> t = time(12, 30)
>>> datetime.combine(d, t)
datetime.datetime(2015, 8, 1, 12, 30)

# 使用 datetime.now() 或 datetime.utcnow()
>>> datetime.now()
datetime.datetime(2014, 8, 31, 18, 13, 40, 858954)
>>> datetime.utcnow()
datetime.datetime(2014, 8, 31, 10, 13, 49, 347984)

# 使用 datetime.srptime()
>>> dt = datetime.strptime("21/11/14 16:30", "%d/%m/%y %H:%M")
>>> dt
datetime.datetime(2014, 11, 21, 16, 30)

# 使用 datetime.timetuple()
>>> tt = dt.timetuple()
>>> for it in tt:
        print(it)

2014
11
21
16
30
0
4
325
-1

# ISO 格式的日期
>>> ic = dt.isocalendar()
>>> for it in ic:
        print(it)

2014
47
5

# 格式化 datetime 对象
>>> dt.strftime("%A, %d. %B %Y %I:%M%p")
'Friday, 21. November 2014 04:30PM'
>>> 'The {1} is {0:%d}, the {2} is {0:%B}, the {3} is {0:%I:%M%p}.'.format(dt, "day", "month", "time")
'The day is 21, the month is November, the time is 04:30PM.'
复制代码


带有 tzinfo 的 datetime 综合演示：

# 嘿，都能看到这里来了，毅力不错哈^_^
>>> from datetime import timedelta, datetime, tzinfo
>>> class GMT1(tzinfo):
        def utcoffset(self, dt):
                return timedelta(hours=1) + self.dst(dt)
        def dst(self, dt):
                # DST 开始于三月最后一个星期天
                # 结束于十月最后一个星期天
                d = datetime(dt.year, 4, 1)
                self.dston = d - timedelta(days=d.weekday() + 1)
                d = datetime(dt.year, 11, 1)
                self.dstoff = d - timedelta(days=d.weekday() + 1)
                if self.dston <= dt.replace(tzinfo=None) < self.dstoff:
                        return timedelta(hours=1)
                else:
                        return timedelta(0)
        def tzname(self, dt):
                return "GMT +1"
        
>>> class GMT2(tzinfo):
        def utcoffset(self, dt):
                return timedelta(hours=2) + self.dst(dt)
        def dst(self, dt):
                d = datetime(dt.year, 4, 1)
                self.dston = d - timedelta(days=d.weekday() + 1)
                d = datetime(dt.year, 11, 1)
                self.dstoff = d - timedelta(days=d.weekday() + 1)
                if self.dston <=  dt.replace(tzinfo=None) < self.dstoff:
                        return timedelta(hours=1)
                else:
                        return timedelta(0)
        def tzname(self, dt):
                return "GMT +2"
        
>>> gmt1 = GMT1()

# 夏令时
>>> dt1 = datetime(2014, 11, 21, 16, 30, tzinfo=gmt1)
>>> dt1.dst()
datetime.timedelta(0)
>>> dt1.utcoffset()
datetime.timedelta(0, 3600)
>>> dt2 = datetime(2014, 6, 14, 13, 0, tzinfo=gmt1)
>>> dt2.dst()
datetime.timedelta(0, 3600)
>>> dt2.utcoffset()
datetime.timedelta(0, 7200)

# 将 datetime 转换到另一个时区
>>> dt3 = dt2.astimezone(GMT2())
>>> dt3
datetime.datetime(2014, 6, 14, 14, 0, tzinfo=<__main__.GMT2 object at 0x036C0F70>)
>>> dt2
datetime.datetime(2014, 6, 14, 13, 0, tzinfo=<__main__.GMT1 object at 0x036C0B10>)
>>> dt2.utctimetuple() == dt3.utctimetuple()
True
复制代码





格式化字符串：strftime() 和 strptime()

date, datetime, 和 time 对象均支持使用 strftime(format) 方法，将指定的日期或时间转换为自定义的格式化字符串

相反的，datetime.strptime() 类方法却是把格式化字符串转换为 datetime 对象

格式化指令含义%a星期的简写（星期一 ~ 天：Mon, Tue, Wed, Thu, Fri, Sat, Sun）%A星期的全写（星期一 ~ 天：Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday）%w在一个星期中的第几天（ 0 表示星期天 ... 6 表示星期六）%d在一个月中的第几天（01, 02, ..., 31）%b月份的简写（一月 ~ 十二月：Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec）%B月份的全写（一月 ~ 十二月：January, February, March, April, May, June, July, August, September, October, November, December）%m月份（01, 02, ..., 12）%y用两个数字表示年份（例如 2014年 == 14）%Y用四个数字表示年份%H二十四小时制（00, 01, ..., 23）%I十二小时制（01, 02, ..., 11）%pAM 或者 PM%M分钟（00, 01, ..., 59）%S秒（00, 01, ..., 59）%f微秒（000000, 000001, ..., 999999）%z与 UTC 时间的间隔 ；如果是本地时间，返回空字符串（(empty), +0000, -0400, +1030）%Z时区名称；如果是本地时间，返回空字符串（(empty), UTC, EST, CST）%j在一年中的第几天（001, 002, ..., 366）%U在一年中的第几周，星期天作为第一天（00, 01, ..., 53）%W在一年中的第几周，星期一作为第一天（00, 01, ..., 53）%c用字符串表示日期和时间（Tue Aug 16 21:30:00 2014）%x用字符串表示日期（08/16/14）%X用字符串表示时间（21:30:00）%%表示百分号


格式化字符串综合演示：

>>> from datetime import datetime
>>> dt = datetime.now()
>>> print('(%Y-%m-%d %H:%M:%S %f): ', dt.strftime('%Y-%m-%d %H:%M:%S %f'))
(%Y-%m-%d %H:%M:%S %f):  2014-08-31 23:54:58 379804
>>> print('(%Y-%m-%d %H:%M:%S %p): ', dt.strftime('%y-%m-%d %I:%M:%S %p'))
(%Y-%m-%d %H:%M:%S %p):  14-08-31 11:54:58 PM
>>> print('%%a: %s ' % dt.strftime('%a'))
%a: Sun 
>>> print('%%A: %s ' % dt.strftime('%A'))
%A: Sunday 
>>> print('%%b: %s ' % dt.strftime('%b'))
%b: Aug 
>>> print('%%B: %s ' % dt.strftime('%B'))
%B: August 
>>> print('日期时间%%c: %s ' % dt.strftime('%c'))
日期时间%c: 08/31/14 23:54:58 
>>> print('日期%%x：%s ' % dt.strftime('%x'))
日期%x：08/31/14 
>>> print('时间%%X：%s ' % dt.strftime('%X'))
时间%X：23:54:58 
>>> print('今天是这周的第%s天 ' % dt.strftime('%w'))
今天是这周的第0天 
>>> print('今天是今年的第%s天 ' % dt.strftime('%j'))
今天是今年的第243天 
>>> print('今周是今年的第%s周 ' % dt.strftime('%U'))
今周是今年的第35周
复制代码













python 对于 时间的操作主要有time 和datetime 两个包。

time： 主要用来查看当前时间
time.time(): 从1970-01-01起到到当前的秒数
time.localtime(): 对time.time() 的封装，可以得到当前的年月日等信息（是一个time.struct_time类。可通过属性名访问到不同信息）
time.strftime('%Y'): 更加方便的格式化输出，其中对应关系：
%Y ---年
%m ---月
%d ----天
%H ----小时
%M ----分钟
%S -----秒
%z -----时区
%a ----英文周几的缩写
%A ----英文周几的全写
%b -----英文月份的缩写
%B ------英文月份的全写
%c -----英文的年月日输出（Wed Jul 11 13:32:32 2018）
%I -----
%p ----返回AM/PM


datetime: 主要用于时间的修改等

datetime.datetime.now():  当前时间（2018-07-11 13:41:19.676643）
datetime.datetime(year=2018, month=7,day=27): 得到指定时间的时间对象

datetime.timedalta(): 时间的偏移量如：
 datetime.timedelta(minutes=10)  
表示偏移10分钟（0:10:00），可以和其他时间对象加减等操作得到新的时间











