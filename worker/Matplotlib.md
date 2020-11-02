1.简介
--------------------------------------------------------------------------------
主要有Image和pyplot。Image是对图片操作，pyplot是绘图的。
matplotlib.pyplot是一个用于python编程语言中的2D图形的绘图库。
它可以在python脚本，shell，Web应用程序服务器和其他图形用户界面工具包中使用。
有几个工具包可用于扩展python matplotlib功能。其中一些是单独的下载，其他可以与matplotlib源代码一起提供，但具有外部依赖性。
Basemap：这是一个地图绘图工具包，包含各种地图投影，海岸线和政治边界。
Cartopy：它是一个映射库，具有面向对象的地图投影定义，以及任意点，线，多边形和图像转换功能
Excel tool：Matplotlib提供与Microsoft Excel交换数据的实用程序。
Mplot3d：用于三维图。
Natgrid：它是natgrid库的接口，用于对间隔数据进行不规则网格化

基本操作流程：
使用plt.figure() 创建一个图像窗口。
使用用画图函数 （plt.plot() 等）画图。
使用 plt.show() 显示图像


from matplotlib import pyplot as plt

plt.figure(num = 2)
plt.plot([5,8,10], [12,16,6])

plt.show()


2.figure（图像窗口）
--------------------------------------------------------------------------------
设置窗口属性，命名，大小等

def figure(num=None, figsize=None, dpi=None, facecolor=None, edgecolor=None, 
           frameon=True, FigureClass=Figure, clear=False, **kwargs):

num：窗口名。整数或字符串。保存在图像对象的 ‘number’属性中。
如果为none，则为 1~N 的整数
如果已存在，因该保证是可用的
如果是数字，则窗口名为 Figure 1、Figure 1 等
如果为字符串，窗口名则为此字符串。
figsize：窗口大小
整数元组。默认为（6.4， 4.8）
dpi：整数
默认为 ‘无图解析’
facecolor：背景色
edgecolor：边框色
frameon：图框，布尔值。默认为True
FigureClass： `~matplotlib.figure.Figure`的子类， 可以选择使用自定义的`.Figure`实例。
clear：布尔值，默认为False。
如果为True并且该图已经存在，那么它将被清除。

3.设置坐标轴
--------------------------------------------------------------------------------
# 设置坐标轴范围
plt.xlim((-1, 2))
plt.ylim((-2, 3))

# 设置坐标轴名
plt.xlabel('I am x')
plt.ylabel('I am y')

# 设置轴刻度
plt.yticks([-2, -1.8, -1, 1.22, 3],[r'$really\ bad$', r'$bad$', r'$normal$', r'$good$', r'$really\ good$']) # xticks


# 获取轴信息
ax = plt.gca()

# 获得指定轴， left，right,bottom， top
x_top = ax.spines['top']  

# 设置轴
x_top.set_color((0,0,0,0))  # 颜色值为 RGBA,或 none
ax.xaxis.set_ticks_position('bottom')  # x轴的刻度显示在图像的下面，top，bottom，both，default，none
ax.spines['bottom'].set_position(('data', 0))  # 把 x轴设置在 y=0 的地方。outward，axes，data

xticks(ticks=None, labels=None, **kwargs)
ticks： 应该放置刻度的位置列表。 您可以传递一个空列表来禁用xticks
labels：要放置在给定* locs *的显式标签列表。（为每个刻度命名）
rotation：刻度显示的角度，0 为水平

4.设置图像标识（legend）
--------------------------------------------------------------------------------
   
 如下图红框中：

legend将要显示的信息来自于 label 值
l2, = plt.plot(x, y2, color='red', linewidth=1.0, linestyle='--', label='square line')
plt.legend(loc='upper right')  # 右上角

5.标注
--------------------------------------------------------------------------------
1.使用plt.plot()

标注出点(x0, y0)的位置信息. 画出一条垂直于x轴的虚线.
plt.plot([x0, x0,], [0, y0,], 'k--', linewidth=2.5)
# set dot styles
plt.scatter([x0, ], [y0, ], s=50, color='b')



2.使用plt.annotate 
plt.annotate(r'$2x+1=%s$' % y0, xy=(x0, y0), xycoords='data', xytext=(+30, -30),
             textcoords='offset points', fontsize=16,
             arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=.2"))
r'$2x+1=%s$'  ： 要注释的内容
xy=(x0, y0) ： 要注释的点
xytext=(+30, -30) ： 注释内容要放的位置
arrowprops ： 设置 箭头的属性
xycoords='data' 是说基于数据的值来选位置, 


3.使用plot.text()
plt.text(-3.7, 3, r'$This\ is\ the\ some\ text. \mu\ \sigma_i\ \alpha_t$',
         fontdict={'size': 16, 'color': 'r'})
-3.7, 3,是选取text的位置
空格需要用到转字符\ 
fontdict设置文本字体

6.坐标轴透明度
--------------------------------------------------------------------------------
当图片中的内容较多，相互遮盖时，我们可以通过设置相关内容的透明度来使图片更易于观察。

for label in ax.get_xticklabels() + ax.get_yticklabels():
    label.set_fontsize(12)
    # 在 plt 2.0.2 或更高的版本中, 设置 zorder 给 plot 在 z 轴方向排序
    label.set_bbox(dict(facecolor='white', edgecolor='None', alpha=0.7, zorder=2))

label.set_fontsize(12)重新调节字体大小，
bbox设置目的内容的透明度相关参，
facecolor调节 box 前景色，
edgecolor 设置边框， 本处设置边框为无，
alpha设置透明度.




7.不同类型图
--------------------------------------------------------------------------------
1.折线图
from matplotlib import pyplot as plt
from matplotlib import style  # 样式

style.use('ggplot')   # 使用特定的表格样式

x, y = [5,8,10], [12,16,6]
x2, y2 = [6,9,11], [6,15,7]

# plt.plot(x,y,color='b',linestyle=':',marker = 'o',markerfacecolor='r',markersize = 10)
plt.plot(x,y,'--', 'g',label='line one', linewidth=5)  # 第一条线，并设置为虚线
line = plt.plot(x2,y2,'r',label='line two',linewidth=5)  # 第二条线
# plt.setp(line,color='r',linewidth = 2.0, alpha = 0.4)

plt.title('Epic Info')  # 图标题
plt.ylabel('Y axis')  # y轴
plt.xlabel('X axis')  # x轴

plt.legend()  # 添加图例，标识不同颜色的线为什么线
plt.grid(True,color='k')  # 添加网格
plt.show()


不同的线条
字符类型字符类型'-'实线'--'虚线'-.'虚点线':'点线'.'点','像素点'o'圆点'v'下三角点'^'上三角点'<'左三角点'>'右三角点'1'下三叉点'2'上三叉点'3'左三叉点'4'右三叉点's'正方点'p'五角点'*'星形点'h'六边形点1'H'六边形点2'+'加号点'x'乘号点'D'实心菱形点'd'瘦菱形点'_'横线点

不同的颜色
字符颜色‘b’蓝色，blue‘g’绿色，green‘r’红色，red‘c’青色，cyan‘m’品红，magenta‘y’黄色，yellow‘k’黑色，black‘w’白色，white

所有可以的风格（plt.style.available）
['dark_background',
'seaborn-talk',
'seaborn-bright',
'seaborn-ticks',
'bmh',
'ggplot',
'seaborn-darkgrid',
'classic',
'fivethirtyeight',
'seaborn-deep',
'seaborn-colorblind',
'seaborn-muted',
'seaborn-pastel',
'seaborn-notebook',
'seaborn-paper',
'seaborn-dark-palette',
'seaborn-whitegrid',
'seaborn-white',
'grayscale',
'seaborn-dark',
'seaborn-poster']


2.条形图
from matplotlib import pyplot as plt

plt.bar([0.25,1.25,2.25,3.25,4.25],[50,40,70,80,20], label="BMW", color='b', width=.5)   # brah  水平条形图
plt.bar([.75,1.75,2.75,3.75,4.75],[80,20,20,50,60], label="Audi", color='r',width=.5) 

plt.legend()  
plt.xlabel('Days')  
plt.ylabel('Distance (kms)') 
plt.title('Information')  

plt.show()  

3.直方图
import matplotlib.pyplot as plt

population_age =  [22,55,62,45,21,22,34,42,42,4,2,102,95,85,55,110,120,70,65,55,111,115,80,75,65,54,44,43,42,48]
bins = [0,10,20,30,40,50,60,70,80,90,100]  # x轴

plt.hist(population_age, bins, histtype='bar', color='b', rwidth=0.8) 

plt.xlabel('age groups') 
plt.ylabel('Number of people')  
plt.title('Histogram')  
plt.show()

4.散点图
import matplotlib.pyplot as plt

x, y = [1,1.5,2,2.5,3,3.5,3.6], [7.5,8,8.5,9,9.5,10,10.5]  
x1, y1 =[8,8.5,9,9.5,10,10.5,11], [3,3.5,3.7,4,4.5,5,5.2]  

plt.scatter(x,y, label='high income low saving',color='r')
plt.scatter(x1,y1,label='low income high savings',color='b')

plt.xlabel('saving*100')
plt.ylabel('income*1000')
plt.title('Scatter Plot')
plt.legend()
plt.show()


5.面积图
import matplotlib.pyplot as plt

days = [1,2,3,4,5]  # x轴

sleeping =[7,8,6,11,7]  # 数据A的y轴数据
eating = [2,3,4,3,2]  # 数据B的y轴数据
working =[7,8,7,2,2]  # 数据C的y轴数据
playing = [8,5,7,8,13]  # 数据D的y轴数据

plt.plot([],[],color='m', label='Sleeping', linewidth=5)
plt.plot([],[],color='c', label='Eating', linewidth=5)
plt.plot([],[],color='r', label='Working', linewidth=5)
plt.plot([],[],color='k', label='Playing', linewidth=5)
plt.stackplot(days, sleeping,eating,working,playing, colors=['m','c','r','k'])

plt.xlabel('x')
plt.ylabel('y')
plt.title('Stack Plot')
plt.legend()
plt.show()


6.饼图
import matplotlib.pyplot as plt

days = [1,2,3,4,5]

sleeping =[7,8,6,11,7]
eating = [2,3,4,3,2]
working =[7,8,7,2,2]
playing = [8,5,7,8,13]

slices = [7,2,2,13]
activities = ['sleeping','eating','working','playing']
cols = ['c','m','r','b']

plt.pie(slices, labels=activities, colors=cols, startangle=90, shadow= True, explode=(0,0.1,0,0),  autopct='%1.1f%%')
plt.title('Pie Plot')
plt.show()


3D 图
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
ax = Axes3D(fig)

# X, Y value
X = np.arange(-4, 4, 0.25)
Y = np.arange(-4, 4, 0.25)
X, Y = np.meshgrid(X, Y)    # x-y 平面的网格
Z = -(X**2 + Y**2)

# ax.scatter(2, 2, 8, color='green')
# ax.plot([0, -2], [0, 2], [0, 8], 'r')

ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=plt.get_cmap('rainbow'))

plt.show()

盒图
import matplotlib.pyplot as plt
import numpy as np

tang_data = [np.random.normal(0,std,100) for std in range(1,4)]
fig = plt.figure(figsize = (8,6))
plt.boxplot(tang_data,notch=False,sym='s',vert=True)

plt.xticks([y+1 for y in range(len(tang_data))],['x1','x2','x3'])
plt.xlabel('x')
plt.title('box plot')
           



matplatlib 还提供了很多的图形，包括但不限于 等高线图、Image图、3D图等


3.多图合并
--------------------------------------------------------------------------------
import matplotlib.pyplot as plt

plt.figure()

plt.subplot(2,2,3)  # 把图像窗口分为 2行2列 的四个部分，选择当前使用的为第三个部分
plt.plot([0,1],[0,1])

plt.show()


subplot(*args, **kwargs)
subplot(nrows, ncols, index, **kwargs)
subplot(pos, **kwargs)
subplot(ax) # {None, 'aitoff', 'hammer', 'lambert', 'mollweide', 'polar', 'rectilinear', str}
args：
一个三位数，或三个独立的数。用来描述子图的位置。
如果三个整数按顺序为* nrows *，* ncols *和* index *，则子图将采用* nrows * rows和* ncols *列的网格上的* index *位置。 
* index *从左上角的1开始，向右增加。
第一个数为行，第二个数为列，第三个数为子图的索引


import matplotlib.pyplot as plt

plt.figure()

ax1 = plt.subplot2grid((3, 3), (0, 0), colspan=3, rowspan=1)#把窗口分为3行3列，使用（0， 0）部分，列跨度为3个，行跨度为1个
ax1.plot([1, 2], [1, 2])    # 画小图
ax1.set_title('ax1_title')  # 设置小图的标题

plt.show()




还可以使用 gridspec，subplots 实现，还可用图中图、第二坐标，动图等


参考：https://morvanzhou.github.io/tutorials/data-manipulation/plt/

4.缺点
--------------------------------------------------------------------------------
他们非常依赖其他软件包，例如NumPy。
它只适用于python，因此很难或不可能在python以外的语言中使用。（但它可以从Julia通过PyPlot包使用）


5.保存图片
--------------------------------------------------------------------------------
plt.savefig('D:\test.png')

savefig(fname, dpi=None, facecolor='w', edgecolor='w',
             orientation='portrait', papertype=None, format=None,
             transparent=False, bbox_inches=None, pad_inches=0.1,
             frameon=None, metadata=None)

fname：文件名或类文件对象
如果没有指定扩展名，则会从文件名中推导出
format：文件扩展名，支持png,pdf,ps,eps,svg
dpi：分辨率，为None时使用默认值
facecolor：背景色，RGBA值
edgecolor：边框色
orientation：方向，可选{'landscape'，'portrait'}
papertype：
transparent：透明，布尔值
bbox_inches：
quality：图片质量，None 或 1~100，仅在格式为jpg或jpeg时可用。


6.中文字体
--------------------------------------------------------------------------------
# from pylab import mpl
import matplotlib as mpl

mpl.rcParams['font.sans-serif'] = ['FangSong']  # 指定默认字体
mpl.rcParams['axes.unicode_minus'] = False  # 解决保存图像,负号'-'显示为方块的问题

