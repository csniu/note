以下是 Python 内置异常类的层次结构:

BaseException
+-- SystemExit
+-- KeyboardInterrupt
+-- GeneratorExit
+-- Exception
      +-- StopIteration
      +-- ArithmeticError
      |    +-- FloatingPointError
      |    +-- OverflowError
      |    +-- ZeroDivisionError
      +-- AssertionError
      +-- AttributeError
      +-- BufferError
      +-- EOFError
      +-- ImportError
      +-- LookupError
      |    +-- IndexError
      |    +-- KeyError
      +-- MemoryError
      +-- NameError
      |    +-- UnboundLocalError
      +-- OSError
      |    +-- BlockingIOError
      |    +-- ChildProcessError
      |    +-- ConnectionError
      |    |    +-- BrokenPipeError
      |    |    +-- ConnectionAbortedError
      |    |    +-- ConnectionRefusedError
      |    |    +-- ConnectionResetError
      |    +-- FileExistsError
      |    +-- FileNotFoundError
      |    +-- InterruptedError
      |    +-- IsADirectoryError
      |    +-- NotADirectoryError
      |    +-- PermissionError
      |    +-- ProcessLookupError
      |    +-- TimeoutError
      +-- ReferenceError
      +-- RuntimeError
      |    +-- NotImplementedError
      +-- SyntaxError
      |    +-- IndentationError
      |         +-- TabError
      +-- SystemError
      +-- TypeError
      +-- ValueError
      |    +-- UnicodeError
      |         +-- UnicodeDecodeError
      |         +-- UnicodeEncodeError
      |         +-- UnicodeTranslateError
      +-- Warning
           +-- DeprecationWarning
           +-- PendingDeprecationWarning
           +-- RuntimeWarning
           +-- SyntaxWarning
           +-- UserWarning
           +-- FutureWarning
           +-- ImportWarning
           +-- UnicodeWarning
           +-- BytesWarning
           +-- ResourceWarning


AssertionError  断言语句（assert）失败
AttributeError  尝试访问未知的对象属性
EOFError    用户输入文件末尾标志EOF（Ctrl+d）
FloatingPointError  浮点计算错误
GeneratorExit   generator.close()方法被调用的时候
ImportError 导入模块失败的时候
IndexError索引超出序列的范围
KeyError字典中查找一个不存在的关键字
KeyboardInterrupt用户输入中断键（Ctrl+c）
MemoryError内存溢出（可通过删除对象释放内存）
NameError尝试访问一个不存在的变量
NotImplementedError尚未实现的方法
OSError操作系统产生的异常（例如打开一个不存在的文件）
OverflowError数值运算超出最大限制
ReferenceError弱引用（weak reference）试图访问一个已经被垃圾回收机制回收了的对象
RuntimeError一般的运行时错误
StopIteration迭代器没有更多的值
SyntaxErrorPython的语法错误
IndentationError缩进错误
TabErrorTab和空格混合使用
SystemErrorPython编译器系统错误
SystemExitPython编译器进程被关闭
TypeError不同类型间的无效操作
UnboundLocalError访问一个未初始化的本地变量（NameError的子类）
UnicodeErrorUnicode相关的错误（ValueError的子类）
UnicodeEncodeErrorUnicode编码时的错误（UnicodeError的子类）
UnicodeDecodeErrorUnicode解码时的错误（UnicodeError的子类）
UnicodeTranslateErrorUnicode转换时的错误（UnicodeError的子类）
ValueError传入无效的参数ZeroDivisionError除数为零