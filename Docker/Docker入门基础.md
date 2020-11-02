# 前置知识

在整个程序的开发、测试、运维过程中，程序的环境依赖配置是一件很麻烦的的事情，而且还会出现一些莫名其妙的错误。为了解决这个问题一般的方法有使用虚拟机和容器技术。其中虚拟机是对整个物理机的模拟，而容器是进程级别的隔离，这两者的优缺点也就显而易见了。

Docker 就是一种流行的软件容器平台。底层基础也就是容器。

基于 Linux 内核的 cgroup，namespace，以及 AUFS 类的 Union FS 等技术，对进程进行封装隔离。这个被隔离的进程就可以抽象化为**容器**。

Docker 的架构图如下：
[![docker 架构](https://s2.ax1x.com/2020/02/06/1ysnPO.md.png)](https://imgchr.com/i/1ysnPO)

- 是一个C/S（客户端/服务端）架构。

- 客户端使用命令行或其他工具使用 [Docker API](https://docs.docker.com/develop/sdk/) 于守护进程（docker daemon）通信。
  - 如命令 `docker pull`，就是通过可执行文件 docker 建立一个客户端并将请求消息 “pull” 发送给守护进程进行通信。
  - 可以使用 go 或 python 自定义一个客户端进行通信（使用 Docker API）。
  - 默认都是将消息发送到本地的守护进程，也可通过 “-H”或修改配置文件将消息发送到远端的守护进程。
- 守护进程根据镜像（Images）生成容器。
  - 如果本地没有需要的镜像，会自动从注册仓库（Registry）下载相应的镜像文件。
  - 容器底层也有一些技术支持来满足资源控制、访问隔离、文件系统隔离的一系列的问题。

<!-- more -->

# 基本概念

**镜像**（Images）：

- 是一种特殊的文件系统。包括应用程序和所有依赖环境。
- 使用**Union FS**技术，为**分层存储架构**。
- **镜像构建时，会一层层构建，前一层是后一层的基础。每一层构建完就不会再发生改变，后一层上的任何改变只发生在自己这一层。**

**容器**（Container）：

- 镜像和容器的关系，就像是面向对象程序设计中的 `类` 和 `实例` 一样，镜像是静态的定义，容器是镜像运行时的实体。容器可以被创建、启动、停止、删除、暂停等。（容器的实质是**进程**）
- **镜像使用的是分层存储，容器也是如此。**在镜像的基础上加一层作为容器运行时的存储层。
- **容器存储层的生存周期和容器一样，容器消亡时，容器存储层也随之消亡。因此，任何保存于容器存储层的信息都会随容器删除而丢失。**
- **所有的文件写入操作，都应该使用数据卷（Volume）、或者绑定宿主目录。**

**仓库**：

- 类似于 Github 的概念。用于集中存储镜像。[Docker Hub](https://hub.docker.com/explore/) 

# 准备工作

***本文以 window 系统为例，其他系统可参考[官方文档](https://docs.docker.com/install/)下载和安装。***

Docker 分为 社区版（CE）和企业版（EE）。社区版又分为 **stable**（稳定版）, **test（测试版）**, 和 **nightly（开发版）**。

下载地址 ：https://hub.docker.com/search?q=&type=edition&offering=communit 

选择 [Docker Desktop for Windows](https://hub.docker.com/editions/community/docker-ce-desktop-windows) 版本下载，下载完成后直接双击安装即可。

- 确认是否安装成功可在命令行下执行： `docker version`

配置加速器：

settings-->Daemon-->Registry Mirrors 中 加入加速器服务地址（如：`https://dockerhub.azk8s.cn`）。

- 检查加速器是否生效，执行 docker info 可看到如下信息：

```
Registry Mirrors:
 https://dockerhub.azk8s.cn/
```

获取 `gcr.io/*` 镜像时，将 `gcr.io/<repo-name>/<image-name>:<version>` 替换为 `gcr.azk8s.cn/<repo-name>/<image-name>:<version>` 即可，如：

```shell
# docker pull gcr.io/google_containers/hyperkube-amd64:v1.9.2
docker pull gcr.azk8s.cn/google_containers/hyperkube-amd64:v1.9.2
```

**启动**：在 window 系统中可以通过双击图标启动，在 Linux 中则通过命令 `sudo systemctl start docker`启动服务端。

# 小试牛刀

执行 `docker run hello-world`,会看到如下输出：

```shell
Unable to find image 'hello-world:latest' locally
latest: Pulling from library/hello-world
1b930d010525: Pulling fs layer
1b930d010525: Verifying Checksum
1b930d010525: Download complete
1b930d010525: Pull complete
Digest: sha256:41a65640635299bab090f783209c1e3a3f11934cf7756b09cb2f1e02147c6ed8
Status: Downloaded newer image for hello-world:latest

Hello from Docker!
This message shows that your installation appears to be working correctly.

To generate this message, Docker took the following steps:
 1. The Docker client contacted the Docker daemon.
 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
    (amd64)
 3. The Docker daemon created a new container from that image which runs the
    executable that produces the output you are currently reading.
 4. The Docker daemon streamed that output to the Docker client, which sent it
    to your terminal.
```

>- 通过 docker 命令（客户端）与守护进程建立连接并发送请求消息（run hello-world）
>- docker 首先在本地查到对应镜像，如果没有会到注册仓库（Docker Hub）查找并[下载](https://hub.docker.com/_/hello-world?tab=description)。
>- 守护进程根据该镜像生成一个容器，并在容器内执行操作。
>- 守护进程将容器的输出流发送到客户端（docker 命令行）

可以看出容器的创建是根据镜像而来的，接下来就看以下对镜像（Image）的操作。

# 镜像

## 镜像管理

使用 `docker image` 命令，可通过 `docker image --help` 查看详细。如下

```
docker image ls  # 列出
docker image rm  # 删除
```

## 获取镜像

```shell
docker pull [选项] <域名/IP>：端口号/<用户名>/<仓库名>：[标签]
```

>**选项**： -a(--all-tags): 下载所有标签镜像；--disable-content-trust: 跳过图形验证（默认True）
>
>**<域名/IP>:端口号**：如未提供默认为 Docker Hub。
>
>**<用户名>/<仓库名>**：表示某个用户下的某个仓库。如果没有用户名，则默认为官方镜像（library）。
>
>**标签**：表示某个仓库的某个版本。

如：

```shell
docker pull  ubuntu:xenial
```

## 定制镜像

上面都是直接拉取别人创建好的镜像，如果别人的镜像无法满足我们的需求，这时就需要定制属于我们直接的镜像了。

在开始之前要先了解一个概念：**定制镜像一定是以一个镜像为基础，在其上进行定制。**

- 可以使用服务类镜像如：nginx、ridis、mysql 等，也可以使用语言类镜像如 node、python、golang 等，还可以使用更基础的操作系统镜像如 ubuntu、centos 等。
- 还有一种特殊的镜像 scratch，是一个空白的镜像。（一般也不会使用）

\# 使用镜像创建一个容器，该镜像必须与 Docker 宿主机系统架构一致。[参考](https://yeasy.gitbooks.io/docker_practice/image/manifest.html)

 定制镜像可以通过两种方式。

### 方法一：commit 保存

```shell
docker run --name webserver -d -p 80:80 nginx  # 启动一个容器
docker exec -it webserver bash  # 以交互式模式进入这个容器，并执行以下命令
echo '<h1>Hello, Docker!</h1>' > /usr/share/nginx/html/index.html  # 修改index 文件
exit # 退出交互式

docker diff webserver # 查看改动，可能不单是index文件，所有有变动的文件都会显示出来
```

通过上面命令我们修改了容器中的一个文件，这时我们想保存当前环境（容器介绍存储层的修改会全部丢失），可以通过 `docker commit` 命令将容器的存储层保存下来成为镜像（在原来的镜像基础上加上存储层，构成新的镜像）。commit 命令格式如下：

```shell
docker commit [选项] <容器ID或容器名> [<仓库名>[:<标签>]]
# docker commit -a 'csniu' -m '修改主页' webserver nginx:v2
```

- -a/--author:指定修改的作者
- -m/--message: 记录本次修改的内容

但是请**慎用** commit 命令创建新的镜像，因为通过 commit 命令制作的镜像别人无法得知制作过程（没有记录），而且还会导致大量无关内容被保存起来，导致镜像非常臃肿。

### 方法二：Dockerfile 定制

镜像的定制实际上就是为每一层所添加的配置和文件的操作。如果把每一层修改、安装、构建、操作的命令都写入一个脚本，用这个脚本来定制镜像就可以解决上面的问题了。这个文件就 Dockerfile 。

Dockerfile 是一个文本文件，其内包含了一条条的 **指令(Instruction)**，**每一条指令构建一层**（不是每一步操作一个指令），因此每一条指令的内容，就是描述该层应当如何构建。如下：

```shell
mkdir mynginx  # 创建一个文件夹
cd mynginx/  # 进入文件夹
touch Dockerfile  # 创建并打开一个文件，输入以下内容
```

```
FROM nginx
RUN echo '<h1>Hello, Docker!</h1>' > /usr/share/nginx/html/index.html
```

执行 `docker build -t nginx:v3` 创建新的镜像。（在 Dockerfile  文件路径下）格式说明：

```shell
docker build [选项] <上下文路径/URL/->
```

**工作原理**：整个制作的过程是在服务端进行的，上面命令只是将请求消息发送给服务端。制作镜像至少需要两个参数：

- Dockerfile 文件，记录了所有的操作。
  - 可以通过 `-f`指定（任意文件名）。否则在上下文路径中查找`Dockerfile `文件。
- 上下文目录，制作镜像过程中所需要的所有文件都在改目录下。
  - 客户端会把改路径下的**所有文件**打包发送给服务端。
  - Dockerfile 文件中 `./` 就是指上下文目录（服务端）。
  - 可以通过`.dockerignore` 文件忽略一些文件。

#### 指令说明：

- FROM 
  - 指定基础镜像，而且必须是第一条指令。
- RUN 
  - 执行命令行命令
  - shell 格式:  `RUN <命令>`
  - exec 格式: `RUN ["可执行文件", "参数1", "参数2"]`
  - **所有 exec 格式的指令都需要使用双引号，因为指令会被解析成 json 数据。**
  - **shell 格式的指令都被包装成`sh -c`形式执行，如`CMD echo $HOME`相当于`CMD [ "sh", "-c", "echo $HOME" ]`**
- COPY
  - 复制文件
  - shell 格式：`COPY [--chown=<user>:<group>] <源路径>... <目标路径>`
  - exec 格式： `COPY [--chown=<user>:<group>] ["<源路径1>",... "<目标路径>"]`
  - 默认会保留文件的元数据，如读、写、执行权限。也可以通过 `--chown=<user>:<group>`指定文件的所属用户及所属组
- ADD
  - 格式同上
  - 更高级的复制文件，源文件可以是`url`， docker 会自动下载。
  - 但是**不推荐使用**，可以通过 `RUN` 和`wget`或 `curl`结合实现。
- CMD
  - 容器的本质是进程，进程的启动需要指定程序和参数。`CMD` 指令就是用于指定默认的容器主进程的启动命令的。
  - shell 格式：`CMD <命令>`
  - exec 格式：`CMD ["可执行文件", "参数1", "参数2"...]`
  - 参数列表格式： `CMD ["参数1", "参数2"...]`
    - 在 `ENTRYPOINT` 指令后，`CMD`传递参数给`ENTRYPOINT` 指令。
  - 也可以在运行时指定新的命令来代替镜像设置中的默认指令。
  - 如果存在多条只会执行最后一条。
- ENTRYPOINT
  - 格式和作用同`CMD`
  - 使用场景：可以结合`CMD`实现更加灵活的程序指定和参数传递。
- ENV 
  - 设置环境变量，之后指令可以直接使用变量。
  - 格式1：`ENV <key> <value>`
  - 格式2：`ENV <key1>=<value1> <key2>=<value2>`
- ARG
  - 设置环境变量，但是在容器运行时不会存在这些环境变量。
  - 格式：`ARG <参数名>[=<默认值>]`
- VOLUME
  - 定义匿名卷
  - 为了保证不在容器的存储层发生写操作，可以把本地路径挂载到容器，用来保存数据如`docker run -d -v mydata:/data xxxx`， 将路径 `/data`挂载到容器并命名为 `mydata`。
  - 为了防止容器运行时未挂载卷，可设置默认卷地址。
  - shell 格式：`VOLUME <路径>`
  - exec 格式：`VOLUME ["路径1", "路径2"]`
- EXPOSE
  - 声明端口，但不会真正打开这个端口。（在随机映射端口时会随机映射一个宿主机端口到该端口）
  - 在随机映射端口时会随机映射一个宿主机端口到该端口，`docker run -P`（-p <宿主端口>:<容器端口>）
  - 格式：`EXPOSE <端口1> [<端口2>...]`
- WORKDIR
  - 指定工作路径。后层的“当前路径”就会改变为该路径。
  - 更改路径时应该使用 `WORKDIR`，而不是 `cd`切换。
  - 格式 ：`WORKDIR <路径>`
- USER
  - 同`WORKDIR`，用个切换用户。
  - 格式：`USER <用户名>[:<用户组>]`

\# 更加详细的介绍可以参考"[编写Dockerfiles的最佳实践](https://docs.docker.com/develop/develop-images/dockerfile_best-practices/)"

#### 多阶构建

通过 Dockerfile 文件构建镜像时有以下形式：

- 将所有的构建过程都写在一个 Dockerfile 文件中。

  - 但层次较多时，镜像的体积会较大，部署时间边长。

- 拆分成多个 Dockerfile 文件，再通过一些编译脚本将其整合。

  - 过程复杂。

- 多层构建（Docker v17.05 开始支持）同时满足了一个 Dockerfile 文件多阶构建的需求。格式如下：

  ```
  FROM golang:1.9-alpine as builder
  RUN apk --no-cache add git
  WORKDIR /go/src/github.com/go/helloworld/
  RUN go get -d -v github.com/go-sql-driver/mysql
  COPY app.go .
  RUN CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .
  FROM alpine:latest as prod
  RUN apk --no-cache add ca-certificates
  WORKDIR /root/
  COPY --from=0 /go/src/github.com/go/helloworld/app .
  CMD ["./app"]
  ```

  - 使用 `as`为某一节命名

  \# 还有些其他特殊的方法用来构建镜像，可自行查找。

# 容器

容器是独立运行的一个或一组应用，以及它们的运行态环境。

## 容器管理

对于容器的管理都`docker container`中，可自行查看文档了解更改。如下：

```shell
docker container ls  # 列出当前在运行的容器
docker container stop # 停止容器
docker container start  # 启动容器（通过 stop 停止的容器）
docker container rm  # 删除一个处于终止状态的容器（-f 删除运行中的容器）
docker container prune  # 删除所有处于终止状态的容器
```

## 启动容器

- 创建新的容器

```shell
docker run [选项] 镜像 [命令] [环境变量...]
```

> -t :分配一个伪终端（pseudo-tty）并绑定到容器的标准输入上。
>
> -i ：打开标准输出，一般 `-i -t`配合使用 `-it`
>
> -d：后台运行
>
> --rm: 结束后自动删除容器
>
> --name: 容器名
>
> -v: 挂载。多次使用
>
> -p: 端口映射
>
> -w: 工作路径
>
> --user: 用户（$UID:$(id -g $USER) 宿主机当前用户）


- 启动已终止的容器

```
docker container start 镜像
```

## 终止容器

- 手动终止正在运行的容器

```
docker container ls  
docker container stop 容器
```

- 自动终止
  - 当容器中指定的应用终止，容器也随之终止。

## 进入容器

```
docker attach [选项] 容器
```

- 如果从这个 stdin 中 exit，会导致容器的停止。**不推荐使用**

```
 docker exec [选项] 容器 命令 [环境变量...]
```

>-i：打开标准输出
>
>-t: 绑定伪终端
>
>**一般配合使用**

## 导入、导出

```shell
docker export 容器 > name.tar  # 导出快照到本地
cat ubuntu.tar | docker import - test/ubuntu:v1.0  # 导入快照为镜像
docker import http://example.com/exampleimage.tgz example/imagerepo  # 从 URL 导入
docker load # 加载本地镜像
```

# 仓库

仓库（`Repository`）是集中存放镜像的地方。

注册服务器（`Registry`）是管理仓库的具体服务器，服务器上可以有多个仓库，而每个仓库下面有多个镜像。Docker 官方维护的一个公共的注册服务器 [Docker Hub](https://hub.docker.com/) 是最大的注册服务器，大部分需求都可以在上面找到。

仓库可以是远端的，也可以是本地的。（感觉是不是和 Git 很像。）

## Docker Hub

要使用 Docker Hub 首先要注册一个账号，可以到官网自行注册。

```shell
docker login  # 登陆
docker logout  # 退出
docker search  # 查找镜像
docker pull  # 拉取
docker push  # 推送
```

# 数据管理

镜像是分层的，并且每一层都只是可读的。容器运行时会在镜像的基础上添加一层存储层用于读写操作。但是由于文件系统的隔离，使得：

- 容器结束数据即丢失，无法持久化。

Docker 主要有两种方式可以将本地的文件挂载到容器中，用于数据的持久化。

- 挂载数据卷（Volumes）
- 挂载主机目录 (Bind mounts)

## 数据卷

`数据卷` 是一个可供一个或多个容器使用的特殊目录，它绕过文件隔离。

Docker 主机会有一个特定的区域用来存放所有的卷，Linux 中默认是 `/var/lib/docker/volumes/`

数据卷具有以下特性：

- `数据卷` 可以在容器之间共享和重用
- 对 `数据卷` 的修改会立马生效
- 对 `数据卷` 的更新，不会影响镜像
- `数据卷` 默认会一直存在，即使容器被删除

对于数据卷的管理可通过 `docker volume`命令，如下：

```shell
docker volume ls  # 列出
docker volume create my-vol  # 创建
docker volume inspect my-vol  # 查看
docker volume rm my-vol  # 删除
docker rm -v  # 删除容器是，删除数据卷
docker volume prune  # 删除所有无主的卷
```

\# 本质上就是在主机上创建一个文件夹（特定区域）独立于容器，用于数据的持久化。

### 挂载：

可以使用 `-v` 或`--mount`命令，效果是一样的。但是 `--mount` 更强大，可读性也更强。

`--mount`由多个键值对组成，由逗号分隔，每一个由`<key>=<value>` 元祖组成。键值对没有顺序。

- `type`，可以是 `bind`，`volume`，`tmpfs`。
- `source`，主机上的文件或目录的路径。可能用 `src`，`source` 指定。
- `destination`，容器中的文件或目录的路径。可能用 `destination`，`dst`，`target` 指定。
- `readonly`，如果存在，将更改 Propagation，可以是一个 `rprivate`。
- `consistency`，如果存在，可以是 `consistent`，`delegated` 或 `cached`，只在 Mac 版有效。
- `--mount` 标志不支持 `z` 或 `Z` 修改 selinux。

```shell
--mount source=my-vol,target=/webapp   # 挂载数据卷到容器路径/webapp
--mount type=bind,source=/src/webapp,target=/opt/webapp  # 挂载主机路径/src/webqpp到容器/opt/webapp
```

# 使用网络

当容器提供的是网络服务时，要想让外部通过网络访问到容器，就需要端口映射（容器的端口和宿主机的端口）。

`-P`会随机映射一个 `49000~49900` 的端口到内部容器开放的网络端口。

`-p`指定映射端口，格式：`ip:hostPort:containerPort`

- `ip:hostPort:containerPort`: 指定地址指定端口映射。如`127.0.0.1:8080:80`
- `hostPort:containerPort`: 映射端口的所有地址。如`5000:5000`
- `ip::containerPort`: 指定地址，任意端口

# Docker三剑客

Compose：用于组织、编排多个容器。

Machine：解决运行环境问题。

Swarm：多主机多个容器的调度部署。

# 参考

[官方文档](https://docs.docker.com/)

[Docker — 从入门到实践](https://yeasy.gitbooks.io/docker_practice/#)

[可能是把Docker的概念讲的最清楚的一篇文章](https://juejin.im/post/5b260ec26fb9a00e8e4b031a#heading-0)

[Docker 命令大全](http://www.runoob.com/docker/docker-command-manual.html)

