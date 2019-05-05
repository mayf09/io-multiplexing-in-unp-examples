# 比较 select poll

从《UNIX 网络编程》 IO 复用部分改编。打印 fd_set, pollfd 数据结构。
详情查看 [《UNIX 网络编程》中的 I/O 多路复用](http://mayunfei.net/2019/04/io-multiplexing-in-unp/) 。

## server

使用 `make all` 编译程序。

使用 `./echo-select` 或 `./echo-poll` 来执行 select 或 poll 回射程序。

## client

使用 `telnet 127.0.0.1 9999` 。