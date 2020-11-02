# python 使用 redis
```
pip install redis
```

## 入门
```
>>> import redis
>>> r = redis.Redis(host='localhost', port=6379, db=0)
>>> r.set('foo', 'bar')
True
>>> r.get('foo')
'bar'
```
<!-- more -->

- 在3.X版本中连接只有`Redis`,`StrictRedis`只是它的一个别名，是为了和之前版本兼容。
- 如果使用的是3.X版则直接使用`Redis`即可。如果是从2.X升级过来则需要修改一下命令：
    - SETEX
    - LREM
    - TTL/PTTL

- 默认情况下python3返回字节，python2返回str。
- 可设置`decode_responses = True`来解码所有响应。

## API 规范
redis-py 默认是遵循[官方命令语法](https://redis.io/commands)的,但是有一些例外：
- SELECT：为实现，“为了保证线程安全”。
- DEL：和python关键字“del”冲突，使用“delete”替代。
- MULTI / EXEC：默认情况下，执行时管道会用MULTI和EXEC语句包装，可以通过指定transaction = False禁用管道。
- SUBSCRIBE / LISTEN
- SCAN / SSCAN / HSCAN / ZSCAN

## 连接池
默认情况下，每创建一个Redis实例就会创建一个连接池。

但是你也可以将已创建好的连接池通过`connection_pool`参数传递给Redis类用于实例化操作。
```
pool = redis.ConnectionPool(host='localhost', port=6379, db=0)
r = redis.Redis(connection_pool=pool)
```

## 连接
redis-py 提供了两种连接方式：
- `TCP socket`的常规连接。
- `Unix domain socket`连接。
```
r = redis.Redis(unix_socket_path='/tmp/redis.sock')
```

也可以创建直接的“Connection”类.
```
pool = redis.ConnectionPool(connection_class=YourConnectionClass, your_arg='...', ...)
```
### 连接断开
连接空闲达到预设的阈值时将会被断开，这是再继续操作将会受到`ConnectionError`错误。客户端可自行判断处理。

但是在生产环境中实时检查会比较繁琐，可通过如下方法解决：
- 在实例化`Redis`或`ConnectionPool`时设置`health_check_interval`值来对执行命令前的健康检查。
    - 该值为整数。
    - 为0时为禁用健康检查。
    - 如为30则表示：当连接空闲超过30秒时，则在执行命令前进行连接的健康检查。
- 使`health_check_interval`小于系统空闲阈值。


## 解析器
redis-py 中有两个解析器类：PythonParser 和 HiredisParser。首先会尝试使用 HiredisParser。

HiredisParser 是使用C语言编写，性能更高。可通过pip安装。
```
pip install hiredis
```

## 响应回调
客户端实例通过一组回调函数将Redis响应转换成Python类型。

自定义实例的回调函数，可在实例化是通过`set_response_callback`参数设置。

修改全局的回调函数，可通过继承`Redis`类并修改`RESPONSE_CALLBACKS`属性。

## 线程安全
Redis客户端实例可以在线程之间安全地共享。

只有在执行命令是才会从连接池中取出连接并执行，当命令执行完毕后连接会被放回连接池中。命令的执行过程不会修改连接实例的状态。

因为`SELECT`命令会修改当前使用的数据库，从而导致连接实例的状态被修改。所有在 redis-py 中没有实现`SELECT`命令。如果在同一应用程序中需要使用不同Redis数据库，则需要创建不同的连接实例（连接池实例）。

## 管道
在redis中使用管道来缓存请求命名，从而减少客户端和服务端的TCP数据包的数量。（即多次请求一次返回）
```
r = redis.Redis(...)  # 创建客户端实例
r.set('bing', 'baz')  # 添加数据

pipe = r.pipeline()  # 创建管道实例
pipe.set('foo', 'bar')  # 向管道中添加命令
pipe.get('bing')

pipe.execute()  # 一次将所有请求发生到服务端并取回结果
# [True, 'baz']
```
- 默认管道操作是具有原子性的。（transaction=True）

## 事务
```
with r.pipeline() as pipe:
    while True:
        try:
            # 开始事务前监视该值，如果被改变则终止事务并抛出 WatchError 错误
            pipe.watch('OUR-SEQUENCE-KEY')
            current_value = pipe.get('OUR-SEQUENCE-KEY')
            next_value = int(current_value) + 1
            # 开始事务
            pipe.multi()
            # 需要执行的命令
            pipe.set('OUR-SEQUENCE-KEY', next_value)
            # 触发执行所有命令
            pipe.execute()
            break
        except WatchError:
            continue
        # finally：
        #     pipe.reset()
```
- 管道使用完毕需要放回到连接池中，需要调用reset()方法或使用上下文管理器。

或者使用客户端对象的`transaction()`方法快捷的执行事务操作。其至少需要一个可调用对象，并且可选择传入需要监视的值。定义如下：
```
def transaction(self, func, *watches, **kwargs):
    shard_hint = kwargs.pop('shard_hint', None)
    value_from_callable = kwargs.pop('value_from_callable', False)
    watch_delay = kwargs.pop('watch_delay', None)
    with self.pipeline(True, shard_hint) as pipe:
        while True:
            try:
                if watches:
                    pipe.watch(*watches)
                func_value = func(pipe)
                exec_value = pipe.execute()
                return func_value if value_from_callable else exec_value
            except WatchError:
                if watch_delay is not None and watch_delay > 0:
                    time.sleep(watch_delay)
                continue
```
上了事务可改写为如下方式：
```
def client_side_incr(pipe):
    current_value = pipe.get（' OUR-SEQUENCE-KEY '）
    next_value =  int(current_value) + 1
    pipe.multi()
    pipe.set('OUR-SEQUENCE-KEY'，next_value）

r.transaction(client_side_incr，'OUR-SEQUENCE-KEY')
```

## 发布/订阅
redis-py提供一个PubSub对象，用来订阅和侦听消息。

```
r = redis.Redis(..)
p = r.pubsub()  # 创建对象

p.subscribe('my-first-channel', 'my-second-channel')  # 订阅频道
p.psubscribe('my-*')  # 模式订阅

p.unsubscribe()  # 取消订阅，如无参数传入则取消所有订阅
p.punsubscribe('my-*')  # 模式取消

p.get_message()  # 读取消息

p.close()
```
- 读取的消息是一个dict，并且始终具有以下键：
    - type：消息类型，值为'subscribe'，'unsubscribe'，'psubscribe'，'punsubscribe'，'message'，'pmessage'之一。
    - pattern：匹配模式，除“pmessage”类型外，均为None。
    - channel：消息发布的频道。
    - data：消息数据。如果是取消订阅，则为取消订阅频道的数量。

#### 回调函数
还可以注册回调函数，来自定义消息处理。该回调函数需要接收单个参数，即消息。
```
def my_handler(message)
    print('My Handler', message['data'])
    
p.subscribe(**{'my_channel': my_handler})  # 订阅频道并注册其回调函数
p.get_message()
```

#### 过滤消息
```
p = r.pubsub(ignore_subscribe_messages = True)
```
- 将过滤所有订阅与取消订阅的消息，即type为‘subscribe’、‘unsubscribe’、‘psubscribe’、‘punsubscribe’的消息。

#### 读取消息的三种策略
- pubsub.get_message(): 有消息则返回消息，无则返回None。
- pubsub.listen(): 会阻塞当前进程直到有消息为止。
- pubsub.run_in_thread(): 会创建一个新的线程，并在新线程中循环调用 get_message。
    - 订阅的频道必须注册回调函数。
    - 可接受sleep_time参数，在每次循环中调用time.sleep()。
    - thread.stop()：结束新线程。


# 参考
- [GitHub redis-py](https://github.com/andymccurdy/redis-py)