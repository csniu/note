首先，Python是一种解释性型语言，是一种语言规范。规范有不同的实现方式，就有了不同的解释器，如 CPython、Pypy、Jython 等。其中CPython是C语言实现，是应用范围最广，也是官方的推荐的，我们在安装python时默认安装的解释器就是它。这叫**解释器环境**。

其次，python在运行时的环境变量也会不同，这里主要说的是”包“的不同。这叫**运行环境**。

还有就是在编辑工程中的编辑器不同。这叫**编辑环境**。

<!-- more -->

# 解释器环境
解释器环境还可以根据其实现语言和python版本细分。

前面也说了CPython是官方推荐的也是应用最广的，所有没有特殊需要就不要换了。

由于python2和python3的不兼容，导致出现了两种不同的编程方式。好在python2到python3的过渡基本完成，所有如果没有特别需求，使用python3就好。

## 多版本共存
可能你会需要在不同python版本测试你的代码，这就需要你同时安装不同的python版本。你可以参考[pyenv](https://github.com/pyenv/pyenv)，还有其和virtualenv的组合版本[pyenv-virtualenv](https://github.com/pyenv/pyenv-virtualenv)。

# 运行环境
这里主要说的是”包“环境的不同。比如你有两个项目都需要第三方包`requests`，但是一个项目需要其1.0版本，另一个项目需要其2.0版本。这时就要保证不同项目的环境完全独立互不干扰才行。

## virtualenv
这时一般推荐的是[virtualenv](https://virtualenv.pypa.io/en/latest/)，它的原理是把系统中的python环境复制一份到virtualenv的环境。每次使用需要进入该环境，使用完需要退出环境。基本使用如下：
```
pip install virtualenv  # 安装

mkdir myproject # 新建路径
cd myproject 

virtualenv  --no-site-packages venv  
# 创建环境到当前路径下 venv 中
# no-site-packages 不复制系统python环境中的所有第三方包

source venv/bin/activate  # 进入环境
...
deactivate  # 退出环境
```

在使用virtualenv过程中还有两个问题：
- 每一个虚拟环境都是安装在指定的路径下，不便于管理。
- 使用虚拟环境需要激活和退出，才能进行其他的操作，如包的安装的程序的运行。

基于这两个问题，又有不同的解决方案。

[virtualenvwrapper](https://virtualenvwrapper.readthedocs.io/en/latest/index.html) 可以把所有的虚拟环境放在一个地方方便管理。基本使用：
```
# 安装
pip install virtualenvwrapper
export WORKON_HOME=~/Envs
source /usr/local/bin/virtualenvwrapper.sh
# pip install virtualenvwrapper-win # Windows 版

mkvirtualenv my_project # 创建环境，会在 ~/Envs 中创建 my_project 文件夹。

workon my_project  # 激活环境

deactivate  # 退出环境

rmvirtualenv my_project  # 删除环境
```

[Pipenv](https://pipenv.readthedocs.io/en/latest/)相当于是pip和virtualenv的合成版。尽管pip可以进行包管理，但是pipenv是一种更高级的工具，可以简化依赖关系的管理。

##### 安装
```
pip install pipenv
```

##### 创建环境
```
pipenv install  
```
- 会在当前路径下创建`Pipfile`和`Pipfile.lock`文件。
    - `Pipfile`: 用来记录包的依赖关系，取代原`requirements.txt`文件。
    - `Pipfile.lock`: 是记录通过hash算法将包的名称和版本以及其依赖关系生成哈希值，以确保包的完整性。
    - 在提交项目时应将这两个文件一并提交。
- 可选参数
    - --three: 使用系统环境中的python3初始化。
    - --two：使用系统环境中的python2初始化。
    - --python: 使用系统环境中的python指定版本初始化。
    - 不可同时使用，系统环境中要有对应版本。
- 所有环境都安装在`~/.local/share/virtualenvs/`目录下。

##### 安装包
```
pipenv install <package>
```
- 如果环境（Pipfile）不存在，则会创建一个。如果存在则会安装并更新Pipfile文件。
- --dev: 可选参数。安装到开发环境中。对应Pipfile文件中的[dev-packages]下，否则对应Pipfile中的[packages]下。

##### 使用
```
pipenv shell  # 激活交互环境。
exit  # 退出环境

pipenv run python main.py  # 运行脚本
```

##### 其他命令
```
pipenv uninstall <package>  
pipenv update [package]
pipenv lock -r --dev > requirements.txt
pipenv install -r requirements.txt
 pipenv --rm   # 删除环境
 pipenv --where
 pipenv --venv
 pipenv graph  # 显示当前环境包的依赖图。
 pipenv lock  #申明式生成 Pipfile.lock 文件
```
当然也还有一些其他的管理工具如[virtualenv-burrito](https://github.com/brainsik/virtualenv-burrito)、[autoenv](https://github.com/inishchith/autoenv)等可供选择。


## venv
上面说的都是基于virtualenv的，virtualenv同时支持python2和python3。但是从pyhon3.3开始就内置了一个虚拟环境工具[venv](https://docs.python.org/3/library/venv.html)。其和virtualenv比较类似，但是不支持python2。

##### 创建环境
```
python3 -m venv env 
```
- 会在当前路径下创建env目录。

##### 激活环境
```
Posix：source ./env/bin/activate
Windows cmd: ./env/Scripts/activate.bat
Windows PowerShell: ./env/Scripts/Activate.ps1
Windows GitBash: source ./env/Script/activate
```

##### 退出环境
```
deactivate
```

与virtualenv相比venv更加轻量级，但是python社区更倾向于使用virtualenv。

## conda & Anaconda
conda 是”包“及其依赖项和环境的管理工具。适用于不同语言，如Python、R、Ruby、Java等等。主要解决”包“的依赖关系和环境管理。

Anaconda 是集成了python、conda以及180+的科学包和其依赖的发行版。所有一般情况下如果需要，直接安装Anaconda就好。


##### 安装

直接到[官网](https://anaconda.org/)下载安装就好。安装完成后，还需要对所有工具包进行升级 `conda upgrade --all`（需要管理员权限）。

##### 包管理
```
conda install package_name
conda remove package_name
conda update packge_name
conda list
```

##### 环境管理
```
conda create -n env_name packages
source activate env_name
source deactivate
conda env remove -n env_name
conda env list
conda env export >tt.yaml
conda env create -f tt.yaml
```

###  推荐方案
- 推荐使用Pipenv，当然使用Venv也可以。
- 有科学计算需要的可以使用Anaconda。



# 开发工具
任何能够编辑普通文本的编辑器都能够用来编写Python代码，然后，使用一个更加强大的编辑器会使您的生活变得容易点。

常见的文本编辑器有：`Vim`、`Sublime Text`、`Atom`等可供选择。

IDEs有：`PyCharm / IntelliJ IDEA`、`Visual Studio Code`、`Eclipse`、`Spyder`等

其他：IDLE（python自带）、IPython。

### 推荐方案
- 脚本开发可以使用 `Vim`或`Sublime Text`，都可以，主要看个人习惯。
- 项目开发可以使用 `PyCharm`,专为python开发。
- 多语言，多功能可以使用`Visual Studio Code`。（推荐）
- 交互式开发可以用`IPython`。
