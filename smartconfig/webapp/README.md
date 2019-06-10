# Airkiss 一键配网

这是利用 Airkiss 进行一一键配网的网页应用。


## 目录结构

| 目录   | 说明              |
| ------ | ----------------- |
| client | 网页客户端源码    |
| server | NodeJS 服务器源码 |


## 编译运行

由于我们一开始希望将网页服务器搭建在嵌入式的小板子上，无论是 flash 还是 ram 都非常的有限，所以尽量避免了使用 Vue, React, Bootstrap，而采用原生的 Javascript, html, css 实现，但是不知不觉中还是使用了 ES6 的特性，所以为了支持更多的浏览器，建议采用 babel 进行转换打包。

另一方面，由于现场我们是在局域网测试，所有的 ip 地址都是固定的局域网 ip，因此客户端需要根据自己的实际情况修改服务器的 ip 地址。


服务器：

​    npm install

​    node server/server.js

客户端：

​    npm install -g live-server

​    live-server ./client


##  联系方式

- 维护：吴晗 张信峰
- 主页：<http://wuhanstudio.cc>
- 邮箱：[wuhanstudio@hust.edu.cn](mailto:wuhanstudio@hust.edu.cn)
