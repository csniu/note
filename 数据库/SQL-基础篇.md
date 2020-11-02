SQL 是指结构化查询语言，全称”Structured Query Language“。是访问和处理 **关系数据库（RDBMS）** 的计算机标准语言。由ANSI组织和定义，主要标准有SQL92和SQL99。SQL语言按照功能可分为以下4类：
- 数据定义语言（DDL：Data Definition Language）：CREATE、ALTER、DROP。
- 数据操作语言（DML：Data Manipulation Language）：INSERT、UPDATE、DELETE。
- 数据控制语言（DCL：Data Control Language）：GRANT、REVOKE、COMMIT、ROLLBACK。
- 数据查询语言（DQL：Data Query Language）：SELECT。


语法特点：
- 总是以关键字开始并以";"结尾。
- 不区分大小写。但是一般约定：
    - SQL 保留字、函数名、绑定变量等都大写。
    - 表名、表别名、字段名、字段别名等都小写。

***注意：SQL本身是标准的语言，但不同DBMS对其支持不同，而且多数对其还进行的扩展。在实际使用中还需注意。***


# 前置知识

DB（DataBase）：数据库，是存储数据的集合。

DBMS（DataBase Management System）：数据库管理系统。DBMS = 多个数据库 + 管理软件。
- 我日常说的Oracle、MySQL数据库，严格来说应该是DBMS。

RDBMS（Relational Database Management System）：关系数据库管理系统。SQL就是其查询语言。

相比于SQL，NoSQL泛指非关系型数据库，包括
- 键值型数据库：知道key时查询速度快，否则要遍历所有数据。一般用作缓存和消息中间件，代表是Redis。
- 文档型数据库：MongoDB。
- 搜索引擎：全文搜索效率高。Elasticsearch、Splunk、Solr。
- 列存储：降低系统的 I/O，适合于分布式文件系统，不足在于功能相对有限。
- 图形数据库：能高效地解决复杂的关系问题。
- 等等。

NoSQL 在不同时期有着不同定义，现在更多的定义为“Not only SQL”。所有SQL与NoSQL并不是完全对立的两个阵营，应该是相辅相成的两种技术。


# 关系数据库
RDBMS是关系数据库管理系统(Relational Database Management System)的缩写，是一种基于E.F.Codd提出的关系模型的数据库管理系统，也是 SQL 的基础。
- 数据存储在称为”表“的数据库对象中。表由”列“和”行“组成。
- ”列“名称为”字段“，每一列存储的数据类型必须相同。
- ”行“也称为“数据行”，是存储一条单独数据的地方。

## 主流关系数据库
商用数据库：Oracle、SQL Server、DB2。

开源数据库：MySQL（MariaDB）、PostgreSQL。

桌面数据库：Access。

嵌入式数据库：SQLite。


## 数据类型
名称|类型|说明
---|---|---
INT|整型|4字节整数类型，范围约+/-21亿
BIGINT|长整型|8字节整数类型，范围约+/-922亿亿
REAL（FLOAT(24)）|浮点型|4字节浮点数，范围约+/-1038
DOUBLE|浮点型|8字节浮点数，范围约+/-10308
DECIMAL(M,N)|高精度小数|由用户指定精度的小数，例如，DECIMAL(20,10)表示一共20位，其中小数10位，通常用于财务计算
CHAR(N)|定长字符串|存储指定长度的字符串，例如，CHAR(100)总是存储100个字符的字符串
VARCHAR(N)|变长字符串|存储可变长度的字符串，例如，VARCHAR(100)可以存储0~100个字符的字符串
BOOLEAN|布尔类型|存储True或者False
DATE|日期类型|存储日期，例如，2018-06-22
TIME|时间类型|存储时间，例如，12:20:59
DATETIME|日期和时间类型|存储日期+时间，例如，2018-06-22 12:20:59

## SQL约束
是限制“表”上数据的强制规则，可以在创建表时规定（CREATE TABLE）也可在创建表之后规定（ALTER TABLE）。常见约束有：
- NOT NULL：非空约束，一般应该避免为NULL，这样可以简化查询条件和加快查询速度。结合DEFAULT使用。
- DEFAULT：默认值
- UNIQUE：唯一值
- 主键：唯一标识，不能重复，不能为空。设置原则是：不使用任何业务相关的字段作为主键。一般使用“自增整数类型”或“全局唯一GUID类型”。
- 联合主键：多个字段唯一标识数据，除非必要否则应避免使用，因为它会使关系表的复杂度上升。多个字段中不能有为NULL的值，否则唯一约束失效（即使其他值相同也不会引起唯一性冲突）。
- 外键：唯一标识其他表数据（注意并不是通过列名实现的）。因为外键约束会降低数据库性能，所有很多公司并不设置外键，而是依靠应用程序自身逻辑。（数据量大或追求性能）
- CHECK：列中所有数据满足某一条件
- 索引：快速检索，效率取决于索引列的值是否散列，即如果越互不相关则效率越高。提高了查询效率但是降低了插入、更新、删除的效率。

### 外键
外键的作用是将数据的一致性、完整性交由数据库自身保证，最大限度的保证数据的一致性、完整性。如一张表的非主键字段指向另一张表的主键字段，则称该字段为该表的一个外键，该表为子表。另一张表为父表。


外键的默认作用有两点：
- 对子表(外键所在的表)的作用：子表在进行写操作的时候，如果外键字段在父表中找不到对应的匹配，操作就会失败。
- 对父表的作用：对父表的主键字段进行删和改时，如果对应的主键在子表中被引用，操作就会失败。

三种关系：
- 一对一：一个表的记录对应到另一个表的唯一一个记录。主要作用是拆分表数据，提高数据库性能。
- 一对多：父表中的一个对应从表中的多个。

- 多对多：实际上是通过两个一对多关系实现的，即通过一个中间表，关联两个一对多关系。


三种约束模式：
- district：严格模式(默认), 父表不能删除或更新一个被子表引用的记录。(同 no action)
- cascade：级联模式, 父表操作后，子表关联的数据也跟着一起操作。
- set null：置空模式,前提外键字段允许为NLL, 父表操作后，子表对应的字段被置空。

“on delete”、 “on update” 、“on insert” 说的都是在父表中执行某种操作时，子表的约束模式。
- on delete
    - district: 父表删除时，如果子表存在则无法删除。
    - cascade: 父表删除时，子表也一起删除。
    - set null: 父表删除时，指表被设置为 NULL。
- on updata
    - district:父表（主键）更新时，如子表中存在则无法更新。
    - cascade:父表（主键）更新时，子表会被一起更新。
    - set null:父表（主键）更新时，子表被设置为NULL。
- on insert:
- 父表可正常增加，无论何种模式。
- 子表新增时外键值必须在主表中存在。


# SQL语法
## SQL 的执行方式

### Oracle 中SQL的执行过程
1. 语法检查：检查拼写是否正确。
2. 语义检查：对象是否存在。
3. 权限检查：是否具有操作权限。
4. 共享池检查
    - 共享池是用来缓存SQL语句和该语句具体执行计划的。
    - 有缓存，则直接调取执行计划。（软解析）
    - 无缓存，则需要创建解析树进行解析，并生成执行计划，再进入“优化器”进行优化。（硬解析）
5. “执行器”执行”执行计划“。

***避免硬解析可以提高执行效率，比如使用绑定变量。***

### MySQL 中SQL的执行过程
首先 MySQL 是C/S架构，其基本架构是：客户端 --> 连接层 --> SQL层 --> 存储引擎层。

其最主要的特点是存储引擎采用插件式。常见引擎有如下：
- InnoDB：是5.5版以后的默认引擎，特点是支持事务、行级锁定、外键约束。
- MyISAM：5.5版之前默认引擎，不支持事务和外键。但是速度快和占用资源少。
- Memory：使用内存作存储介质，响应速度快。但是如果服务进程崩溃，会导致数据丢失。
- NDB：也叫 NDB Cluster 存储引擎。主要用于分布式集群环境。
- Archive：具有压缩机制。

***MySQL中每张表可以选择使用不同的存储引擎。***

其SQL执行过程如下：
- 查询缓存：8.0版本以后抛弃该环节。
- 解析器：语法、语言解析。
- 优化器：确定SQL执行路径。
- 执行器：检查权限并执行SQL。

分析SQL语句的执行效率（时间）：
- 开启 `PROFILING`
    - `SELECT @@PROFILING;`：查看当前状态，0为关闭，1为开启。
    - `SET PROFILING=1;`：设置状态。
- 执行SQL
- 查看执行过程中所需资源
    - `SHOW PROFILES;`: 当前会话所产生的所有PROFILE。
    - `SHOW PROFILE FOR QUERY <query_id>`: 查看指定查询的PROFILE。
    - `SHOW PROFILE;`: 查看上一次查询的PROFILE。

## 变量（MySQL为例）

### 全局变量
- 来源于软件自编译、配置文件中、以及启动参数中指定的变量。
- 只能修改（部分，重启失效）不能定义。但是需要`SUPER`权限。
- 作用于全局。


查询：
```
SHOW GLOBAL VARIABLES;
SHOW GLOBAL VARIABLES LIKE '%sql%';

SELECT @@GLOBAL.<var>;
SELECT @@<var>;  # 该变量不能存在于会话变量
```

修改：
```
SET GLOBAL <var>='';
SET @@GLOBAL.<var>='';
```

***详细的变量介绍可查看[MySQL官方文档](https://dev.mysql.com/doc/refman/8.0/en/server-system-variables.html)。***

### 会话变量
- 来自于全局变量的复制(部分)
- 只能修改不能定义。不需要`SUPER`权限。
- 作用于当前连接，连接断开变量消失。


查询变量：
```
SHOW SESSION VARIABLES;
SHOW SESSION VARIABLES LIKE '%sql%';

SELECT @@SESSION.<var>;
SELECT @@LOCAL.<var>;
SELECT @@<var>;
```


赋值：
```
SET SESSION <var> = '';
SET LOCAL <var> = '';
SET @@SESSION.<var> = '';
SET @@LOCAL.<var> = '';

SET @@<var> = '';
SET <var> = '';
```

***使用”@@“访问变量时，优先从会话变量中查找，会话变量中没有的才会到全局变量中查找。***


### 用户变量
- 是用户自定义的变量。
- 无需声明，直接赋值即可。
- 作用于当前连接，连接断开变量消失。


查询：
```
SELECT @<var>;
```

设置：
```
SET @<var>=1;
SET @<var>:=0;  # ":="
SELECT @<var>:=0; # 只能使用”:=“

SELECT count(*) INTO @<var> FROM <table_name> WHERE ...;  # SELECT INTO
```
***[官方文档说明在这里](https://dev.mysql.com/doc/refman/8.0/en/user-variables.html).***

### 局部变量
- 是用户自定义的变量。
- 定义于函数或存储过程中，需要先声明再赋值。
- 只作用于函数或存储过程中，执行完毕即消失。


如：
```
CREATE PROCEDURE `p_name`()
BEGIN
    DECLARE i INT;  -- 声明变量
    SET i = 1;  -- 赋值变量
    
    DECLARE m INT;
    SELECT count(*) INTO @m FROM <table_name>;  -- 使用”SELECT INTO“赋值
    ...    
END
```


## 数据定义语言（DDL）
### 数据库
```
CREATE DATABASE <database_name>；
DROP DATABASE <database_name>；
```

### 数据表
```
CREATE TABLE <table_name> (<field1> <data_type1> 约束, <field2> <data_type2> 约束,...);
DROP TABLE <table_name>;
TRUNCATE TABLE <table_name>;  # 清空表中数据
ALTER TABLE <table_name> ADD <column_name> <data_type>;  # 添加新字段
ALTER TABLE <table_name> DROP COLUMN <column_name>;  # 删除字段
ALTER TABLE <table_name> ALTER COLUMN <column_name> <data_type>;  # 修改数据类型
ALTER TABLE <table_name> ALTER COLUMN <column_name> TO <new_column_name>;  # 修改数据类型
```

### 视图
```
CREATE VIEW <view_name> AS SELECT <field1>, <field2> FROM <table_name> [WHERE ...];
ALTER VIEW <view_name> AS SELECT <field1>, <field2> FROM <table_name> [WHERE ...]; # 同创建语法
DROP VIEW view_name;
```
- 视图是虚拟表，并不真实存在，但是可以像真实表一样使用。实际上就是在`SELECT`语句的基础上的封装。
- 视图可以嵌套。
- SQLite 不支持视图的修改，只支持创建和删除。
- 一般DBMS不支持创建索引，但SQL Server支持。

作用：
- 屏蔽了底层数据结构，提高了数据的安全性。
- 简化SQL查询语句，提高了代码的复用率，同时也提高了执行效率（视图的SQL语句只会在创建时编译一次）。

***和临时表不同，视图不存储数据，而临时表是真实存在的表（但是只对应当前会话可见，会话结束后自动释放）。***

## 数据操作语言（DML）
### 创建数据
```
INSERT INTO <table_name> (<field1>, <field2>) VALUES (<value1>, <value2>);
```
- 字段顺序要和值顺序一一对应。
- 多条数据可用逗号隔开。

### 修改数据
```
UPDATE <table_name> SET <field1>=<value1>, <field2>=<value2> [WHERE ...];
```
- 会修改结果集中的 **所有** 数据。


### 删除数据
```
DELETE FROM <table_name> [WHERE ...];
```
- 会删除结果集中的 **所有** 数据。


## 数据查询语言（DQL）
### 简单查询
```
SELECT * FROM <table_name>;  # “*”表示所有字段
SELECT <field1>, <field2> FROM <table_name>;  # 查询指定字段
SELECT <filed> AS f_alias FROM <table_name> AS t_alias;  # 重命名
SELECT <field1>, <field2>, 'constant' AS <field_name> FROM <table_name>;  # 设置常数列，常量需要使用引号。
```
- SELECT 后不一定都有 FROM，如`SELECT 100+200;`可以用来计算，但这并不是SQL的强项。但是可以用其判断当前数据库的连接是否有效
（`SELECT 1;`）。

### 去重
```
SELECT DISTINCT <field>, <field>  FROM <table_name>
```

### 条件查询
```
SELECT * FROM <table_name> WHERE <field1> = 'value1' AND <field2> = 'value2';  
```
- 支持表达式：>,<,=,>=,<=,<>,IN, LIKE(模式匹配),BETWEEN(某个范围内)。
- 通配符只能和“LIKE”运算符一起使用，统配符有：
    - “%”：匹配任意个任意字符。Access中使用“*”。
    - “_”：匹配一个字符，Access中使用“？”，DB2中不支持。
    - `[charlist]`: 匹配字符列表中的单一字符。
    - `[!charlist]`或`[^charlist]`:不匹配字符列表中的单一字符。
- 多条件可使用 AND、OR 和 NOT，并可结合圆括号（“()”）组合使用。
- 特殊值“NULL”可用“IS NULL”和“IS NOT NULL”判断。

### 排序
```
SELECT * FROM <table_name> ORDER BY <field> ASC;
```
- 多字段排序用逗号隔开。
- 支持升序（ASC）和降序（DESC）。默认为ASC，可省略。

### 分页
```
SELECT * FROM <table_name> LIMIT <M> OFFSET <N>;
```
- LIMIT: 限制每一页的数量。
- OFFSET：偏移量，从第几条数据开始。可以省略，相当于“OFFSET 0”
- OFFSET 超过结果集最大数量时，返回为空。
- OFFSET 越大效率越低。

***MySQL、PostgreSQL、SQLite 中使用“LIMIT”，其他数据库略有不同，如SQL Server 和 Access 中使用“TOP”关键字，Oracle 使用“ROWNUM”关键字。***

***约束结果数据可以减少网络传输量，同时也可以提高查询效率。如约束只返回一条结果，则不需要扫描所有数据，而是查到一条即可返回结果。***

### 聚类查询
```
SELECT COUNT(*) FROM <table_name>;  # 计数结果数量
```
除了`COUNT`函数外还有以下函数可使用：
- SUM：求和，必须是数值型。
- AVG：平均值，必须是数值型。
- MAX：最大值。
- MIN：最小值。

***注意：如果聚合查询的WHERE条件没有匹配到任何行，COUNT()会返回0，而SUM()、AVG()、MAX()和MIN()会返回NULL。***

```
SELECT COUNT(*), <filed1> FROM <table_name> GROUP BY <field1>;
```
- 结果集中只能有分类字段（field1）和聚类结果（COUNT）

```
SECLECT SUM(<field>) AS sm FROM <table_name> HAVING sm > 5; 
```
- 过滤分组。
- HAVING 和 WHERE 的作用是一样的。不同的是WHERE作用于数据行，HAVING则作用于分组。


### 连表查询
SQL92和SQL99标准中连表查询语法略有不同，一般SQL92更简单但是可读性查，SQL99的可读性更强。现DBMS对SQL99标准支持度更好些，但是也略有不同。多表连接时更推荐SQL99的写法。

#### 笛卡尔查询
查询结果是两张表的“乘积”，即用表1中的每一条数据都和表2的所有数据拼接一次。

SQL92
```
SELECT * FROM <table_a>, <table_b>;  
```
SQL99
```
SELECT * FROM <table_a> CROSS JOIN <table_b>;
```

#### 等值连接和非等值连接
使用“=”连接的叫等值连接，其他都叫非等值连接。
SQL92
```
SELECT * FROM <table_a>, <table_b> WHERE <table_a.field> = <table_b.field>;
```

SQL99
```
SELECT * FROM <table_a>, <table_b> WHERE <table_a> NATURAL JOIN <table_b>;  # 自动查询相同字段并进行等值连接
SELECT * FROM <table_a>, <table_b> WHERE <table_a> JOIN <table_b> USING (<field>);  # USING连接，需要指定同名字段
SELECT * FROM <table_a> [INNER] JOIN <table_b> ON <table_a.field> = <table_b.field>;  # ON连接
```

#### 外连接
SQL92中用“+”表示从表，且没有全连接。
```
SELECT * FROM <table_name1>  RIGHT [OUTER] JOIN <table_name2> ON <table_name1.field> = <table_name2.field>;
```
连接方式有：
- RIGHT [OUTER] JOIN：右外连接，右表所有数据。
- LEFT [OUTER] JOIN：左外连接，左表所有数据。
- FULL [OUTER] JOIN：全连接，两张表所有数据。

#### 自连接
查询条件使用当前表字段
SQL92
```
SELECT * FROM <table_a> AS a, <table_a> AS b WHERE <a.field> > <b.field>;
```
SQL99
```
SELECT * FROM <table_a> AS a JOIN <table_a> AS b ON <a.field> > <b.field>;
```
#### 注意
##### 不同DBMS中的差异
 - 不是所有的 DBMS 都支持全外连接，如：MySQL、Access、SQLite。
 - Oracle 没有表别名 AS，而是直接写在后面。
 - SQLite 的外连接只有左连接

##### 性能
- 控制连接表数量。
- 连接时使用WHERE语句。
- 使用自连接而不是子查询。 



### SELECT 的执行顺序
首先关键字的顺序不能颠倒：
 ```
SELECT ... FROM ... WHERE ... GROUP BY ... HAVING ... ORDER BY ...
 ```
 
执行顺序：
 ```
FROM > WHERE > GROUP BY > HAVING > SELECT的字段 > DISTINCT > ORDER BY > LIMIT
 ```
 
如：
 ```
SELECT DISTINCT player_id, player_name, count(*) as num #顺序5
FROM player JOIN team ON player.team_id = team.team_id #顺序1
WHERE height > 1.80 #顺序2
GROUP BY player.team_id #顺序3
HAVING num > 2 #顺序4
ORDER BY num DESC #顺序6
LIMIT 2 #顺序7
 ```
 每一个步骤都会创建一个“虚表”，然后用这个虚表的结果作为下一步的输入。

### 子查询

```
# 非关联子查询，子查询只执行一次
SELECT <field>,... FROM <table_a> WHERE <field> = (SELECT MAX(<field>) FROM <table_b>)  

# 关联子查询，子查询需要执行多次
SELECT <field>,... FROM <table_a> AS a WHERE <field> > (SELECT AVG(<field>) FROM <filed_b> AS b WHERE a.<field1> = b.<field1>)
```

#### 集合比较子查询
```
SELECT * FROM <table_a> WHERE <field> IN (SELECT <field> FROM <table_b> [WHERE ...])

SELECT * FROM <table_a> WHERE <field> > ANY (SELECT <field> FROM <table_b> [WHERE ...])
SELECT * FROM <table_a> WHERE <field> > SOME (SELECT <field> FROM <table_b> [WHERE ...])  # 同ANY

SELECT * FROM <table_a> WHERE <field> > ALL (SELECT <field> FROM <table_b> [WHERE ...]
```
- ANY 和 ALL 必须和比较符一起使用。


##### IN & EXIST
下面两个查询语句的结果是一样的：
```
SELECT * FROM A WHERE cc IN (SELECT cc FROM B)
SELECT * FROM A WHERE EXISTS (SELECT cc FROM B WHERE B.cc=A.cc)
```
- 结果是一样的，区别是执行的效率不同。
- 如果A表小于B表，则用EXISTS效率会高些。

#### 子查询作为计算字段
```
SELECT team_name, (SELECT count(*) FROM player WHERE player.team_id = team.team_id) AS player_num FROM team
```


### 合并结果集
```
SELECT * FROM <table_name1>
UNION 
SELECT * FROM <table_name2>
```
- 合并多个查询结果集，每个结果集必须拥有相同数量的列，且对应列要具有相似的数据类型。
- 合并后的结果集的列名，等于第一个结果集的列名。
- `UNION` 默认会去重，不去重可以使用`UNION ALL`.


## 函数
SQL中内置函数一般可分为：
1. 算术函数
    - ABS(): 取绝对值。
    - MOD(): 取余数。
    - ROUND(): 四舍五入。
2. 字符串函数
    - CONCAT(): 拼接字符串。
    - LENGTH(): 计算字符串长度，一个汉字算三个字符。
    - CHAR_LENGTH(): 计算字符串长度，一个汉字算一个字符。
    - LOWER(): 转小写。
    - UPPER()：转大写。
    - REPLACE(): 替换函数。
    - SUBSTRING(): 截取字符串。
3. 日期函数
    - CURRENT_DATE():系统当前日期。
    - CIRRENT_TIME():系统当前时间，没有具体日期。
    - CURRENT_TIMESTAMP():系统当前时间，日期+时间。
    - EXTRACT():提取具体的年月日。
    - DATE():返回时间日期部分。
    - YEAR():返回时间年份部分。
    - MONTH():返回时间月份部分。
    - DAY():返回时间天数部分。
    - HOUR():返回时间小时部分。
    - MINUTE():返回时间分钟部分。
    - SECOND():返回时间秒部分。
4. 转换函数
    - CAST(): 转换数据类型。如:CAST(123.123 AS DECIMAL(8,2))
    - COALESCE(): 返回第一个非NILL值。如：COALESCE(null,1,2)，运行结果为 1。
5. 聚合函数
    - COUNT():总行数。
    - MAX():最大值。
    - MIN():最小值。
    - SUM():求和。
    - AVG():平均值。

***函数部分在不同DBMS中支持不同，而且各有扩展。所有可以移植性比较差，使用过程中需要格外注意。***

## 事务
事务就将任意条SQL语句作为一个整体进行操作，要么成功，要么失败并还原成原状态。具有以下特点：
- 原子性（Atomic）：将所有SQL作为原子工作单元执行，要么全部执行，要么全部不执行。
- 一致性（Consistent）：就是数据库在进行事务操作后，会由原来的一致状态，变成另一种一致的状态。也就是说当事务提交后，或者当事务发生回滚后，数据库的完整性约束不能被破坏。
- 隔离性（Isolation）：如果有多个事务并发执行，每个事务作出的修改必须与其他事务隔离。
- 持久性（Duration）：即事务完成后，对数据库数据的修改被持久化存储，即使系统崩溃。（通过日志完成）

***Oracle是默认支持事务的，而MySQL中要使用合适的存储引擎（InnoDB）。***

### 控制语句
- `START TRANSACTION` 或 `BEGIN`: 显示开启事务。
- `COMMIT`：提交事务，当提交事务后，对数据库的修改是永久性的。
- `ROLLBACK` 或者 `ROLLBACK TO [SAVEPOINT]`: 回滚事务。撤销正在进行的所有没有提交的修改，或者将事务回滚到某个保存点。
- `SAVEPOINT`：创建保存点，一个事务中可以存在多个保存点。
- `RELEASE SAVEPOINT`：删除某个保存点。
- `SET TRANSACTION`: 设置事务的隔离级别。

### 隐式事务&显式事务
隐式事务实际上就是自动提交，Oracle 默认不自动提交，需要手写 COMMIT 命令，而 MySQL 默认自动提交（配置自动提交：`SET AUTOCOMMIT=1;`）。

自动提交模式下，每执行一条SQL语句都会自动COMMIT，也就是说每一条SQL语句都是独立事务。

非自动提交模式下，开启一个事务需要显示的开启，并需要手动COMMIT。

在MySQL中还需要考虑 `completion_type`的配置项。
- completion=0，这是默认情况。也就是说当我们执行 COMMIT 的时候会提交事务，在执行下一个事务时，还需要我们使用 START TRANSACTION 或者 BEGIN 来开启。
- completion=1，这种情况下，当我们提交事务后，相当于执行了 COMMIT AND CHAIN，也就是开启一个链式事务，即当我们提交事务之后会开启一个相同隔离级别的事务。
- completion=2，这种情况下 COMMIT=COMMIT AND RELEASE，也就是当我们提交后，会自动与服务器断开连接。

### 隔离级别
隔离性是为了防止数据库在并发处理时出现数据不一致的情况。在最严格的情况下，我们可以采用串行的方式，这样同一时刻只会有一个事务在执行，也就不会出现数据不一致的情况，当然也就没有并发了。在实际生产环境中，为了提高数据库的吞吐能力，往往需要牺牲掉一定的正确性来换取。

在SQL92标准中，定义了数据库在并发状态下出现的3种异常，以及4种隔离级别来处理该异常。关系如下：

 |隔离级别\异常 |脏读|不可重复读|幻读
---|---|---|---|---
读未提交|允许|允许|允许
读已提交|禁止|允许|允许
可重复读|禁止|禁止|允许
可串行化|禁止|禁止|禁止

3种异常：
- 脏读（Dirty Read）：在一个事务内，读到另一个事务更新后但未提交的数据。如果另一个事务回滚，那么当前事务读到的数据就是脏数据。（一次读取）
- 不可重复读（Nnrepeatable Read）：在一个事务内，多次读同一数据，在这个事务还没有结束时，如果另一个事务恰好更新了该数据。导致多次读取结果不一致。（多次读取）
- 幻读（Phantom Read）：在一个事务内，更新某数据时，也有其他事务在更新该数据。导致更新后的结果并不是预想的值，产生”幻觉“。（一次更新，多次读取）


4种隔离级别：
- 读未提交（READ UNCOMMITTED）：允许读到未提交的数据，这种情况下查询是不会使用锁的，可能会产生脏读、不可重复读、幻读等情况。
- 读已提交（READ COMMITTED）：只能读到已经提交的内容，可以避免脏读的产生，属于 RDBMS 中常见的默认隔离级别（比如说 Oracle 和 SQL Server），但如果想要避免不可重复读或者幻读，就需要我们在 SQL 查询的时候编写带加锁的 SQL 语句
- 可重复读（REPEATABLE READ）：保证一个事务在相同查询条件下两次查询得到的数据结果是一致的，可以避免不可重复读和脏读，但无法避免幻读。MySQL 默认的隔离级别就是可重复读。
- 可串行化（SERIALIZABLE）：将事务进行串行化，也就是在一个队列中按照顺序执行，可串行化是最高级别的隔离等级，可以解决事务读取中所有可能出现的异常情况，但是它牺牲了系统的并发性。


## 存储过程（Stored Procedure）
存储过程和视图类似，都是对SQL语句的封装。但是其有不同于视图，存储过程可以直接操作底层数据结构，能够实现更加复杂的数据处理。

存储过程可以有输入，也可以返回输出。由SQL语句和流控制语句组成。

创建存储过程的格式如下：
```
CREATE PROCEDURE 存储过程名称([参数列表])
BEGIN
    需要执行的语句
END    
```

如定义一个名为“add_num”的累加器：
```
-- DELIMITER //
CREATE PROCEDURE `add_num`(IN n INT)
BEGIN
       DECLARE i INT;
       DECLARE sum INT;
       
       SET i = 1;
       SET sum = 0;
       WHILE i <= n DO
              SET sum = sum + i;
              SET i = i +1;
       END WHILE;
       SELECT sum;
END
-- END //
-- DELIMITER ;
```
- 因为SQL的默认结束符为“;”，为防止上面语句块中的“;”破坏整体性，需要在开始处定义新的结束符，最后再改回默认结束符。(当使用Navicat等管理工具时会自动完成该工作。)
- 注释用“--”。

其他命令：
```
CALL add_num(50);  # 调用
DROP PROCEDURE add_num;  # 删除
ALTER PROCEDURE ....  # 修改

SHOW PROCEDURE STATUS;  # 查看当前数据的所有存储过程
SHOW PROCEDURE STATUS WHERE db = <db_name>;  # 查看指定数据的所有存储过程
SHOW CREATE PROCEDURE <procedure_name>;  # 参考详情
```


### 参数类型
- IN：输入参数。可用于存储过程中计算使用，但不能被返回。
- OUT：输出参数。用于将存放计算结果，不能参与计算，但可以被返回。
- INOUT：是IN和OUT的集合。既可以输入也可以输出。

如：
```
CREATE PROCEDURE `get_hero_scores`(
       OUT max_max_hp FLOAT,
       OUT min_max_mp FLOAT,
       OUT avg_max_attack FLOAT,  
       s VARCHAR(255)  # 默认为 IN 型
       )
BEGIN
       SELECT MAX(hp_max), MIN(mp_max), AVG(attack_max) FROM heros WHERE role_main = s INTO max_max_hp, min_max_mp, avg_max_attack;  # 计算结果并赋值给输出变量
END
```

使用如下：
```
CALL get_hero_scores(@max_max_hp, @min_max_mp, @avg_max_attack, '战士');
SELECT @max_max_hp, @min_max_mp, @avg_max_attack;
```

### 流控制语句
- BEGIN...END: 中间可以包含多个语句，并且每个语句都以“;”为结束符。
- DECLARE：变量声明，需要在`BEGIN...END`中并在其他语句之前。
- SET: 变量赋值。
- SELECT...INTO: 变量赋值，将计算结果放到输出变量中。
- IF...THEN...[ELSE...ELSEIF...]ENDIF: 条件判断语句。
- CASE WHEN...THEN...[ELSE...]END: 分支判断。
- LOOP、LEAVE、ITERATE：分别是：循环、结束循环、跳过本次循环。类似python中`for`、`break`、`continue`。
- REPEAT...UNTIL...END:首先会执行一次循环，然后在 UNTIL 中进行表达式的判断，如果满足条件就退出，即 END REPEAT；如果条件不满足，则会就继续执行循环，直到满足退出条件为止。
- WHILE...DO...END WHILE：这也是循环语句，和 REPEAT 循环不同的是，这个语句需要先进行条件判断，如果满足条件就进行循环，如果不满足条件就退出循环。

### 特点
优点：
- 一次编译多次使用，提高执行效率。
- 可以重复使用，减少工作量。
- 安全性高，网络传输量小。

缺点：
- 可移植性查，不同DBMS支持不同，需要重新编写。
- 调试困难，只有少数DBMS支持调试功能。增加了开发和维护成本。
- 不适用高并发场景。高并发的场景需要减少数据库的压力，有时数据库会采用分库分表的方式，而且对可扩展性要求很高，在这种情况下，存储过程会变得难以维护，增加数据库的压力，显然就不适用了。

***存储过程的特点就是“优点明显、缺点突出”，实际使用中还是要结合实际情况。***

## 游标
一般我们在编写SQL语句时，大多是面向集合（结果集）的操作。游标则是数据库提供的一种更加灵活的数据操作方式，允许我们面向过程操作数据。首先使用游标时，系统会为用户在内存中设置缓存区，用来存放结果集。然后我们可以通过移动游标的方式，来定位结果集中的任意数据，从而实现面向过程的操作。游标一般在复杂的存储过程中会使用。使用游标有如下几个步骤：
- 定义游标
```
DECLARE <cursor_name> CURSOR FOR SELECT ....;  
```
- 打开游标
```
OPEN <cursor_name>;
```
- 获取数据
```
FETCH <cursor_name> INTO <var>, <var> ...;  # 获取当前行数据，并将列数据存入对应变量中。游标自动移到下一行。 
```
- 关闭游标
```
CLOSE <cursor_name>;
```
- 释放游标
```
DEALLOCATE <cursor_name>;  # 如果不释放，则会一直在内存中，直至连接断开。
```
***不同DBMS语法略有不同，这样以MySQL为例。***

下面是一个简单的例子，定义了一个`wishing_you_prosperity`存储过程，并根据`programmer`表中的`age`值修改`wealth`值：
```
CREATE PROCEDURE `wishing_you_prosperity`()
BEGIN
    -- 创建接收游标的变量
    DECLARE temp_id INT;  
    DECLARE temp_age INT;
    DECLARE temp_wealth INT; 
    DECLARE prosperity INT; 

    -- 创建结束标志变量  
    DECLARE done INT DEFAULT false;
       
    -- 定义游标     
    DECLARE cur_programmer CURSOR FOR SELECT id, age, wealth FROM programmer;
    -- 定义”NOT FOUND“异常，用来判断循环结束  
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = true;  
       
    OPEN cur_programmer;  
    -- 循环读取数据
    FETCH cur_programmer INTO temp_id, temp_age, temp_wealth;
    REPEAT
        IF NOT done THEN
            IF temp_age <= 30 THEN
                SET prosperity = temp_wealth + 1000000;
            ELSEIF temp_age > 30 AND temp_age <= 35 THEN
                SET prosperity = temp_wealth + 1500000;
            ELSEIF temp_age > 35 AND temp_age <= 45 THEN
                SET prosperity = temp_wealth + 2500000;
            ELSE 
                SET prosperity = temp_wealth * 2;
            END IF;
            -- 更新数据
            UPDATE programmer SET wealth = prosperity WHERE id = temp_id;
        END IF;
    FETCH cur_programmer INTO temp_id, temp_age, temp_wealth;
    UNTIL done = true END REPEAT;  -- 判断退出条件
       
    CLOSE cur_programmer;
    -- Navicat 中不需要
    -- DEALLOCATE cur_programmer；
END
```

## 实用SQL语句
- 插入数据，如果已存在就先删除再插入
```
REPLACE INTO students (id, class_id, name, gender, score) VALUES (1, 1, '小明', 'F', 99);
```

- 插入数据，如果已存在就更新数据
```
INSERT INTO students (id, class_id, name, gender, score) VALUES (1, 1, '小明', 'F', 99) ON DUPLICATE KEY UPDATE name='小明', gender='F', score=99;
```
- 插入数据，如果已存在则跳过，什么也不做
```
INSERT IGNORE INTO students (id, class_id, name, gender, score) VALUES (1, 1, '小明', 'F', 99);
```

- 将结果集保存到新的数据表中
```
CREATE TABLE students_of_class1 SELECT * FROM students WHERE class_id=1;
```

- 将结果集添加到已有的数据表中
```
INSERT INTO statistics (class_id, average) SELECT class_id, AVG(score) FROM students GROUP BY class_id;
```


# 参考
- [廖雪峰老师的SQL教程](https://www.liaoxuefeng.com/wiki/1177760294764384)
- 极客时间-SQL必知必会（陈旸）
- [MySQL中全局变量、会话变量、用户变量和局部变量的区别](https://blog.csdn.net/albertsh/article/details/103421646)




