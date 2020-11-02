## Python 单元测试

- 测试就是对一段代码（一个函数、一个模块等）的评估。
- 通过输入不同的值，对结果的正确性进行判断。如果不是“理想”值，要么是被测试的代码有问题，要么是测试的输入值有问题。

<!-- more -->

#### 实现方法

**1.**`print`

- 最简单的方法是通过 print。把代码的运行结果 print 出来，进行**人工**判断。
- 优点：简单；缺点：粗暴。



**2.** `function or model`

- 通过编写一个函数或者组织一个模块，对特定的代码进行测试。
- 简单的例子：
```python
def add(arg1, arg2):
    return arg1 + arg2
 
def test_add():
    if add(2, 3) != 5:
        raise 

if __name__ == '__main__':
    test_add()
```

- 当然这样也可以达到测试的目的，但是从代码量、复用性还是从后续的执行和维护。都无疑添加了很大的工作量。



**3.**framework

- 使用测试框架，本质上也是方法2，只不过更加的方便简洁。
- 现流行的框架有很多。各有各的特点。根据不同的需求选择不同的框架就可以。



### Framework----unittest

------















### 参考

[官方文档](https://docs.python.org/3/library/unittest.html)

[csdn](https://blog.csdn.net/huilan_same/article/details/52944782)

