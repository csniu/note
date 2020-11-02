## Blog

搭建个人博客因该是很多人的需要，尤其是程序员！

但是怎么搭建，搭建成什么样的每个人都会有不同的选择。

根据实现的技术大致有以下几种方案：

- 个人主页

  有很多的平台都可以注册。优缺点就大家各自体会吧。

- 静态网站生成技术

  这个就有一些技术门槛了，要有一些计算机的基础才行了。基本原理就是通过编写一些静态文件（Markdown），再通过编译器自动编译成要展示的页面。主流的有 hexo、hugo、jekyll等。

  优点：操作简单、足够定制化。

  缺点：有一定的技术门槛，是静态的。

- 内容管理系统

  这种技术要求就更高了，要涉及到服务器、数据库、域名等等。也就是建站了。如 Wordpress 等

  缺点：技术要求高、维护成本高。

  优点：完全定制化。

参考：[怎么选择和快速搭建个人博客](https://segmentfault.com/a/1190000011661576)


## Hexo 

***本机环境 window10， 64-bit，命令行操作默认是在 Git Bash 下除非特别说明。***

### 基础篇

#### 一、搭建环境

- 安装 Git

- 安装 Node.js

  - 下载地址：https://nodejs.org/en/download/
  - 默认会添加到环境变量中好安装 npm。
  - 测试：`node -v` 、 `npm -v`

- 安装 Hexo

  - `npm install -g hexo-cli`
  - 测试：`hexo -v`


#### 二、初始化

- 创建项目文件夹

  - `mkdir myblog`

- 初始化

  - `hexo init myblog`
  - 会自动在 myblog 文件夹中生成必要的文件。

- 目录介绍

  - ```
    .
    ├── _config.yml
    ├── package.json
    ├── scaffolds
    ├── source
    |   ├── _drafts
    |   └── _posts
    └── themes
    ```

    > _config.yml：配置文件
    >
    > packge.json：应用程序的信息
    >
    > scaffolds：模板文件夹
    >
    > source：资源文件夹
    >
    > themex：主题文件夹


#### 三、配置

- 网站的大部分配置都可以在 _config.yml 中设置。（主题中也有一个_ config.yml 别搞混了）

- 详细介绍请移步[官方文档](https://hexo.io/zh-cn/docs/configuration)，这里就不多介绍了。

#### 四、小试牛刀

运行以下命令：

```bash
hexo new test_my_site  # 新建一篇文章
hexo g  # 解析成 Html 文件
hexo s  # 启动服务器
```

这个时候打开浏览器访问：http://localhost:4000/  ，就能看到你博客了。

> 会生成 db.json 和 public 文件夹
>
> db.json: 缓存文件
>
> public：生成的静态文件地址（Html 等前文件，真正展示的网页文件）



#### 五、运行原理及流程

操作流程一般是：1.创建 md 文件。2.编辑内容。 3.渲染成 html 文件。4.发布

**1.**  `hexo new [layout] <title>`

> - 首先根据 layout 参数(有默认值)到 scaffolsd 文件夹下查找对应的模板文件。
>
> - layout 参数主要用来查找模板得到 Front-matter，和判断文件要保存的位置
>
> - Front-matter，如下
>
>   ```
>   ---
>   title: Hello World
>   date: 2019-04-01 16:51:03
>   ---
>   ```
>
>   >- 记录一些必要信息，标题、时间、分类、标签等，详细参数[参见文档](https://hexo.io/zh-cn/docs/front-matter)
>   >
>   >- hexo 提供了三种默认的布局：post(一篇文字)、page（一个文件夹）、draft。保存地址如下：
>   >
>   >  - post: source/_posts
>   >
>   >  - page: source
>   >
>   >  - draft: source/_drafts
>   >
>   >- 其他所有的布局都和 post 一样。
>   >
>   >- 创建一个 page 就是对应一个导航地址（如 home， tags 等）
>

> - 创建 md 文件
> - 默认以“标题 ”为文件名。(也可在配置文件中修改文件名命名规则)`

**2.** 编辑内容

- 想写什么就大胆的写吧。反正也不会有人看。

**3.** `hexo generate`

- 内容写好，接下来就是要转译成 Html 等文件了。

> - 查找文件
>   - 忽略 source 下所有以 "_"开头的文件/文件夹，除了 _port
>   - md 文件将被解析，其他文件会被直接拷贝
> - 解析 md 文件。
>   - 根据当前的主题和一定的解析规则，将 md 文件解析成 html 文件用于展示。
>   - 解析的文件和拷贝的文件被保存在 public 中。

**4.** `hexo deploy`

> - 就是将 public 文件夹下的文件推送到远端。
> - 其他文件并不会被推送。

-----

### 主题篇

#### 一、命令

```
hexo -help

hexo init [folder]  # 初始化，可指定文件夹，默认当前路径

hexo new [layout] <title>  # 新建一片文章，可指定 layout,默认使用配置文件中的默认值

hexo generate # 生成静态文件，可选参数 -d(--deploy):生成后立即部署，-w(--watch):监视文件变化

hexo publish [layout] <filename>  # 发表草稿

hexo server # 启动服务器，默认 4000端口。可选参数：-p(--port):端口号，-s(--static):只使用静态文件，-l(--log):启动日志记录；-i：指定ip地址

hexo deploy  # 部署，可选 -g(--generate):部署之前生成静态文件

hexo render <file1> [file2] ...  # 渲染文件，可选 -o(--output):输出路径

hexo migrate <type>  # 从其他博客系统迁移内容

hexo clean  # 清除缓存文件（db.json）和已生成的静态文件（public）

hexo list  

hexo version

# 以上命令都有以下可选参数
hexo --safe  # 安全模式，不使用插件和脚本
hexo --debug  # 调试模式，在终端和debug.log 中记录日志
hexo --config custom.yml  # 自定义配置文件路径
hexo --draft  # 显示草稿。（source/_drafts）
hexo --cwd /path/to  # 自定义当前工作路径
```



#### 二、部署到远端服务器

- 在基础篇中都是部署在本地的，建博客当然是要让更多人看到我们的文章了，明显在本地是不行的。

- hexo 提供了一键部署的功能 `hexo deploy`。

- 但是在运行前要在配置文件中配置好远端的信息才性。

- 以下以配置git 的远程仓库为例（如 gitbub）:

1. 安装部署git插件：`npm install hexo-deployer-git --save`
2. 配置

```
deploy:
  type: git
  repo: <repository url> #https://bitbucket.org/JohnSmith/johnsmith.bitbucket.io.git
  branch: [branch] #published
  message: [message]
```

> type:  类型，当然不止 git 一种了
>
> repo：远端仓库地址
>
> branch：分支，使用 gitbub 时系统会自动检测
>
> massage：提交信息

***也可以同时配置多个远端。***

***更直接的把 public 文件夹直接拷贝走。***



#### 三、主题

##### 1.安装

- 所有主题都存放在根目录下的 themes 下。
- 下载主题：`git clone https://github.com/iissnan/hexo-theme-next.git（主题的地址）`
- 官方主题列表：https://hexo.io/themes/
- 一个主题就 themes 下的一个文件夹，一般结构如下：
  - _config.yml：主题配置文件
  - languages：语言文件
  - layout：布局文件，存放主题的模板文件。决定网页的呈现方式。本质就是带变量的 html 文件，一般包括如下
    - index: 首页
    - post：文章详情页
    - page：导航标签页
    - archive：归档页
    - category：类别页
    - tag：标签页
    - layout：网页的整个布局，动态的由以上文件组成
  - scripts：脚本文件夹
  - source：资源文件夹

##### 2.使用

- 设置配置文件中的theme字段信息即可。


#### 四、其他

- 在实际使用中每个人都有一些不同的个性化需求。如 评论系统 等。
- 遇到想要的功能 Google 一下基本都会有的。
- 我的学习 hexo 的原则就是理解其运行原理即可，不懂的、想用的可以自行 Google。太深入的问题就没有必要去深究了。否则就违背了初衷。
- 另外写了个脚本，可以通过有道云笔记来管理和备份博客内容。见[GitHub](https://github.com/csniu/YouDaoNote-Hexo)


## 参考

- [官方文档](https://hexo.io/zh-cn/docs/)，更加详细的介绍。
- [站点布局的原理](https://www.jianshu.com/p/5a1e6d8c83af)。

