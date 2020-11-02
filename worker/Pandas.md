1.介绍
--------------------------------------------------------------------------------
在pandas中有两类非常重要的数据结构，即序列Series和数据框DataFrame。
Series类似于numpy中的一维数组，除了通吃一维数组可用的函数或方法，而且其可通过索引标签的方式获取数据，还具有索引的自动对齐功能；
DataFrame类似于numpy中的二维数组，同样可以通用numpy数组的函数和方法，而且还具有其他灵活应用。

1.不同于numpy
索引标签：列标签
自动换标签：当 序列/数据框 进行算术运算时，会自动对齐数据。（相同行标签/列标签之间进行运算，如一个没有相应标签，则计算结果后对应标签值为NaN）

默认设置
--------------------------------------------------------------------------------

np.get_option()查看对应的设置值如：np.get_option('display.max_rows')查看最大显示行数np.set_option()设置对应的设置值如：np.set_option('display.max_rows', 20)设置最大显示行数为20


2.Series
--------------------------------------------------------------------------------
类似于numpy中的一维数组，适用于一维数组的函数和方法。
而且可以通过索引标签的方式获取数据，还有索引的自动对齐功能。
1.创建

def __init__(self, data=None, index=None, dtype=None, name=None, copy=False, fastpath=False)
data: 读取的数据源，如：arrary、dict、list
index:  索引。
值必须是可哈希的。允许使用非唯一索引值。
如没有指定，会通过 range(len(data)) 生成。
如 数据源是dict，且同时指定 index ，则以index为准。
dtype: 数据类型，如未指定会自动推断出
name
copy: 是否复制源数据，默认为 False
fastpath

2.指标函数

count()非空元素计算min()最小值max()最大值idxmin()最小值的位置idxmax()最大值的位置quantile(0.1)10%分位数sum()求和mean()均值median()中位数mode()众数var()方差std()标准差mad()平均绝对偏差skew()偏度kurt()峰度describe()一次行输出多个指标值。Serise  DataFrame可用， 一维数组不可用

3.数据框 DataFrame
--------------------------------------------------------------------------------
类似于numpy中的二维数组，同样适用于numpy数组的函数和方法。

1.创建

def __init__(self, data=None, index=None, columns=None, dtype=None, copy=False)
data： array、dict
index：列索引
columns：行索引
dtype：数据类型
copy

dict 嵌套 dict 时： key 为列标签，嵌套key 为行标签。
dict 嵌套 list（arrary）时： key 为列标签， 行标签会自动生成 range(le(n))


4.方法
--------------------------------------------------------------------------------
apply()

5.缺失值处理
--------------------------------------------------------------------------------
pd.isnull( data ) ：检测数据中有多少缺失值
删除法当数据中的某个变量大部分值都是缺失值，可以考虑删除改变量；当缺失值是随机分布的，且缺失的数量并不是很多是，也可以删除这些缺失的观测。data.dropna(self, axis=0, how='any',       thresh=None, subset=None,inplace=False)删除缺失值，默认会删除任何含有缺失值的行1.axis：轴方向， 可是多个轴 组成的 tuple、list2.how：默认 any，如存在任何NA值，则删除             all：如全部值都为NA，则删除改标签3.thresh：int值：要求许多非NA值替补法对于连续型变量，如果变量的分布近似或就是正态分布的话，可以用均值替代那些缺失值；如果变量是有偏的，可以使用中位数来代替那些缺失值；对于离散型变量，我们一般用众数去替换那些存在缺失的观测。data.fillna(self, value=None, method=None,                axis=None, inplace=False,               limit=None, downcast=None,                **kwargs)1.value：用于填充的数值，可是是标量、dict、Series、DataFrame2.method：'backfill'，'bfill'，'pad'，'ffill'，None 可选。            ffill  前项填充            bfill  后项填充3.axis：轴插补法插补法是基于蒙特卡洛模拟法，结合线性模型、广义线性模型、决策树等方法计算出来的预测值替换缺失值。data.fillna()data.fillna( 0 )：全部用 0 填充data.fillna( method='bfill' )：用前一项数组填充data.fillna( method='ffill' )：使用后一项值填充data.fillna( {'x1': 1, 'x2': 2} )：指定不同的标签填充不同的值

6.多成索引
--------------------------------------------------------------------------------
s = pd.Series( np.arange(1, 10), index=[ ['a', 'a', 'a', 'b', 'b', 'c', 'c', 'd', 'd'], [1, 2, 3, 1, 2, 3, 1, 2, 3] ])

data = pd.DataFrame(np.random.randint(0,150,size=(8,12)),
                                  columns = pd.MultiIndex.from_product([ ['模拟考','正式考'],  ['数学','语文','英语','物理','化学','生物'] ]),
                                   index = pd.MultiIndex.from_product([['期中','期末'], ['雷军','李斌'], ['测试一','测试二']]))
Seriess['a']外层索引为 a 的数据s['a', 1]外层索引为 a 内层索引为 1 的数据s['a'][ [1, 3] ]外层外 a ，内层为 1、3 的数据s[ ['a', 'c'] ]外层为 a、c 的数据s['a': 'c']外成 a 到 c 的数据s.unstack()将 Series 变成 DataFrame
DataFrame
data = pd.DataFrame(np.random.randint(0,150,size=(8,12)),
                                  columns = pd.MultiIndex.from_product([ ['模拟考','正式考'], ['数学','语文','英语','物理','化学','生物'] ]),
                                   index = pd.MultiIndex.from_product([['期中','期末'], ['雷军','李斌'], ['测试一','测试二']]))


7.数据透视表
--------------------------------------------------------------------------------

def pivot_table(data, values=None, index=None, columns=None, aggfunc='mean',
                         fill_value=None, margins=False, dropna=True, margins_name='All')
data：数据源
values：指定需要聚合的数据字段。
index：指定某些原始变量作为行索引
columns：指定分组变量
aggfunc：聚合函数
fill_value：填充缺失值，默认不填充
margins：是否进行行或列的汇总，默认不汇总
dropna：是否删除缺失值
margins_name：默认行汇总或列汇总的名称为‘ALL’

函数的作用：从特定的角度去分析数据，得到结果（表格的形式）。
values：需要统计的数据
index：相当于结果的行名
columns： 相当于结果的列名。
如：pd.pivot_table(data, values='年龄', index='医院', columns='患病'， aggfunc=mean)
        是统计，不同医院，是否患病，的平均年龄


8.SQL操作
--------------------------------------------------------------------------------

1.增

增加行pd.concat(data1, data2)将data2 中的数据加到 data1 中，会自动对齐两个数据的变量，可指定拼接的轴Series.append()dataframe.loc[new_loc] = 1增加一行增加列pd.DataFrame(data,               columus=['name', 'age', 'sex', 'score'] )新增加一列 score由于新增列没有赋值， 会是 NaNdataframe.assige()
2.删

删除表（数据集）del data删除行data.drop( [1, 3, 5] )删除 1， 3， 5 行索引的行删除列data.drop( ['name', 'age'], axis=1) )布尔过滤删除data[ data['age'] > 12]过滤（删除）年龄小于12的数据行删除重复值data.drop_duplicates()可指定比较的列
drop(self, labels=None, axis=0, index=None, columns=None, level=None, inplace=False, errors='raise')
labels：要删除的轴 单个标签，或标签列表
axis：要删除的轴， int 或 轴名称
index， columns： 要删除的标签， 单个或多个组成的列表。指定 ( labels= [], axis=1 ) 就相当于指定 columns=lables。0 轴是行， 1 轴是列。
level：int或level name，默认为None 对于MultiIndex
inplace：bool，默认为False， 如果为True，则进行就地操作并返回None。
errors：'ignore', 'raise' 可选， 默认为 raise。如果“ignore”(忽略)，则禁止显示错误，并删除现有标签。

3.改
赋值
# 把数据集data 的 name 列中的 所有为 a 的值替换成 1， b 替换成 2， c 替换成 3
data['name'] = data['name'].map({'a': 1, 'b': 2, 'c': 3})  

data.replace()
data.rename()

dataframe.set_index()  # 设置索引

pd.cut()  # 分组



4.查

前几行。默认5data.head()后几行。默认5data.tail()随机几行，默认1data.sample()索引df.index列名df.columns数据df.valuesqueryisin()select()where()指定行data.loc[ [2, 5, 7] ]，( iloc() )loc （标签查找），iloc (索引查找)指定列data[ ['name', 'age', sex'] ]data.loc[ : , ['name', 'age', 'sex'] ]逻辑查询data[ (data['name'] == 'ben')  & (data['age'] >12) ]名字为ben且年龄大于12& 和、| 或每个条件必须用括号括起来查找重复数据data.duplicated()返回每一行是否有重复行。可通过 data.duplicated().any()查看数据集中是否有重复行删除重复行data.drop_duplicates(inplace=True)inplace=True表示就地执行。等价于： data = data.drop_duplicates（）（不需要从新赋值回去）查看每个属性的样本数data['name'].value_counts()按照 name 列分类返回每一类的数量
numpy：先行后列；data[1, 2] 第一行，第二列
pandas：Series---先外后内
               DataFrame--- data[列]
                                 ----data.loc[行]
               删除默认是行；pd.dorp(1) 删除第一行

5.聚合

def groupby(self, by=None, axis=0, level=None, as_index=True, sort=True,
                group_keys=True, squeeze=False, **kwargs)
by：mapping、function、str、iterable，用于确定 groupby 的组。如果是一个函数， 则会调用对象的每一个索引值
axis：轴
sort：默认为 True。对组建经行排序，关闭可以提升性能

data.groupby()
默认自动选择数值型数据进行聚合计算。
data.groupby( ['sex] ).mean()
按照性别分类，分别计算出 身高 体重 （数组型）数据的平均值。
data.groupby( ['sex] ).agg( [np.mean, np.median] )
计算多个统计量

6.排序

sort_index()    def sort_index(self, axis=0, level=None,                    ascending=True, inplace=False,                   kind='quicksort', na_position='last',                  sort_remaining=True, by=None):按照索引排序sort_values()    def sort_values(self, by, axis=0, ascending=True,              inplace=False,kind='quicksort',               na_position='last')按照索引值排序data.sort_values( by=['name', 'age'] )       优先name 再 age

7.多表连接
pd.merge（data1, data2, on='name'）

def merge(left, right, how='inner', on=None, left_on=None, right_on=None, 
                    left_index=False, right_index=False, sort=False, suffixes=('_x', '_y'),
                     copy=True, indicator=False, validate=None):

left  right：两个数据集'
how：连接方式， 默认为  inner--内连接，返回两张表中共同部分的数据
left：左连接，保留 左边 数据集的所有数据，同时匹配右边数据集的数据，匹配不上的数据为NaN
right：右连接
outer：外连接
on：关联的数据索引（主键）



9.文件的读取与存储
--------------------------------------------------------------------------------

1.读取
pd.read_csv() 等方法
header 参数为指定数据的标题行，默认为0行，如果没有可设置为None
parse_datas：是否要自动解析数据
index_col：索引列

2.存储
data.to_dict() 等


时间操作
--------------------------------------------------------------------------------

dp.timetamp()dp.to_datetime()

字符串操作
--------------------------------------------------------------------------------

df.str.strip()   修改数据中的所有字符串。。。。。


画图操作
--------------------------------------------------------------------------------

