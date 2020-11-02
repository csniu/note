
# 定义类

```python
# -*- coding: utf-8 -*-
import time

class Person(object):
    '''所有的（中国）上海人民'''
    birthplace = '上海'
    interest = '甜点'
    __country = '中国'
    def __init__(self, name, sex, birth):
        self.name = name
        self.sex = sex
        self._birth = birth

    def get_age(self):
        tm_year = time.localtime().tm_year
        age = tm_year - self._birth
        return age
```

- 在 python 定义类使用关键字 class。
- 类的命名一般遵守驼峰命名法。
- 类也一个对象。

<!-- more -->

**新式类与经典类**

新式类和经典类的最主要区别就是继承的问题，经典类是深度优先，新式类是广度优先。

当然如果利用的是 Python3.x 并且也不打算使用 Python2.x 的版本，也可以忽略这个问题。

在 **Python2.x** 中默认都是经典类，只有显示继承 object 才是新式类。如下：

- class ClassName(object): pass   新式类
- class ClassName(): pass  经典类
- class ClassName: pass  经典类

在 **Python3.x**中取消了经典类，默认都是新式类，也不必显示继承 object。如下：

- class ClassName(object): pass
- class ClassName(): pass 
- class ClassName: pass
- 以上三种写法都是新式类，但是推荐第一张写法。

## 代码分解

```python
class Person(object):
```
- 通过关键字定义 Person 类，并且继承 object

```python
'''所有的（中国）上海人民'''
```
- 类的文档字符串，可通过 Person.\_\_doc\_\_ 访问或者 p.\_\_doc\_\_ (p 为 Person的一个实例)。

```python
    birthplace = '上海'
    interest = '甜点'
    __country = '中国'
```
- 类属性，是所有实例对象所共有的。

```python
    def __init__(self, name, sex, birth):
        self.name = name
        self.sex = sex
        self._birth = birth
```
- 类的构造器函数

```python
        self.name = name
        self.sex = sex
        self._birth = birth
```
- 实例属性，每个实例所特有的属性，在实例化时需要提供的。

```python
    def get_age(self):
        tm_year = time.localtime().tm_year
        age = tm_year - self._birth
        return age
```
- 类方法。第一的参数必须是 self 表示实例自身，其他和普通函数相同。

## 公有属性、私有属性、保护属性、特殊属性、类属性、实例属性

**1. 公有属性**就是在类外可以方法的属性，如上例的 birthplace、interest、name、sex 属性，如下通过点访问：

```python
p = Person(name='张三', sex='男', birth=1990)
print(p.birthplace)
print(p.interest)
print(p.name)
print(p.sex)
```

**2. 私有属性**就是在类的外不不能访问或不想被外部访问的属性，以双下划线开头的属性，如上例中的 \_\_country，如下

```python
print(p.__country)
```

`p.__country` 会抛出 AttributeError 错误。

严格来说 python 没有完全限制你访问属性，`__country` 只是被转换成 `_Person__country` ,通过 `p._Person__country` 以然可以访问私有属性。

**3. 保护属性**是以单下划线开头的属性，如上例中的 \_birth。如下：

```python
print(p._birth)
```

保护属性当然不是真的受保护属性，不让你从外部访问。只是一种约定，以单下划线开头的属性只是类内部私有的，不提供给外部使用。当然了，如果你一定要用也没有办法阻止你。

**4. 特殊属性**就是 python 内置的一些类属性，具有特殊意义的属性。以双下划线开头和结尾的属性。如上例的 \_\_doc\_\_ 。如下：

```python
print(p.__doc__)
print(Person.__doc__)
```

`__doc__` 表示类的文档字符串。通过类属性或实例属性都可以访问。

类似的还有如：`__dict__ ` : 类的所有属性组成的字典，`__module__`：类定义所在的模块等。

**5. 类属性**是类对象的属性 ，也是所有实例对象的共有属性。不是定义在构造器类的属性。如上例的 birthplace、interest、\_\_country。类属性也是可以通过继承获得和覆盖的。

类属性是类实例化后，实例默认就带有的属性。可以通过类对象访问，也可以通过实例对象访问，如下：

```python
print(Person.birthplace)
print(p.birthplace)
```

重新赋值

1. 通过类对象重新赋值，所有实例对象的属性也会改变。如下：
```python
p = Person(name='张三', sex='男', birth=1990)
print(p.birthplace)  # 上海
Person.birthplace = '北京'
print(p.birthplace)  # 北京
```

2. 通过实例对象重新赋值，python 会该实例创建一个同名的实例属性，并且实例属性会强制匹配掉类属性。如下：

```python
p = Person(name='张三', sex='男', birth=1990)
print(p.birthplace)  # 类属性，上海
p.birthplace = '北京'
print(p.birthplace)  # 实例属性， 北京
del p.birthplace  # 删除实例属性
print(p.birthplace)  # 类属性， 上海
```

**6. 实例属性**就是定义在构造器内的属性。在实例化时需要提供的属性（也可以有默认值），如上例中的 name、sex、\_birth。

- `hasattr(obj, attr)`：判断对象是否具有指定属性/方法；
- `getattr(obj, attr[, default])` 获取属性/方法的值, 要是没有对应的属性则返回 default 值（前提是设置了 default），否则会抛出 AttributeError 异常；
- `setattr(obj, attr, value)`：设定该属性/方法的值，类似于 obj.attr=value；

## 类方法、实例方法、静态方法、特殊方法

```python
# -*- coding: utf-8 -*-
import time

class Person(object):
    '''所有的（中国）上海人民'''
    birthplace = '上海'
    interest = '甜点'
    __country = '中国'
    instance_number = 0
    def __init__(self, name, sex, birth):
        self.name = name
        self.sex = sex
        self._birth = birth
        Person.instance_number = Person.instance_number + 1

    def get_age(self, *args, **kwargs):
        tm_year = time.localtime().tm_year
        age = tm_year - self._birth
        return age

    @staticmethod
    def get_time(*args, **kwargs):  
        print('Class Attribute: ', Person.birthplace)
        print(time.time())
        print('Args: ', args)
        print('Kwargs: ', kwargs)

    @classmethod
    def get_instance_number(cls, *args, **kwargs):
        print('Class: ', cls)
        print('Args: ', args)
        print('Kwargs: ', kwargs)
        print('Instance Number: ', cls.instance_number)  

p = Person(name='张三', sex='男', birth=1990)   
```



1. 实例方法： 第一个参数为 self 的方法，除了类方法和静态方法都是实例方法。self 表示实例本身。如上例中get_age 方法。只能通过实例对象调用。如下：

```python
p = Person(name='张三', sex='男', birth=1990)
print(p.get_age())
print(Person.get_age())  # 会报错
```

2. 类方法： 使用 @classmethod 装饰器装饰，并且第一个参数为类对象。如上例中的 get_instance_number 方法。在类方法中无法访问实例的属性和方法。可以通过类对象或实例对象调用，如下：

```python
print(Person.get_instance_number())
print(p.get_instance_number())
```

3. 静态方法：使用 @staticmethod 装饰器装饰，并且没有隐含参数。如上例中的 get_time 方法。在静态方法中因为没有隐含参数（类或实例对象），所有在静态方法中无法访问实例的属性和方法，如过要访问类方法或属性要通过类对象引用，如上例。可以通过类对象或实例对象调用，如下：

```python
print(Person.get_time())
print(p.get_time())
```

4. 特殊方法：就是 python 内置的一些方法，也属于以上三种之一。以双下划线开头和结尾的方法。有很多的魔法方法后面再聊。如上例的 `__init__`方法。

## 类的继承

继承就是在不需要重新编写的情况下，能够使用现有类的方法的方法。被继承的类被称为父类或基类，需要继承的类称为子类或派生类。最大优势在于多态。

继承的功能：

1. 获得父类的方法或属性。
2. 修改或覆盖父类的方法或属性。
3. 增加新的方法或属性。

**1.获得父类的方法或属性**

```python
class Person(object):
    legs = 2
    __star = '地球'

    def __init__(self, name):
        self.name = name

    def talk(self):
        return 'blabla'

class Chinese(Person):
    pass

p = Chinese('zhangsh')
print(p.legs)  
print(p._Person__star)  # 注意私有属性的转换名
print(p.talk())
print(p.name)
```

\# 这样在子类中不需要进行任何的操作就可以获得父类的所有方法。

**2. 修改父类的方法或属性**

```python
class Alien(Person):
    __star = '火星'

    def __init__(self, name, sex):
        super(Alien, self).__init__(name)
        print(self.name)
        self.sex = sex
        if self.sex == '男':
            self.name = '大卫'

    def talk(self):
        content = super(Alien,self).talk()
        print(content)
        return 'You need a Python!'

et = Alien('张三', '男')
print(et.name)
print(et.talk())
```

- super(Alien, self)：返回 Alien 的父类对象。如果不使用 super 方法，则会完全覆盖父类的对应功能。
- 类属性直接覆盖就可以。
- 实例属性，需要先继承再重构
- 修改方法是也是先先继承再重构

**3.添加新的属性或功能**

直接在子类中进行定义就可以。注意父类中如果存在`__init__`方法，如上。

### 多重继承

继承既然有这么多的好处，那如果需要的功能在多个类中，要怎么办昵？

这个时候就需要用到多重继承了。使用方法都和单继承一样，需要注意的是子类调用父类方法时的查找顺序问题。

MRO（Method Resolution Order），方法解析顺序。代表的类继承的顺序。可通过类方法**`mro()`**查看。

在新式类中是广度优先，经典类中是深度优先。

#### super 原理

```python
def super(cls, inst):
    mro = inst.__class__.mro()
    return mro[mro.index(cls) + 1]
```

- 首先获取实例类的 MRO 顺序列表。
- 查找 cls 类的序号。
- 返回序号加一的类，上一个父类。

所有说调用super方法并不是简单的返回父类，而是通过一定的算法（C3）实现广度优先的调用。

更加详细的介绍可以自行 Google 菱形继承或super()，示例如下：

```python
class A():
    def __init__(self):
        print("进入A…")
        print("离开A…")

class B(A):
    def __init__(self):
        print("进入B…")
        super().__init__()
        print("离开B…")
    
class C(A): 
    def __init__(self):
        print("进入C…")
        super().__init__()
        print("离开C…")
 
class D(B, C):
    def __init__(self):
        print("进入D…")
        super().__init__()
        print("离开D…")

d = D()
# 进入D…
# 进入B…
# 进入C…
# 进入A…
# 离开A…
# 离开C…
# 离开B…
# 离开D…
```



# 类的魔法方法

魔法方法就是能够给类添加一些“魔法”的特殊方法，通过以双下划线开头和结尾。最常用的应该就是 `__init__`方法了。

## 类的实例化

一般认为python 类的实例化操作首先是调用 `__init__` 方法，但是其实不是。首先调用的是 `__new__`方法，然后再调用 `__init__`方法。（在调用 `__new__`之前还有吗？）

`__new__`: 构造器，类实例化之前调用的方法，将类作为第一个参数传入，并且返回类的实例。

`__init__`: 初始化操作，根据`__new__`方法返回的实例（第一个参数 self）进行一些初始化操作，但是不返回任何对象（None）。

python 类实例化的顺序：

- 调用`__new__`方法，根据参数进行一些操作，并返回类实例。
- 调用`__init__`方法，将 `__new__` 方法方法的实例作为第一个参数传入，其他参数原封不动的传入（即使在 `__new__` 方法有改变，也不会影响传入值。）

```python
class number(float):
    def __new__(cls, *args, **kwargs):
        print('进入 new 方法！')
        print('args: ', args, kwargs)
        args = ['3.65']
        return float.__new__(cls, *args, **kwargs)

    def __init__(self, *args, **kwargs):
        print('进入 init 方法！')
        print('##',type(self))
        print(args, kwargs)
        self.arg = args
        self.kw = kwargs

f = number('3.4')
print(f)
print(f.arg)
print(f.kw)
```

如下是一个只能实例化一次类（实例化多次也是返回一个实例）：

```python
class Singleton(object):
    """单例"""
    _instance = None
    def __new__(cls, *args, **kwargs):
        if not cls._instance:
            cls._instance = super(Singleton, cls).__new__(cls, *args, **kwargs)
        return cls._instance
class MySingleton(Singleton):
    pass
a = MySingleton()
b = MySingleton()
print(a is b)	# True
```

## 其他魔法方法

python 中还有很多类似的魔法方法。

有操作符类，可以进行操作符重载，从而实现一些运算方法。

有类的表示方面的，可以实现不同的方法调用展现出不同的形式。

还有对属性控制方面的，可以控制在访问属性、删除属性、设置属性以及属性不存在的情况下的行为。

以及上下文管理的魔法方法。等等，如下表，详情请看参考连接或官方文档。

| **魔法方法**                                     | **含义**                                                     |
| ------------------------------------------------ | ------------------------------------------------------------ |
|                                                  | **基本的魔法方法**                                           |
| `__new__(cls[, ...])`                            | 1. __new__ 是在一个对象实例化的时候所调用的第一个方法2. 它的第一个参数是这个类，其他的参数是用来直接传递给 __init__ 方法3. __new__ 决定是否要使用该 __init__ 方法，因为 __new__ 可以调用其他类的构造方法或者直接返回别的实例对象来作为本类的实例，如果 __new__ 没有返回实例对象，则 __init__ 不会被调用4. __new__ 主要是用于继承一个不可变的类型比如一个 tuple 或者 string |
| `__init__(self[, ...])`                          | 构造器，当一个实例被创建的时候调用的初始化方法               |
| `__del__(self)`                                  | 析构器，当一个实例被销毁的时候调用的方法                     |
| `__call__(self[, args...])`                      | 允许一个类的实例像函数一样被调用：x(a, b) 调用 x.__call__(a, b) |
| `__len__(self)`                                  | 定义当被 len() 调用时的行为                                  |
| `__repr__(self)`                                 | 定义当被 repr() 调用时的行为                                 |
| `__str__(self)`                                  | 定义当被 str() 调用时的行为                                  |
| `__bytes__(self)`                                | 定义当被 bytes() 调用时的行为                                |
| `__hash__(self)`                                 | 定义当被 hash() 调用时的行为                                 |
| `__bool__(self)`                                 | 定义当被 bool() 调用时的行为，应该返回 True 或 False         |
| `__format__(self, format_spec)`                  | 定义当被 format() 调用时的行为                               |
|                                                  | **有关属性**                                                 |
| **`__getattr__(self, name)`**                    | 定义当用户试图获取一个不存在的属性时的行为， **# 防止死循环  （1）** |
| **`__getattribute__(self, name)`**               | 定义当该类的属性被访问时的行为，**不论属性存不存在， 先****__getattribute__ 后__getattr__** |
| `__setattr__(self, name, value)`                 | 定义当一个属性被设置时的行为                                 |
| `__delattr__(self, name)`                        | 定义当一个属性被删除时的行为                                 |
| `__dir__(self)`                                  | 定义当 dir() 被调用时的行为                                  |
| `__get__(self, instance, owner)`                 | 定义当**描****述符**的值被取得时的行为                       |
| `__set__(self, instance, value)`                 | 定义当**描述符**的值被改变时的行为                           |
| `__delete__(self, instance)`                     | 定义当**描述符**的值被删除时的行为                           |
|                                                  | **比较操作符**                                               |
| `__lt__(self, other)`                            | 定义小于号的行为：x < y 调用 x.__lt__(y)                     |
| `__le__(self, other)`                            | 定义小于等于号的行为：x <= y 调用 x.__le__(y)                |
| `__eq__(self, other)`                            | 定义等于号的行为：x == y 调用 x.__eq__(y)                    |
| `__ne__(self, other)`                            | 定义不等号的行为：x != y 调用 x.__ne__(y)                    |
| `__gt__(self, other)`                            | 定义大于号的行为：x > y 调用 x.__gt__(y)                     |
| `__ge__(self, other)`                            | 定义大于等于号的行为：x >= y 调用 x.__ge__(y)                |
|                                                  | **算数运算符**                                               |
| `__add__(self, other)`                           | 定义加法的行为：+                                            |
| `__sub__(self, other)`                           | 定义减法的行为：-                                            |
| `__mul__(self, other)`                           | 定义乘法的行为：*                                            |
| `__truediv__(self, other)`                       | 定义真除法的行为：/                                          |
| `__floordiv__(self, other)`                      | 定义整数除法的行为：//                                       |
| `__mod__(self, other)`                           | 定义取模算法的行为：%                                        |
| `__divmod__(self, other)`                        | 定义当被 divmod() 调用时的行为                               |
| `__pow__(self, other[, modulo])`                 | 定义当被 power() 调用或 ** 运算时的行为                      |
| `__lshift__(self, other)`                        | 定义按位左移位的行为：<<                                     |
| `__rshift__(self, other)`                        | 定义按位右移位的行为：>>                                     |
| `__and__(self, other)`                           | 定义按位与操作的行为：&                                      |
| `__xor__(self, other)`                           | 定义按位异或操作的行为：^                                    |
| `__or__(self, other)`                            | 定义按位或操作的行为：\|                                     |
|                                                  | **反运算**   （a+b , python 会先找a 的 add 方法， 如果a 没有， 则会在b 中找 radd 方法（反运算方法）） |
| `__radd__(self, other)`                          | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rsub__(self, other)`                          | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rmul__(self, other)`                          | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rtruediv__(self, other)`                      | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rfloordiv__(self, other)`                     | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rmod__(self, other)`                          | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rdivmod__(self, other)`                       | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rpow__(self, other)`                          | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rlshift__(self, other)`                       | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rrshift__(self, other)`                       | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rand__(self, other)`                          | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__rxor__(self, other)`                          | （与上方相同，当左操作数不支持相应的操作时被调用）           |
| `__ror__(self, other)`                           | （与上方相同，当左操作数不支持相应的操作时被调用）           |
|                                                  | **增量赋值运算**                                             |
| `__iadd__(self, other)`                          | 定义赋值加法的行为：+=                                       |
| `__isub__(self, other)`                          | 定义赋值减法的行为：-=                                       |
| `__imul__(self, other)`                          | 定义赋值乘法的行为：*=                                       |
| `__itruediv__(self, other)`                      | 定义赋值真除法的行为：/=                                     |
| `__ifloordiv__(self, other)`                     | 定义赋值整数除法的行为：//=                                  |
| `__imod__(self, other)`                          | 定义赋值取模算法的行为：%=                                   |
| `__ipow__(self, other[, modulo])`                | 定义赋值幂运算的行为：**=                                    |
| `__ilshift__(self, other)`                       | 定义赋值按位左移位的行为：<<=                                |
| `__irshift__(self, other)`                       | 定义赋值按位右移位的行为：>>=                                |
| `__iand__(self, other)`                          | 定义赋值按位与操作的行为：&=                                 |
| `__ixor__(self, other)`                          | 定义赋值按位异或操作的行为：^=                               |
| `__ior__(self, other)`                           | 定义赋值按位或操作的行为：\|=                                |
|                                                  | **一元操作符**                                               |
| `__pos__(self)`                                  | 定义正号的行为：+x                                           |
| `__neg__(self)`                                  | 定义负号的行为：-x                                           |
| `__abs__(self)`                                  | 定义当被 abs() 调用时的行为                                  |
| `__invert__(self)`                               | 定义按位求反的行为：~x                                       |
|                                                  | **类型转换**                                                 |
| `__complex__(self)`                              | 定义当被 complex() 调用时的行为（需要返回恰当的值）          |
| `__int__(self)`                                  | 定义当被 int() 调用时的行为（需要返回恰当的值）              |
| `__float__(self)`                                | 定义当被 float() 调用时的行为（需要返回恰当的值）            |
| `__round__(self[, n])`                           | 定义当被 round() 调用时的行为（需要返回恰当的值）            |
| `__index__(self)`                                | 1. 当对象是被应用在切片表达式中时，实现整形强制转换2. 如果你定义了一个可能在切片时用到的定制的数值型,你应该定义 __index__3. 如果 __index__ 被定义，则 __int__ 也需要被定义，且返回相同的值 |
|                                                  | **上下文管理（with 语句）**                                  |
| `__enter__(self)`                                | 1. 定义当使用 with 语句时的初始化行为2. __enter__ 的返回值被 with 语句的目标或者 as 后的名字绑定 |
| `__exit__(self, exc_type, exc_value, traceback)` | 1. 定义当一个代码块被执行或者终止后上下文管理器应该做什么2. 一般被用来处理异常，清除工作或者做一些代码块执行完毕之后的日常工作 |
|                                                  | **容器类型**                                                 |
| `__len__(self)`                                  | 定义当被 len() 调用时的行为（返回容器中元素的个数）          |
| `__getitem__(self, key)`                         | 定义获取容器中指定元素的行为，相当于 self[key]               |
| `__setitem__(self, key, value)`                  | 定义设置容器中指定元素的行为，相当于 self[key] = value       |
| `__delitem__(self, key)`                         | 定义删除容器中指定元素的行为，相当于 del self[key]           |
| `__iter__(self)`                                 | 定义当迭代容器中的元素的行为                                 |
| `__reversed__(self)`                             | 定义当被 reversed() 调用时的行为                             |
| `__contains__(self, item)`                       | 定义当使用成员测试运算符（in 或 not in）时的行为             |

# 元类

***写在前面，元类是一种非常高级的“魔法”，一般情况下是用不到的。当然最好也别轻易的尝试，除非你真的明白了其中的原理。当然也只是介绍了一些基础知识和自己的理解。***

在大多数编程语言中，类就是一组用来描述如何生成一个对象的代码段。但是，Python中的类还远不止如此。

**类同样也是一种对象。**这个对象（类）自身拥有创建对象（类实例）的能力，而这就是为什么它是一个类的原因。但它的本质仍对象，是对象就可以对它进行一些操作，如赋值、拷贝、当作参数传递等。

既然类也是对象，那类是通过什么创建的昵？

**元类就是用来创建类的类。**如Python 内置的 type 函数，本质上就是一个元类，是python默认用来创建所有类的元类。

>type(object_or_name, bases, dict)
>
>object_or_name:  类 或者是一个表示类名的字符串，
>
>bases: 元组类，表示类的基类。
>
>dict: 字典类型，用来存放该类的所有属性和方法。
>
>```python
>classname = type('classname', (), {'name': '张三'})  # 创建一个类
>print(classname)
>print(classname.name)
>```

## 类的创建过程

1. 当python解释器看到 class 时，会首先解析 `class ...`中的内容。解析基类的信息，主要是找到 `__metaclass__`属性。查找的顺序如下：

>```python
>class Foo(Bar):
>    pass
>```
>
>1. 首先会在 Foo 类中查收是否有定义 `__metaclass__`属性，如果没有则到其父类中查找。
>2. 重复上述步骤。
>3. 如果所有父类中都没定义 `__metaclass__`属性，则会到模块层中查找，并尝试同样的操作。
>4. 如果还是没有找到，则会使用内置的 type 元类。

2. 找到元类后，python 需要准备 namespace，如果元类中定义了 `__prepare__` ，则会通过调用它来得到 namespace。
3. 之后是调用 `exec` 来执行类的 body，包括属性和方法的定义，最后这些定义会被保存进 namespace。
4. 上述步骤结束后，就得到了创建类需要的所有信息，这时 Python 会调用元类的构造函数来真正创建类。

## 自定义元类

元类最主要的作用就是为了在创建类时能够自动的改变类。

```python
# 元类会自动将你通常传给‘type’的参数作为自己的参数传入
def upper_attr(future_class_name, future_class_parents, future_class_attr):
    '''返回一个类对象，将属性都转为大写形式'''
    #  选择所有不以'__'开头的属性
    attrs = ((name, value) for name, value in future_class_attr.items() 
        if not name.startswith('__'))
     # 将它们转为大写形式
    uppercase_attr = dict((name.upper(), value) for name, value in attrs)
    # 通过'type'来做类对象的创建
    return type(future_class_name, future_class_parents, uppercase_attr)
 
__metaclass__ = upper_attr  #  这会作用到这个模块中的所有类
 
class Foo(object):
    # 我们也可以只在这里定义__metaclass__，这样就只会作用于这个类中
    bar = 'bip'
```

\# 在这个模块中定义的所有类都会通过 upper_attr(元类)创建。

```python
class UpperAttrMetaclass(type):
    def __new__(cls, name, bases, dct):
        attrs = ((name, value) for name, value in dct.items() 
                 if not name.startswith('__'))
        uppercase_attr = dict((name.upper(), value) for name, value in attrs)
        return type.__new__(cls, name, bases, uppercase_attr)
```

\# 其他类只需要继承该类。因为该类的父类（type）定义了`__metaclass__`属性。

元类的本质很简单，难点在于使用到元类的问题都很复杂。主要是用途是创建API，如Django ORM。

>1. 拦截类的创建。
>2. 修改类。
>3. 返回修改后的类。



# 参考

[Python 文档](https://docs.python.org/zh-cn/3/tutorial/classes.html)

[python3-cookbook](https://python3-cookbook.readthedocs.io/zh_CN/latest/chapters/p08_classes_and_objects.html)

[深刻理解Python中的元类(metaclass)](http://blog.jobbole.com/21351/)

[What are metaclasses in Python?](https://stackoverflow.com/questions/100003/what-are-metaclasses-in-python)

[Python中的类属性、实例属性与类方法、静态方法](https://blog.csdn.net/qq_31780525/article/details/72639491)

[Python魔法方法指南](https://pyzh.readthedocs.io/en/latest/python-magic-methods-guide.html#id28)

[Python技术进阶——魔法方法](http://kaito-kidd.com/2017/02/22/python-magic-methods/)

[Python 元类 (MetaClass) 小教程](https://lotabout.me/2018/Understanding-Python-MetaClass/)

[Python: super 没那么简单](https://mozillazg.com/2016/12/python-super-is-not-as-simple-as-you-thought.html)