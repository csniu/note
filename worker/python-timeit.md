timeit 模块详解 -- 准确测量小段代码的执行时间

timeit 模块提供了测量 Python 小段代码执行时间的方法。它既可以在命令行界面直接使用，也可以通过导入模块进行调用。该模块灵活地避开了测量执行时间所容易出现的错误。

以下例子是命令行界面的使用方法：
$ python -m timeit '"-".join(str(n) for n in range(100))'
10000 loops, best of 3: 40.3 usec per loop
$ python -m timeit '"-".join([str(n) for n in range(100)])'
10000 loops, best of 3: 33.4 usec per loop
$ python -m timeit '"-".join(map(str, range(100)))'
10000 loops, best of 3: 25.2 usec per loop
复制代码

以下例子是 IDLE 下调用的方法：
>>> import timeit
>>> timeit.timeit('"-".join(str(n) for n in range(100))', number=10000)
0.8187260627746582
>>> timeit.timeit('"-".join([str(n) for n in range(100)])', number=10000)
0.7288308143615723
>>> timeit.timeit('"-".join(map(str, range(100)))', number=10000)
0.5858950614929199
复制代码

需要注意的是，只有当使用命令行界面时，timeit 才会自动确定重复的次数。


timeit 模块

该模块定义了三个实用函数和一个公共类。


timeit.timeit(stmt='pass', setup='pass', timer=<default timer>, number=1000000)

创建一个 Timer 实例，参数分别是 stmt（需要测量的语句或函数），setup（初始化代码或构建环境的导入语句），timer（计时函数），number（每一次测量中语句被执行的次数）

注：由于 timeit() 正在执行语句，语句中如果存在返回值的话会阻止 timeit() 返回执行时间。timeit() 会取代原语句中的返回值。


timeit.repeat(stmt='pass', setup='pass', timer=<default timer>, repeat=3, number=1000000)

创建一个 Timer 实例，参数分别是 stmt（需要测量的语句或函数），setup（初始化代码或构建环境的导入语句），timer（计时函数），repeat（重复测量的次数），number（每一次测量中语句被执行的次数）


timeit.default_timer()

默认的计时器，一般是 time.perf_counter()，time.perf_counter() 方法能够在任一平台提供最高精度的计时器（它也只是记录了自然时间，记录自然时间会被很多其他因素影响，例如计算机的负载）。


class timeit.Timer(stmt='pass', setup='pass', timer=<timer function>) 

计算小段代码执行速度的类，构造函数需要的参数有 stmt（需要测量的语句或函数），setup（初始化代码或构建环境的导入语句），timer（计时函数）。前两个参数的默认值都是 'pass'，timer 参数是平台相关的；前两个参数都可以包含多个语句，多个语句间使用分号（;）或新行分隔开。

第一次测试语句的时间，可以使用 timeit() 方法；repeat() 方法相当于持续多次调用 timeit() 方法并将结果返回为一个列表。

stmt 和 setup 参数也可以是可供调用但没有参数的对象，这将会在一个计时函数中嵌套调用它们，然后被 timeit() 所执行。注意，由于额外的调用，计时开销会相对略到。


- timeit(number=1000000) 

功能：计算语句执行 number 次的时间。

它会先执行一次 setup 参数的语句，然后计算 stmt 参数的语句执行 number 次的时间，返回值是以秒为单位的浮点数。number 参数的默认值是一百万，stmt、setup 和 timer 参数由 timeit.Timer 类的构造函数传递。

注意：默认情况下，timeit() 在计时的时候会暂时关闭 Python 的垃圾回收机制。这样做的优点是计时结果更具有可比性，但缺点是 GC（garbage collection，垃圾回收机制的缩写）有时候是测量函数性能的一个重要组成部分。如果是这样的话，GC 可以在 setup 参数执行第一条语句的时候被重新启动，例如：
timeit.Timer('for i in range(10): oct(i)', 'gc.enable()').timeit()
复制代码


- repeat(repeat=3, number=1000000) 

功能：重复调用 timeit()。

repeat() 方法相当于持续多次调用 timeit() 方法并将结果返回为一个列表。repeat 参数指定重复的次数，number 参数传递给 timeit() 方法的 number 参数。

注意：人们很容易计算出平均值和标准偏差，但这并不是非常有用。在典型的情况下，最低值取决于你的机器可以多快地运行给定的代码段；在结果中更高的那些值通常不是由于 Python 的速度导致，而是因为其他进程干扰了你的计时精度。所以，你所应感兴趣的只有结果的最低值（可以用 min() 求出）。


- print_exc(file=None) 

功能：输出计时代码的回溯（Traceback）

典型的用法：
t = Timer(...)       # outside the try/except
try:
    t.timeit(...)    # or t.repeat(...)
except Exception:
    t.print_exc()
复制代码

标准回溯的优点是在编译模板中，源语句行会被显示出来。可选的 file 参数指定将回溯发送的位置，默认是发送到 sys.stderr。


命令行界面

当被作为命令行程序调用时，可以使用下列选项：
python -m timeit [-n N] [-r N] [-s S] [-t] [-c] [-h] [statement ...]
复制代码

各个选项的含义：

选项原型含义-n N--number=N执行指定语句（段）的次数-r N--repeat=N重复测量的次数（默认 3 次）-s S--setup=S指定初始化代码或构建环境的导入语句（默认是 pass）-p--process测量进程时间而不是实际执行时间（使用 time.process_time() 代替默认的 time.perf_counter()）以下是 Python3.3 新增：-t--time使用 time.time()（不推荐）-c--clock使用 time.clock()（不推荐）-v--verbose打印原始的计时结果，输出更大精度的数值-h--help打印一个简短的用法信息并退出


示例

以下演示如果在开始的时候设置初始化语句：

命令行：

$ python -m timeit -s 'text = "I love FishC.com!"; char = "o"'  'char in text'
10000000 loops, best of 3: 0.0877 usec per loop
$ python -m timeit -s 'text = "I love FishC.com!"; char = "o"'  'text.find(char)'
1000000 loops, best of 3: 0.342 usec per loop
复制代码

使用 timeit 模块：
>>> import timeit
>>> timeit.timeit('char in text', setup='text = "I love FishC.com!"; char = "o"')
0.41440500499993504
>>> timeit.timeit('text.find(char)', setup='text = "I love FishC.com!"; char = "o"')
1.7246671520006203
复制代码

使用 Timer 对象：
>>> import timeit
>>> t = timeit.Timer('char in text', setup='text = "I love FishC.com!"; char = "o"')
>>> t.timeit()
0.3955516149999312
>>> t.repeat()
[0.40193588800002544, 0.3960157959998014, 0.39594301399984033]
复制代码


以下演示包含多行语句如何进行测量：

（我们通过 hasattr() 和 try/except 两种方法测试属性是否存在，并且比较它们之间的效率）

命令行：
$ python -m timeit 'try:' '  str.__bool__' 'except AttributeError:' '  pass'
100000 loops, best of 3: 15.7 usec per loop
$ python -m timeit 'if hasattr(str, "__bool__"): pass'
100000 loops, best of 3: 4.26 usec per loop

$ python -m timeit 'try:' '  int.__bool__' 'except AttributeError:' '  pass'
1000000 loops, best of 3: 1.43 usec per loop
$ python -m timeit 'if hasattr(int, "__bool__"): pass'
100000 loops, best of 3: 2.23 usec per loop
复制代码

使用 timeit 模块：
>>> import timeit
>>> # attribute is missing
>>> s = """\
... try:
...     str.__bool__
... except AttributeError:
...     pass
... """
>>> timeit.timeit(stmt=s, number=100000)
0.9138244460009446
>>> s = "if hasattr(str, '__bool__'): pass"
>>> timeit.timeit(stmt=s, number=100000)
0.5829014980008651
>>>
>>> # attribute is present
>>> s = """\
... try:
...     int.__bool__
... except AttributeError:
...     pass
... """
>>> timeit.timeit(stmt=s, number=100000)
0.04215312199994514
>>> s = "if hasattr(int, '__bool__'): pass"
>>> timeit.timeit(stmt=s, number=100000)
0.08588060699912603
复制代码


为了使 timeit 模块可以测量你的函数，你可以在 setup 参数中通过 import 语句导入：

def test():
    """Stupid test function"""
    L = [i for i in range(100)]

if __name__ == '__main__':
    import timeit
    print(timeit.timeit("test()", setup="from __main__ import test"))
复制代码
