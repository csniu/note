一、简介
--------------------------------------------------------------------------------
NumPy（Numerical Python的简称）是Python数值计算最重要的基础包。大多数提供科学计算的包都是用NumPy的数组作为构建基础。
NumPy的部分功能如下：
ndarray，一个具有矢量算术运算和复杂广播能力的快速且节省空间的多维数组。
用于对整组数据进行快速运算的标准数学函数（无需编写循环）。
用于读写磁盘数据的工具以及用于操作内存映射文件的工具。
线性代数、随机数生成以及傅里叶变换功能。
用于集成由C、C++、Fortran等语言编写的代码的A C API。
NumPy本身并没有提供多么高级的数据分析功能，理解NumPy数组以及面向数组的计算将有助于你更加高效地使用诸如pandas之类的工具。
对于大部分数据分析应用而言，我最关注的功能主要集中在：
用于数据整理和清理、子集构造和过滤、转换等快速的矢量化数组运算。
常用的数组算法，如排序、唯一化、集合运算等。
高效的描述统计和数据聚合/摘要运算。
用于异构数据集的合并/连接运算的数据对齐和关系型数据运算。
将条件逻辑表述为数组表达式（而不是带有if-elif-else分支的循环）。
数据的分组运算（聚合、转换、函数应用等）。。
NumPy之于数值计算特别重要的原因之一，是因为它可以高效处理大数组的数据。
import numpy as np
my_arr = np.arange(1000000)
my_list = list(range(1000000))
基于NumPy的算法要比纯Python快10到100倍（甚至更快），并且使用的内存更少。


二、ndarray
--------------------------------------------------------------------------------
1.说明
NumPy的ndarray是一种多维数组对象。是一个大数据集容器，可对整块数据执行一些数学运算。
所有元素必须是相同类型的。
每个数组都有一个shape（一个表示各维度大小的元组）和一个dtype（一个用于说明数组数据类型的对象）

2.创建

注意：认为np.empty会返回全0数组的想法是不安全的。很多情况下（如前所示），它返回的都是一些未初始化的垃圾值。

3.数据类型
--------------------------------------------------------------------------------
除非特别说明（稍后将会详细介绍），np.array会尝试为新建的这个数组推断出一个较为合适的数据类型，存储在dtype中
dtype（数据类型）是一个特殊的对象，它含有ndarray将一块内存解释为特定数据类型所需的信息

    


3.属性
shape： 一个表示各维度大小的元组(arr2.shape)
dtype： 一个用于说明数组数据类型的对象（data.dtype）
ndim： 数组的维度（arr2.ndim)
T： 原数组的转置数组， 原数组的视图
H： 矩阵的共轭转置


4.方法

astype()astype(self, dtype, order='K',            casting='unsafe', subok=True,            copy=True)casting控制可能发生的数据转换类型。 默认为'不安全'为了向后兼容。'no', 'equiv', 'safe', 'same_kind', 'unsafe' 可选将浮点数转换成整数时，会发生截取删除。（直接删除小数部分）dtype：可以从枚举中指定，也可从其他 ndarray 对象上得到将 ndarray 对象的数据类型转换成指定数据类型。默认是重新复制一份copy()拷贝一份原数据，互不影响transpose( (1, 0) )参数--轴编号组成的元组表示把原 0 轴转换成原 1号轴， 原 1号轴转换成 原 0 号轴。nonzero（）返回非空元素的坐标。是一个元组包含多个array 的结构。每一个arrar 对应数据的一个轴any()检测数组中是否有一个或多个 Trueall()检测数组中是否全部为 Truetang_array.clip(2,4)限制数据中的元素的最大和最小值， 大于4 的 都为4，小于2 的都为2tang_array.round(decimals=1)四舍五入， decimals 精度squeeze（）压缩，删除为空的轴np.concatenate((a,b))拼接数据，可指定拼接轴np.vstack((a,b))拼接np.hstack((a,b))拼接a.flatten()多维转换为 1维（拉平）a.ravel()同上np.linspace(0,10,10)np.logspace(0,1,5)np.meshgrid(x,y)np.zeros(3)np.ones((3,3))np.empty(6)a.fill(1)np.zeros_like(tang_array)np.ones_like(tang_array)np.identity(5)np.set_printoptions(precision = 2)设置打印精度，数量等np.pad(tang_array,pad_width = 1,mode = 'constant',constant_values = 0)边缘填充在数据 tang_array 外加一圈 全0 的数据np.nonzero（）找到所有不为0的元素索引np.unravel_index(100,(6,7,8))shape为（6，7，8）的矩阵，并找到第100个元素的索引值np.datetime64（）日期np.arange('2017-10','2017-11',dtype='datetime64[D]')z.flags.writeable = False设置 数据 z 为不可修改np.floor(z)取整数部分

5.运算

数组的广播
如果两个数组的维度不同， 则元素到元素的操作时不可能的。但在Numpy 中是可行的，Numpy 会把较小的数组广播到较大的数组大小。
满足条件
1：让所有输入数组都向其中shape最长的数组看齐，shape中不足的部分都通过在前面加1补齐
2：输出数组的shape是输入数组shape的各个轴上的最大值
3：如果输入数组的某个轴和输出数组的对应轴的长度相同或者其长度为1时，这个数组能够用来计算，否则出错
4：当输入数组的某个轴的长度为1时，沿着此轴运算时都用此轴上的第一组值
对两个数组，分别比较他们的每一个维度（若其中一个没有当前维度则忽略），满足：
            1：当前维度的值相等
            2：当前维度的值有一个是1
输出数组的维度是每个维度的最大值，广播将值为1 的维度进行 ”复制 拉伸“
参考：http://scipy.github.io/old-wiki/pages/EricsBroadcastingDoc


内积
计算两个矩阵的内积， np.dot(arr1, arr2)

np.multiply(x,y)np.dot(x,y)np.logical_and(x,y)np.logical_or(x,y)np.logical_not(x,y)



6.基础操作

查索引--[行, 列]（得到是视图）arr[0]一维：第一个数据多维：低一维的 ndarray 数据arr[0, 0]第一行 第一列等价于： arr[0][0]arr[1:3]一维：切片  第二 和 第三个数据多维：切片 第二 和 第三 行的所有数据arr[1:3, 1:3]多维切片， 第一到第三行， 第一到第三列 的数据布尔索引（副本）arr[ [True, False, True] ]布尔型数组的长度必须和被索引的轴长一致。第一和第三行花式索引（副本）arr[ [1, 3, 5] ]不论多少，得到的总是一个一维的数组，把说有数据嵌套在一个list中第一 三 五 行切片切边的到的数据都是原数据的视图， 即 改变切片中的数据，原数据也会改变支持 &  | 等布尔算术运算符，python 的 and  or 等无效增删改赋值广播赋值转置方法 transpose（）属性 T


三、random
--------------------------------------------------------------------------------

python 内置的random 模块生成的随机数，是伪随机数，因为他们都是通过算法基于 随机数生成种子， 在确定性的条件下生成的。
可以用 np.random.seed() 更改随机数生成种子。seed 确定随机数生成器的种子
np.random 的数据生成函数使用的是全局的随机种子。
permutation     返回一个序列的随机排列或返回一个随机排列的范围。shuffle对一个序列就地随机排列rand生成随机的0-1的浮点数randint从给定的上下限范围内随机选取整数randn生成正态分布（平均值为 0， 标准差为1）的样本值， 类似于MATLAB 接口binomial生成二项分布的样本值normal生成正态（高斯）分布的样本值beta生成 Beta 分布的样本值chisquare生成卡方分布的样本值gamma生成 Gamma 分布的样本值uniform生成在[0， 1]中均匀分布的样本值

四、计算函数
--------------------------------------------------------------------------------

一元abs  fabs计算整数、浮点数或复数的绝对值，对于非复数值，可以使用更快的 fabssqrt计算各元素的平方根， 相当于 arr ** 0.5square计算各元素的平方， 相当于 arr ** 2exp计算各元素的指数e xlog log10 log2 log1p分别为自然对数（低数为e）、底数为10的log、底数为2的log、log(1+x)sign计算各元素的正负号， 正（1）负（-1） 零（0）ceil计算各元素的ceiling值， 向上取整floor计算个元素的floor 值， 向下取整arccos  arccosh arcsinarcsinh  arctan  arctanh反三角函数二元logical_not计算各元素的 not x 的真正， 相当于 -xadd将数组中对应的元素相加subtract从第一个数组中减去第二个数组中的元素multiply数组元素相乘divide  floor_divide除法， 整除power第一数组中的A, 第二个数组中的B, 计算 A 的 B 次方maximum  fmax元素级的最大值计算。 fmax 忽略 NaNminimum  fmin元素级最小值， fmin 忽略 NaNmod元素级求模计算（除法的余数）copysign将第二个数组中的值的符号复制给第一个数组中的值

五、数据处理函数
--------------------------------------------------------------------------------


where(condition, x=None, y=None)第一个是array类型， 二三 可是array或 标量都可以三个参数都给原理：遍历第一参数的所有元素，如果为True 则选择 X 或 X 中对应的元素，否则选择 Y 或Y 中对应的元素。if condition : X else: Y只给第一个参数返回 condition.nonzero()，非空值的坐标mean()平均值sum()求和std()标准差var()方差max()最大值argmax()最大元素的索引min()最小值argmin()最小元素的索引cumsum()累加函数cumprod()累积函数np.bincount(data['name'])返回数据中，每个元素出现的次数（元素必须是非零整数），还可以指定权重
都可以指定一个参数：axis，表示要计算的轴方向。 如： axis = 0
即是顶级函数，也是类方法


六、排序
--------------------------------------------------------------------------------

np.sort()sort(a, axis=-1, kind='quicksort',           order=None)返回排序的副本a： 需要排序的数组axis： 轴编号，默认最后一个轴kill：排序算法，  {'quicksort', 'mergesort', 'heapsort'} 可选order：排序的字段， str 或 listnp.argsort()argsort(a, axis=-1, kind='quicksort',              order=None使用指定算法沿给定轴执行  间接排序， 并返回一个形状相同的索引数组。np.lexsort()np.lexsort([-1*tang_array[:,0],tang_array[:,2]])第一列（[:,0])）降序（-1），第三列（[:, 2]）升序()np.searchsorted(tang_array,values)把values 数据插入 tang_array 数据中，tang_array 要是排序号的，values 的数据会按排序的顺序插入到对应的位置中

七、集合函数
--------------------------------------------------------------------------------

np.unique()(ar, return_index=False, return_inverse=False, return_counts=False, axis=None)唯一化（去重）ar：序数组return_index：是否返回 去重后的数据的 索引号return_inverse：是否返回所有数据的索引号。（重复的元素都以第一次出现的索引号为准）return_counts：是否返回元素的次数np.intersect1d(x, y)计算 x, y 中的公共元素，并返回有序结果union1d(x, y)并集in1d（x, y）得到一个表示 “x 的元素是否包含于 y” 的布尔型数组setdiff1d(x, y)集合的差， 即元素在 x 中，但不在 y 中setxor1d(x, y)对称差， 即存在于一个数组中，但不同时存在两个数组中的元素

八、文件的输入输出
--------------------------------------------------------------------------------

读取数据np.load(file)从二进制文件中读取数据（.npy 文件）np.loadtxt()loadtxt(fname, dtype=float, comments='#', delimiter=None, converters=None, skiprows=0,  usecols=None, unpack=False, ndmin=0, encoding='bytes')fname：file\str\pathlib.Path:要读取的文件，文件名、生成器。如果文件扩展名是“.gz"、".bz2",首先对文件经行解压。dtype：结果数组的数据类型。默认为 float()comments：注释。默认为”#“delimiter：分隔符，默认为空格converters：转换器， dict。将列号映射到要转换的函数的字典skiprows：跳过第一个'skiprows`行;默认值：0。usecols：要读取数据的那些行。int或sequence。usecols =（1,4,5）将提取第2列，第5列和第6列。默认值None，导致读取所有列。unpack：解包：bool，可选， 如果为True，则返回的数组被转置，因此参数可以是。使用``x，y，z = loadtxt（...）``解压缩。与结构化一起使用时 data-type，为每个字段返回数组。默认值为False。ndmin：返回的数组至少具有`ndmin`维度。否则将挤压单维轴。合法值：0（默认值），1或2。encoding：用于解码输入文件的编码。不适用于输入流。特殊值'bytes'启用向后兼容性解决方法，确保您在可能的情况下接收字节数组作为结果并传递，latin1将字符串编码为转换器。覆盖此值以接收unicode数组并将字符串作为输入传递给转换器。如果设置为“无”。使用系统默认值。默认值为'bytes'。读取 CSV 文件时：由于 csv 是以逗号分隔的文件，所以如果表格内容中出现其他逗号，则会报错如果表格内容有缺失值，会报错应该指定分隔符： delimiter=','保存数组save()save(file, arr, allow_pickle=True, fix_imports=True)file：要保存到的文件，如果是字符串会自动添加‘.npy’arr：要保存的数组savetxt()savetxt(fname, X, fmt='%.18e', delimiter=' ', newline='\n', header='', ooter='', comments='# ', encoding=None)fname：文件名或文件句柄，如果文件名以``.gz``结尾，则文件会自动保存压缩gzip格式。X：要保存到文本文件的数据fmtdelimiter：分隔符newline：换行符header：将在文件开头写入的字符串footer：页脚， 将在文件末尾写入的字符串。comments：评论：str，可选。将被添加到``header``和``footer``字符串的字符串，将它们标记为评论。默认值：'＃'encoding： {None，str}，可选， 用于编码输出文件的编码。不适用于输出，流。如果编码不是'bytes'或'latin1'， 您将无法在NumPy版本<1.14中加载该文件。默认是'latin1'。savez()savez(file, *args, **kwds)np.savez('filename.npz', a=arr1, b=arr2)np.savez('filename', arr1, arr2)如果没有关键字，则会自动指定  arr_0  arr_1 等变量名将多个数组保存到一个  为压缩的文件中。savez_compressed()类似于 savez()， 但是保存为压缩文件。

九、线性代数函数
--------------------------------------------------------------------------------

diag以一维数组的形式返回方阵的对角线（或非对角线）元素。或将一维数组转换为方阵（非对角线元素为0）dot矩阵乘法trace计算对角线元素和det计算矩阵行列式eig计算方阵的本征值和本征向量inv计算方阵的逆pinv计算矩阵的 Moore-Penrose 伪逆qr计算QR分解svd计算奇异值分解（SVD）solve解线性方程组 Ax = b，其中A 为一个方阵lstsq计算 Ax = b 的最小二乘解


