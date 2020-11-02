# 消息队列

消息队列是一种应用间通信的方式。

1. 生产者只负责发送消息。

2. 消息系统负责传递和分发消息。
3. 消费者只用从消息系统中取得消息。

目前的消息队列产品有很多如：ActiveMQ、RabbitMQ 、ZeroMQ 、RocketMQ 等等。


# RabbitMQ

使用 Erlang 语言开发，基于 AMQP（Advanced Message Queue）协议实现的开源项目。支持多语言，包括但不限于 Python、Java、Ruby、PHP、C#、JavaScript、GO 等

Github 地址为： https://github.com/rabbitmq/rabbitmq-website

官网： https://www.rabbitmq.com/

<!-- more -->

## 安装

1. 安装 Erlang 语言

   下载地址： http://www.erlang.org/downloads

2. 安装 RabbitMQ

   下载地址：https://www.rabbitmq.com/download.html

## 基础概览介绍

![RabbitMQ 原理图](https://s2.ax1x.com/2020/02/06/1yslMd.png)

1. Message 消息：由消息头和消息体组成，消息体是不透明的，消息头由一系列属性组成，如下：
   1. routing-key：路由键
   2. priority：优先权
   3. delivery-mode：持久化
2. Publisher 生成者：发布消息的客户端
3. Exchange 交换器：接收消息，并负责把消息路由给服务器中的队列。
4. Binding 绑定：交换器与消息队列的对应关系。
5. Queue 消息队列：保存消息，一个消息可以分发到多个队列中。
6. Connection 网络连接：一个真实的网络连接，如 TCP 连接。
7. Channel 信道：建立在一个真实网络连接中的虚拟连接。消息都是通过信道传递，因为对应操作系统而言创建和销毁一个连接是代价昂贵的。
8. Consumer 消费者：接收消息的客户端。
9. Virtual Host 虚拟主机，表示一批交换器、消息队列和相关对象。虚拟主机是共享相同的身份认证和加密环境的独立服务器域。每个 vhost 本质上就是一个 mini 版的 RabbitMQ 服务器，拥有自己的队列、交换器、绑定和权限机制。vhost 是 AMQP 概念的基础，必须在连接时指定，RabbitMQ 默认的 vhost 是 / 。
10. Broker 服务器实体。



# Python 客户端

在 Python 中使用 RabbitMQ 依赖于 **pika** 库，需要提前安装。

定义生产者：

```python
# -*- coding: utf-8 -*-
import pika

# 连接服务器
connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
# 创建一个信道
channel = connection.channel()
# 创建一个队列，重复声明唯一性，确保队列存在（如果不存在，消息会丢失）(使用不同参数重复命名会报错)
channel.queue_declare(queue='task_queue') 
# 发送一条消息
channel.basic_publish(exchange='',          # 指定交换器
                      routing_key='task_queue',  # 指定路由键
                      body='Hello World!')  # 消息体
# 关闭连接
connection.close()
```

定义消费者：

```python
# -*- coding: utf-8 -*-
import pika
connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = connection.channel()
channel.queue_declare(queue='task_queue')

# 定义回调函数
def callback(ch, method, properties, body):
    print(" [x] Received %r" % body)

# 绑定队列的回调函数,接收到消息后调用回调函数
channel.basic_consume(queue='task_queue',            # 指定队列
                      auto_ack=True,                 # 打开自动确认
                      on_message_callback=callback)  # 指定回调函数
# 开始接受消息，会一直运行
channel.start_consuming()
```

## 问题1：消息确认

在消费者定义定义中设置 auto_act=True 表示打开自动确认（默认为 Fasle），这种情况下当 RabbitMQ 将一条消息分发出去后会将消息标记为删除，如果此时消费者被后台杀死等异常情况发送，并且任务并没有被顺利完成，则此条消息将会被丢失。

可以使用手动确认的方式，确保任务被正常消费。如下：

```python
def callback(ch, method, properties, body):
    print(" [x] Received %r" % body)
    ch.basic_ack(delivery_tag=method.delivery_tag)  # 手动确认
    
channel.basic_consume(queue='hello',
                      auto_ack=False,               # 关闭自动确认，使用手动确认
                      on_message_callback=callback)
```

## 问题2：任务分发

默认情况下，RabbitMQ 会按照顺序将消息发送给消费者，每一个消费得到的任务是相同的。即使每个消费者的工作时间不同，会出现很忙的消费者和很空闲的消费者。

可以使用 `basic.qos`方法和 `prefetch_count = 1`设置。这告诉 RabbitMQ 一次只向一个消费者发送一条消息。或者，换句话说，在处理并确认前一个消息之前，不要向工作人员发送新消息。如下：

```python
channel.basic_qos(prefetch_count=1)  # 设置
channel.basic_consume(queue='hello',
                      auto_ack=False,
                      on_message_callback=callback)
```

## 问题3：消息持久化

如果在工作中 RabbitMQ 服务器意外崩溃，则所有消息都将会丢失。

为了避免这种情况发生，可以在声明队列时（第一次）和发生消息时，指定消息持久化。如下：

```python
channel.queue_declare(queue='task_queue', durable=True)  # durable=True
channel.basic_publish(exchange='',
                      routing_key='hello',
                      body=msg,
                      properties=pika.BasicProperties(delivery_mode=2))  # 标记消息为持久性
```

将消息标记为持久性并不能完全保证消息不会丢失。虽然它告诉RabbitMQ将消息保存到磁盘，但是当RabbitMQ接受消息并且尚未保存消息时，仍然有一个短时间窗口。

## 问题4：交换器

消息发送到交换器，交换器再把消息路由到消息队列中。把消息路由到那些队列是由交换器的类型决定的。如在前面发送消息时，交换器指定的时空（exchange = ''）表示使用默认的交换器。默认交换器使用 routing-key 路由到指定队列（如果存在）。

在 RabbitMQ 中还有一些其他类型的交换器，如：direct、topic、headers 、fanout。

1. direct：完全匹配、单播的模式

   ![direct](https://s2.ax1x.com/2020/02/06/1ysGZt.png)

   消息中的路由键（routing key）如果和 Binding 中的 binding key 一致， 交换器就将消息发到对应的队列中。

   路由键与队列名完全匹配，如果一个队列绑定到交换机要求路由键为“dog”，则只转发 routing key 标记为“dog”的消息，不会转发“dog.puppy”，也不会转发“dog.guard”等等。

2. fanout：广播模式

   ![fanout](https://s2.ax1x.com/2020/02/06/1ys3qI.png)

   该模式会将接收的消息广播到所有绑定的消息队列中，而不会处理 routing-key。

3. topic：模式匹配模式

   ![topic](https://s2.ax1x.com/2020/02/06/1ysMxH.png)

   首先发送消息客户端的 routing-key 不能是随意的，必须是有点（“.”）分割的单词组成。

   再者binding-key 是routing-key 的一种匹配模式，其中通配符 “*” 匹配一个单词，“#” 匹配0个或多个单词。如上图。

   参考：https://www.rabbitmq.com/tutorials/tutorial-five-python.html

   指定自定义交换器，修改代码如下：

   ```python
   # send.py
   # -*- coding: utf-8 -*-
   import pika
   
   connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
   channel = connection.channel()
   channel.queue_declare(queue='task_queue', durable=True)  
   
   # 声明一个交换器
   channel.exchange_declare（exchange = 'logs'，exchange_type = 'fanout'）
   # 绑定一个队列到交换器
   channel.queue_bind（exchange = 'logs'，queue = 'task_queue'）
   # 发送一条消息
   channel.basic_publish(exchange='logs',    # 指定交换器
                         routing_key='hello',
                         body='Hello World!')
   
   connection.close()
   ```

   ## 问题5：RPC

   **RPC是什么？**

   RPC 是指远程过程调用协议，就是有两台服务器A、B，在A上想调用B上的一个函数。因为A、B是两台服务器，无法直接调用，这个时候就需要用到 RPC 了。（更加详细的介绍请自行Google）

   **使用 RabbitMQ 实现 RPC ：**

   1. 原生产者

      >1. 发送消息时同时发送 correlation_id 和 reply_to。
      >
      >> correlation_id 用于标识本次请求。
      >>
      >> reply_to设置回调队列。
      >
      >2. 同时也是消费者。
      >
      >>用于接收运行结果。

   2. 原消费者

      将结果信息和 correlation_id  发送到 “reply_to” 中。

   **问题：** 这种方法是阻塞的，原生产者会一直等待结果返回。而且使用 RPC 时容易混淆本地和远端的函数，对调试工作增加不必要的麻烦，一般不建议使用，如果可以应该是**异步管道**。

   详细介绍请看官网实例：https://www.rabbitmq.com/tutorials/tutorial-six-python.html

   ## 其他问题参考

   消息确认：https://www.rabbitmq.com/confirms.html

   监控：https://www.rabbitmq.com/monitoring.html

   配置： https://www.rabbitmq.com/production-checklist.html

   

   ## 参考

   消息队列之 RabbitMQ：https://www.jianshu.com/p/79ca08116d57
