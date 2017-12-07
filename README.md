
# master_work简介

master_work是一个由C开发的，方便快速搭建后端服务的系统。其进程模型（多进程单线程模型）是参照nginx构造的，一部分代码是从nginx里搬过来的，


## 如何使用：

## 理解master_work：
### 1. 进程模型
### 2. 事件驱动
### 3. recv buf
### 4. send buf
### 5. 内存池

## 待补充：


## 主要功能：

### 1. 基础功能

- 支持SQL读写分离。
- 支持透明的MySQL连接池，不必每次新建连接。
- 支持平滑上线DB或下线DB，前端应用无感知。
- 支持多个slave，slave之间通过权值进行负载均衡。
- 支持强制读主库。
- 支持主流语言（java,php,python,C/C++,Go)SDK的mysql的prepare特性。
- 支持到后端DB的最大连接数限制。
- 支持SQL日志及慢日志输出。
- 支持SQL黑名单机制。
- 支持客户端IP访问白名单机制，只有白名单中的IP才能访问kingshard。
- 支持字符集设置。
- 支持last_insert_id功能。
- 支持动态修改kingshard部分配置项（具体参考管理端命令）。
- 支持以Web API调用的方式管理kingshard。

### 2. sharding功能

- 支持按整数的hash和range分表方式。
- 支持按年、月、日维度的时间分表方式。
- 支持跨节点分表，子表可以分布在不同的节点。
- 支持跨节点的count,sum,max和min等聚合函数。
- 支持单个分表的join操作，即支持分表和另一张不分表的join操作。
- 支持跨节点的order by,group by,limit等操作。
- 支持将sql发送到特定的节点执行。
- 支持在单个节点上执行事务，不支持跨多节点的分布式事务。
- 支持非事务方式更新（insert,delete,update,replace）多个node上的子表。

## kinshard文档

### kingshard安装和使用

[1.安装kingshard](./doc/KingDoc/kingshard_install_document.md)

[2.如何利用一个数据库中间件扩展MySQL集群——kingshard使用指南](./doc/KingDoc/how_to_use_kingshard.md)

[3.kingshard sharding介绍](./doc/KingDoc/kingshard_sharding_introduce.md)

[4.kingshard按时间分表功能介绍](./doc/KingDoc/kingshard_date_sharding.md)

[5.kingshard 快速入门](./doc/KingDoc/kingshard_quick_try.md)

[6.管理端命令介绍](./doc/KingDoc/admin_command_introduce.md)

[7.管理端Web API接口介绍](./doc/KingDoc/kingshard_admin_api.md)

[8.kingshard SQL黑名单功能介绍](./doc/KingDoc/sql_blacklist_introduce.md)

[9.kingshard的FAQ](./doc/KingDoc/function_FAQ.md)

[10.kingshard SQL支持范围](./doc/KingDoc/kingshard_support_sql.md)

[11.如何配合LVS实现集群部署](./doc/KingDoc/how_to_use_lvs.md)

### kingshard架构与设计

[1.kingshard架构设计和功能实现](./doc/KingDoc/architecture_of_kingshard_CN.md)

[2.kingshard性能优化之网络篇](./doc/KingDoc/kingshard_performance_profiling.md)

[3.kingshard性能测试报告](./doc/KingDoc/kingshard_performance_test.md)
## 鸣谢
- 感谢[mixer](https://github.com/siddontang/mixer)作者siddontang, kingshard最初的版本正是基于mixer开发而来的。
- 感谢[bigpyer](https://github.com/bigpyer)，他对kingshard做了详细的性能测试，并撰写了一份非常详细的测试报告。
- 感谢以下[开源爱好者](https://github.com/flike/kingshard/graphs/contributors)为kingshard做出的贡献。

## kingshard用户列表

https://github.com/flike/kingshard/issues/148

## 反馈
kingshard开源以来，经过不断地迭代开发，功能较为完善，稳定性有较大提升。 **目前已有上二十家公司在生产环境使用kingshard作为MySQL代理。** 如果您在使用kingshard的过程中发现BUG或者有新的功能需求，请发邮件至flikecn#126.com与作者取得联系，或者加入QQ群(147926796)交流。
欢迎关注**后端技术快讯**公众号，有关kingshard的最新消息与后端架构设计类的文章，都会在这个公众号分享。

<img src="./doc/KingDoc/wechat_pic.png" width="20%" height="20%">

## License

kingshard采用Apache 2.0协议，相关协议请参看[目录](./doc/License)