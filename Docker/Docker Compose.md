Docker 用来用来创建隔离环境。

Compose 是用于定义和运行多容器Docker应用程序的工具。通过Compose你可以使用YAML文件来配置多个服务，并可以使用一条命令来创建和启用所有的服务。

三步走：
- 通过 Dockerfile 定义你的服务。
- 通过 docker-compose.yml 组织所有服务。
- 通过 `docker-compose up` 创建并启用所有服务。

<!-- more -->

# 安装
首先 Compose 是依赖于 `Docker Engine` 的，所有在安装 Compose之前请确保已安装 `Docker Engine`。

Mac 和 Wondows 中，在安装桌面版Docker程序是就已经集成了 Docker Engine 和 Compose 了，不需要单独安装。

Windows Server 和 Linux 中需要单独安装，可以参考[官方文档](https://docs.docker.com/compose/install/)。

# 入门操作
### 创建应用程序
如创建一个 Flask 应用程序：
```
mkdir composetest
cd composetest
```

app.py
```
import time

import redis
from flask import Flask

app = Flask(__name__)
cache = redis.Redis(host='redis', port=6379)


def get_hit_count():
    retries = 5
    while True:
        try:
            return cache.incr('hits')
        except redis.exceptions.ConnectionError as exc:
            if retries == 0:
                raise exc
            retries -= 1
            time.sleep(0.5)


@app.route('/')
def hello():
    count = get_hit_count()
    return 'Hello World! I have been seen {} times.\n'.format(count)
```
- Redis中维护一个计数器。
- 使用Redis默认暴露 6379 端口。

### 创建 Dockerfile 文件
```
FROM python:3.7-alpine
WORKDIR /code
ENV FLASK_APP app.py
ENV FLASK_RUN_HOST 0.0.0.0
RUN apk add --no-cache gcc musl-dev linux-headers
COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt
COPY . .
CMD ["flask", "run"]
```
- 创建python环境并通过requirements.txt安装包。
- 拷贝应用程序，也可以通过挂载的方式。
- 设置环境变量和启动命令。

### 创建 docker-conpose.yml 文件
```
version: '3'
services:
  web:
    build: .  # 使用当前路径下 Dockerfile 文件构建
    ports:
      - "5000:5000"
    volumes:
      - .:/code  # 挂载应用程序路径
    environment:
      FLASK_ENV: development
  redis:
    image: "redis:alpine"
```
- 创建两个服务，flask web 和 redis。
- redis服务使用 DockerHub 上的公共镜像创建，默认端口 6379。
- version：指定 docker-compose.yml 文件的写法格式

### 创建并启用服务
执行命令：
```
docker-compose up
```
- 根据当前路径下的 docker-compose.yml 文件自动创建所有镜像并启动。

#### 更新
直接修改docker-compose.yml文件并通过上述命令启动。
- 仅重新创建更改的镜像。


### 其他命令
```
docker-compose --help

docker-compose up -d  # 后台
docker-compose ps  # 查看当前运行容器
docker-compose stop  # 停止
docker-compose down  # 完全删除容器
docker-compose down  --volumes  # 同时删除数据卷

docker-compose logs  # 查看服务日志输出
docker-compose start|stop|rm  eureka  # 启动/停止/删除指定服务已存在的容器
```

### 启动和停止顺序

可以通过 [depends_on](https://docs.docker.com/compose/compose-file/#depends-on) 选项来控制服务启动和关闭的顺序。

但是Compose不会完全等到容器“就绪”才启动其他容器。

- 应用程序应该考虑到其他服务随时不可以的情况。
- 或者可以用  [wait-for](https://github.com/Eficode/wait-for)之类的工具解决。


# 参考
- [Docker 官方文档](https://docs.docker.com/compose/)