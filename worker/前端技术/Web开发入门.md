# WEB开发概览

## 基础概念
互联网：
- 凡是能彼此之间能通信的设备组成的网络就叫互联网，无所谓多少设备和使用什么技术通信。
- 一般可分为广域网、域域网、局域网。
- 国际标准的互联网写法是internet（i小写）。

因特网：
- 由上千万台设备组成，并且使用TCP/IP协议通信的网络。但是使用TCP/IP协议的网络并不一定是互联网。
- 因特网提供的服务一般包括：www（World Wide Web）服务(万维网)、电子邮件服务、远程登陆服务、文件传输服务、网络电话等等。
- 国际标准的因特网写法是Internet（i大写）。
- 日常所说的互联网也泛指因特网。

万维网：
- 只是因特网提供的一种服务，是由许多互相链接的**超文本**组成的系统。只要是在应用层上使用的是HTTP协议，就称为万维网（World Wide Web）亦作 WWW、Web。

## WEB开发
WEB 服务是一种C/S（客户端/服务端）架构，客户端一般指代我们的浏览器用来发起请求，而服务端就是提供服务的一方，响应请求。

整个数据交互过程可概述为如下：
1. 首先服务端要开启服务（程序）并监听指定的网络端口，服务开启后可同时接收多个客户端的请求。
2. 客户端通过URL（统一资源标志符）定位到我们的服务地址并发起请求。
3. 服务端再将数据（html、css、图片等等）发送到客户端。
4. 服务端再将数据渲染成特定的样式并呈现出来，这就是我们所看到的网页了。

上述过程体现出了WEB的三大关键技术URL、HTML和HTTP。
- URL
  统一资源定位符（Uniform Resource Locator）用来定位 WEB 上的一个资源，如一个HTML页面或一个CSS文档再或者是一副图像等。**理论**上每个有效 URL 都定位唯一一个资源。一般有协议、域名、端口、服务器资源地址、查询参数、锚点等构成，其中有些是必须的有些是可选的，详情见[MDN的文档](https://developer.mozilla.org/zh-CN/docs/Learn/Common_questions/What_is_a_URL)。

- HTML
  

三项关键技术：

一个全球网络资源唯一认证的系统，统一资源标志符（URI）；
超文本标记语言（HTML）；
超文本传输协议（HTTP）。[12]



# 参考
- [百度百科-互联网](https://baike.baidu.com/item/%E4%BA%92%E8%81%94%E7%BD%91/199186)
- [你要问我应用层？我就和你扯扯扯](https://www.cnblogs.com/cxuanBlog/p/12262563.html)
- [计算机网络漫谈：OSI七层模型与TCP/IP四层（参考）模型](https://www.jianshu.com/p/c793a279f698)

# WEB 的技术发展

