Python 可以支持非常多的数据库管理系统，如：MySQL、Oracle、SQL Server 和 PostgreSQL 等。为了实现对这些 DBMS 的统一访问，Python 需要遵守一个规范，这就是 DB API 规范。可以访问[这里](https://wiki.python.org/moin/DatabaseInterfaces)查看支持的数据库。

对于不同的DBMS有普通的模块支持，相同的DBMS也会有不同的实现方式。对MySQL主要有一下两类：
- 使用支持原生 SQL 语句的连接方式。
- 使用支持 ORM 框架的方式。

<!-- more -->

----

## 1.原生 SQL 语句

使用原生的 SQL 语句去操作数据库会更加的灵活可以，也比较轻量级。

这种方式的使用和在数据库的客户端执行 SQL 语句基本无差。

缺点就是要拼接 SQL 语句，代码会变得非常臃肿，也不利于后期的维护工作。当然安全性也是个问题。

目前主要的 MySQL 驱动有如下几个：
- MySQL-python

  又叫 MySQLdb，是最流行的一款，但是只支持 python2.x，而且安装还有很多的前置条件。

  现在基本**不推荐**使用。

- mysqlclient(import MySQLdb)

  由 MySQLdb 发展而来，完全兼容 MySQLdb 同时支持 python3.x 。

  也是 Django ORM 的依赖工具。

  **推荐**使用。

- PyMySQL

  纯 python 实现的驱动，速度上自然是逊色一些。

  优点是安装方便简单。


一般流程：

- 创建连接
- 获得游标
- 执行 SQL 语句
- 关闭游标和连接

```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-
import pymysql

# 连接数据库
db = pymysql.connect(host='127.0.0.1', port=3306, user='root', passwd='123', db='t1')

# 游标设置为字典类型
cursor = db.cursor(cursor=pymysql.cursors.DictCursor)

# 执行语句
r = cursor.execute("call p1()")  # 向SQL中传入参数时，需要使用”%s“占位。或者使用字符串拼接好SQl语句

# 获取结果
result = cursor.fetchone()

# 提交和关闭连接
db.commit()
# db.rollback()
cursor.close()
db.close()
```
 


## 2.ORM 框架

在使用原生 SQL 时，对数据库的简单操作或者是低频次的使用时，是足够的。

但是如果项目比较繁琐需要频繁的对数据库进行操作，这个时候就会遇到一些问题，比如连接池，性能等等问题。

ORM 框架是建立在数据库 API 之上，使用关系对象映射进行数据操作。

简单理解就是：

- ORM 框架本身是没有操作数据库的能力，它依赖于一些支持原生 SQL 的工具包（如：mysqlclient）。
- ORM 是对数据库的关系（表字段，表之间的对于关系等）做了抽象化的映射。
- 是注重使用层面的封装，解决了一些必要的问题。

原理就是：把对类的操作转化为原生的 SQL 进行数据库操作。

缺点：会有一些性能上的损失。

一般流程：

- 1.创建类。对数据库的映射关系。
- 2.操作类。转换成 SQL 语句，对数据库进行操作。

主要有以下ORM工具：
- [SQLAlchemy](http://www.sqlalchemy.org/)：使用最多的工具。不仅支持ORM还支持原生的SQL操作。
- Django ORM：是Django用来访问数据库的接口。
- [peewee](http://docs.peewee-orm.com/en/latest/)：轻量级的工具。
- [PonyORM](http://ponyorm.com/)
- [SQLObject](http://www.sqlobject.org/)


## SQLAlchemy

### 1.安装

`pip3 install sqlalchemy`

记得要安装底层的 MySQL API `pip3 install mysqlclient`

### 2.连接

```python
from sqlalchemy import create_engine
engine = create_engine('mysql+pymysql://root:root@localhost/test?charset=utf8',echo=True)
```

- 通过 [create_engine](https://docs.sqlalchemy.org/en/latest/core/engines.html#sqlalchemy.create_engine) 创建一个[`Engine`](https://docs.sqlalchemy.org/en/latest/core/connections.html#sqlalchemy.engine.Engine)的实例。也是 SQLAlchemy 的核心接口。
- 创建 Engine 后并不会立即建立连接，而是会在第一次执行语句时才会真正的创建连接。
- 第一个参数是连接数据库的表单数据，格式如下：
  - `dialect[+driver]://user:password@host:port/dbname[?key=value..]`
    - dialect：数据库类型，如 MySql、oracle 等。
    - driver：数据库驱动名称，如 mysqlclient(mysqldb)。MySQL默认的是MySQLdb。
    - user：用户名
    - password：用户密码
    - host：主机地址
    - port；端口号
    - dbname：需要绑定的数据库名。**连接时必须绑定具体的数据库，这就现在了使用多数据库的情况。**
    - 后面还可以指定一些其他的连接信息，如：charset=utf8
- echo=True，表示启用 logging 记录底层的 SQL 语句。
- 还有很多的其他参数可设置，可查看文档。这些参数主要用于底层的连接池创建设置。

### 3.使用

SQLALchemy 支持一下使用方式。

- 原生的 SQL 语句
- `Expression Language`
- ORM 

对数据库的操作一般也就是 增、删、改、查、事务 等。

#### 原生 SQL 方法

```python
conn = engine.connect()  # 创建一个连接
trans = connection.begin()  # 开始一个事务

try:
	rs = conn.execute('SELECT 5')  # 执行 SQL 语句，不需要游标也不需要 commit
	# sqlalchemy推荐使用text()函数封装一下sql字符串,不同数据库, 可以使用统一的sql参数传递写法. 参数须以:号引出.
	# result = engine.execute(text('select * from stock_basics where pe < :pe'), {'pe': 2})

	data = rs.fetchone()[0]  # 取回结果
	trans.commit()  # 提交，对应于事务操作
except:
	trans.rollback()  # 回滚，事务
conn.close()  # 关闭连接，也可以使用 with 语句
```

#### 创建映射关系
在使用另外两种方式操作数据之前，都需要先对数据库的表做映射。

- 创建基类
在 SQLAlchemy 中所有的数据模型都要继承于一个基类，而这个基类一般通过 [`declarative_base()`](https://docs.sqlalchemy.org/en/latest/orm/extensions/declarative/api.html#sqlalchemy.ext.declarative.declarative_base)创建。

```python
from sqlalchemy.ext.declarative import declarative_base
Base = declarative_base()
```

- 创建表映射
现在基类已经有了，接下来就是定义数据模型（对应数据库中的一张表）了，如下：

```python
from sqlalchemy import Column, Integer, String

class User(Base):
    __tablename__ = 'users'  # 数据库的表名
    
    # 字段
    id = Column(Integer, primary_key=True)  # 设置为主键
    name = Column(String)
    fullname = Column(String)
    nickname = Column(String)
    
    def __repr__(self):
        return "<User(name='%s', fullname='%s', nickname='%s')>" % (
            self.name, self.fullname, self.nickname)
```
	- 必须要定义的表名和主键，其他的和 Python 类无差别。
	- [支持的字段类型](https://docs.sqlalchemy.org/en/latest/core/type_basics.html?highlight=column%20type#generic-types)，Column 的[可选参数](https://docs.sqlalchemy.org/en/latest/core/metadata.html?highlight=column%20autoincrement#sqlalchemy.schema.Column.__init__)


- 创建数据库表
```python
  Base.metadata.create_all(engine)  # 自动创建所有不存在的表
```


#### 使用示例

```
meta = MetaData(bind=engine, reflect=True)
table = meta.tables['stock_basics']
result2 = list(engine.execute(table.select(table.c.pe < 2))) 

# and
s = users.select(and_(users.c.age < 40, users.c.name != 'Mary'))
s = users.select((users.c.age < 40) & (users.c.name != 'Mary'))

# or
s = users.select(or_(users.c.age < 40, users.c.name != 'Mary'))
s = users.select((users.c.age < 40) | (users.c.name != 'Mary'))

# not 
s = users.select(not_(users.c.name == 'Susan'))
s = users.select(~(users.c.name == 'Susan'))

# like
s = users.select(users.c.name.startswith('M'))
s = users.select(users.c.name.like('%a%'))
s = users.select(users.c.name.endswith('n'))

# in
s = users.select(users.c.age.between(30,39))
s = users.select(users.c.name.in_('Mary', 'Susan'))

# SQL函数（func）
s = users.select(func.substr(users.c.name, 2, 1) == 'a')

# 可以不在表上操作，使用全局的 select 方法
# 连表
s = select([users, emails], emails.c.user_id == users.c.user_id)
s = select([users.c.name, emails.c.address], emails.c.user_id == users.c.user_id)

# 通过外键关联
s = join(users, emails).select()

# 外连接
s = outerjoin(users, emails).select()

```


#### ORM

##### 创建会话

```python
from sqlalchemy.orm import sessionmaker
Session = sessionmaker(bind=engine)  # 根据 engine 创建会话类
session = Session()  
```

- Session 就相当于连接池。
- 对数据库的操作，首先要拿到一个连接（Session 实例），对数据库的所有操作都要在这个连接中实现。
- session 使用完要记得关闭。也可以使用 with 语句。
- session 也具有一些类方法，可以查看当前会话的状态等。
- 在 seesion 上的增删改操作并不会立马写如数据库，而是会被记录下来，在合适的时候才会写入。
	- seesion.new : 查看未提交的操作记录。
	- seesion.flush() : 写入数据库，但不提交。
	- seesion.commit(): 提交。
	- seesion.rollback(): 回滚。

##### 3.增删改查操作

查:

使用 seesion 的 query 方法。 
- 传入映射类时，返回 Query 对象，支持 get、切片、过滤、排序、分组等操作。

```
query = session.query(User)  # select * from User
query.get(2)  # 主键查询

query.filter(User.id == 2) # where
query.limit(1)  # limit

query.order_by(User.name)  # ORDER BY
query.order_by('name')
query.order_by(User.name.desc())
query.order_by('name desc')

query.offset(1)  # 从第2条开始

query.filter(User.id > 1, User.name != 'a') # and
query.filter(User.id > 1).filter(User.name != 'a')
query.filter(or_(User.id == 1, User.id == 2)) # or
query.filter(not_(User.name == None)) # not 
query.filter(User.name != None)
query.filter(User.name == None)  # is None
query.filter(User.name.is_(None))
query.filter(User.name.like('%ed%'))  # like
query.filter(User.id.in_((1, 2)) # in
query.filter(~User.id.in_((1, 2)) # not in
query.filter(User.name.in_(
        session.query(User.name).filter(User.name.like('%ed%'))
))
query.filter(User.name.match('wendy'))  # match

users = db.session.query(User).join(Role, Role.id == User.role_id)  # （内）连接
# Query.outerjoin() # 左连接

from sqlalchemy.orm import aliased
adalias1 = aliased(User)  # 表别名，同一个表多次使用时


# 查询集操作
query.all()  # 全部
query.first()  # 第一个
query.last()  # 最后一个
query.one()  # 只返回一个，查询结果不等于一个时会报错
query.one_or_none()  # 只返回一个，结果为空时返回None，多于1个时报错。
query.count()  # 计数
# session.query(func.count(User.name), User.name).group_by(User.name).all()
query.label()  # 重命名
query.label('newname').subquery()  # 重命名并作为子查询
query = session.query(User).filter(User.id.in_(subquery))  # 使用子查询

# 转 DataFrame
query.statement  # 对应的sql语句
df = pd.read_sql(query.statement, query.session.bind)
```

[参考文档](https://docs.sqlalchemy.org/en/latest/orm/tutorial.html#querying)

- 传入多个字段时，返回由通用顺序组成的tuple的list。

```
for name, fullname in session.query(User.name, User.fullname):
    print(name, fullname)
```

增改删：

```python
user = query.get(1)
# 更新
user.name = 'd'
session.flush()  # 写数据库但不提交

# 增加	
user1 = User()
session.add(user1)  # 如已存在会报错
session.add_all([...])
session.flush()  # 写数据库但不提交

# 删除
session.delete(user)
session.flush()  # 写数据库但不提交

session.commit()  #提交所有，保存到数据库
seesion.rollback()  # 回滚
seesion.close()
```

----

## 其他问题

1.在使用过程中一般有两个情况：（1）代码优先。（2）数据库优先

- 代码优先：通过编写代码动态的创建数据库。需要考虑数据的迁移问题(alembic )。

- 数据库优先：一般数据库已经创建好了，再去编写 Model 就很繁琐。这个时候可以通过一些工具直接从数据库生成 Model。（如 sqlacodegen ）

  ```python
  pip install sqlacodegen
  sqlacodegen mysql+pymysql://root:123456@127.0.0.1:3306/mystock > models.py
  # 有主键的会生成一个类（Model），没有主键会生成一个表（Table）
  ```

  

2.也不是只有只有这两种方法，条条大路通罗马。

- ORM 框架主要是解决了连接池的问题，我们也可以通过 [DBUtils](https://commons.apache.org/proper/commons-dbutils/) 解决这个问题。

## 参考
SQLAlchemy 官方文档：https://docs.sqlalchemy.org/en/latest/index.html

alembic 官方文档：https://alembic.sqlalchemy.org/en/latest/

sqlacodegen ：https://pypi.org/project/sqlacodegen/