Redis 是一个开源的、基于内存的、高性能的key-value数据库。 

特点：
- 支持数据持久化。
- 支持多种数据结构。string、list、set、zset、hash等。
- 支持数据备份。
- 高性能。
- 所有操作都具有原子性。
- 支持主从同步

<!-- more -->

# 基本操作

启动服务：
```
redis-server.exe redis.conf 
```

连接服务;
```
redis-cli.exe -h 127.0.0.1 -p 6379
```

数据超出
```
set foo bar  # 添加
get foo  # 取出
exists key  # 检查key是否存在
keys *  # 查看所有key
flushall  # 清除所有key
```

# 配置
配置文件是安装路径下的 redis.conf 文件。

可以通过直接修改文件或交互式下的 CONFIG 命令修改配置。

配置说明：
- daemonize：后台守护进程运行，默认‘no’。
- pidfile：当为后台守护进程运行时会把pid写入该路径文件中。默认为：var/run/redis.pid
- port：端口。默认6379。
- bind：绑定主机地址。127.0.0.1
- timeout: 最大空闲时间。如设置为0则关闭改功能。
- loglevel：日志等级，默认verbose。
- logfile：日志输出，默认为标准输出。
- databases： 数据库数量。
- save：将数据同步到数据文件。可以多个条件配合。
    - save 900 1 ： 900秒1次更改。
    - save 300 10 ：300秒10个更改。
    - save 60 10000 ：60秒10000个更改。
- rdbcompression：保存本地时是否压缩数据。默认yes。
- dbfilename：本地数据库文件名。默认dump.rdb.
- dir: 本地数据库保存地址。
- requirepass：连接密码。默认关闭。
- maxclients：最大连接数。默认无限制。
- maxmemory：最大内存限制。
- appendonly：是否在每次更新操作后进行日志记录。如果不开启，可能会在断电时导致一段时间内的数据丢失。默认是“no”。
- appendfilename：更新日志文件名。
- appendfsync： 更新日志条件，可选no、always、everysec（每秒，默认）。

其他详细配置项及详细介绍可参考[W3Cschool Redis 教程](https://www.w3cschool.cn/redis/redis-conf.html)。

# 数据类型
## string
string是redis最基本的类型，且是二进制安全的。比如图片等。

最大可存储512MB数据。
```
SET key "value"
GET key
```

## hash
hash 是一个键值对集合，适合于存储对象。

每个hash可存储2的（32-1）次方个值（40多亿）。

```
HMSET key field value [field value]
HGETALL key
```

## list
每个list可存储2的（32-1）次方个值（40多亿）。
```
lpush key value [value]
lpush key value [value]  # 加载列表后
lrange kay start stop 
```

## set
每个set可存储2的（32-1）次方个值（40多亿）。
```
sadd key member  # 添加，成功返回1，成员已存在返回0

smembers key
```

## zset（有序集合）
每个元素都会关联一个double类型的分数。redis正是通过分数来为集合中的成员进行从小到大的排序。

zset的成员是唯一的,但分数(score)却可以重复。
```
zadd key score member [score member]

ZRANGEBYSCORE key min max
```

## 其他命令
### 发布订阅
```
# 订阅/退订给定模式频道，可使用通配符“*”
PSUBSCRIBE pattern [pattern ...]  
PUNSUBSCRIBE [pattern [pattern ...]]  

# 订阅/退订给定频道
SUBSCRIBE channel [channel ...]
UNSUBSCRIBE channel [channel ...]

# 发布消息到指定频道
PUBLISH channel message  

# 查看状态
PUBSUB <subcommand> [argument [argument ...]]
```

### 事务
```
MULTI  # 开始事务

.....   # 执行命令
# DISCARD  # 取消事务
EXEC  # 触发事务， 一并执行事务中的所有命令

Watch  # 监视一个(或多个) key ，如果在事务执行之前这个(或这些) key 被其他命令所改动，那么事务将被打断
UNWATCH  # 取消 WATCH 命令对所有 key 的监视
```

### 脚本
Redis 脚本使用 Lua 解释器来执行脚本。

执行脚本的常用命令为 EVAL。
```
EVAL script numkeys key [key ...] arg [arg ...]
```

# 参考
- [官方文档](https://redis.io/)
- [W3Cschool Redis 教程](https://www.w3cschool.cn/redis/)