# Airkiss

这个项目是 Airkiss 协议的实现以及 3 个应用，同时包含了 Airkiss 的介绍，以及其他相关项目的链接，希望能够帮助到对 Airkiss 感兴趣的人。

这个项目最初是在联创 Hackday 24 小时搭建起来的，所以有些代码实现可能比较简单粗暴，之后有机会也会慢慢完善的。





## 项目结构

三个目录分别为独立的应用示例，每个目录下均有独立的 README

| 目录        | 说明                                                         |
| ----------- | ------------------------------------------------------------ |
| smartconfig | 传统应用，利用 Airkiss 一键配网，同时完成多个传感器与路由器的连接 |
| mesh        | 利用 Airkiss 进行多个传感器之间的无连接通信，例如在没有 wifi 连接的情况下把文本发送到多个显示屏上进行显示 |
| hacking     | 利用 Airkiss 在没有网络连接的情况下向主机发送命令，或者协助主机联网 |



## 相关介绍

这里对 Airkiss 相关的项目做一个总结，当然也有很多其他很好的项目，因为我也没有一一尝试所以没有加入进来，欢迎一起完善。



1. Airkiss 文档

   当然首先是腾讯的[官方文档](https://iot.weixin.qq.com/wiki/document-7_1.html)，不过介绍不算太详细。当前目录下也有个相对详细的 pdf 文档，但是相对有些过时了，自然也是可以直接看对应的源码实现。

   

2. Airkiss 发送端

   - Linux
     
     - [CaptainDouche](https://github.com/CaptainDouche/esp_smartcfg_linux) 
     
   - Windows
     
     - [KhonKaenMakerClub](https://github.com/KhonKaenMakerClub/.NetSmartConfig) (C#)
     - [Detao](https://github.com/Detao/AirKiss) (Qt  实现)
     
   - Android
     
     - [expressif](https://github.com/EspressifApp/EspRelease) (乐鑫官方)

     - [zhchbin](https://github.com/zhchbin/WeChatAirKiss)
     - [chengxinping](https://github.com/chengxinping/AirKissDemo)
     
   - IOS

     - https://github.com/James-oc/JMAirKiss

     

3. Airkiss 接收端

   - Linux：
     - [pannzh](https://github.com/pannzh/Airkiss)
   - ESP8266
     - [yulong88888](https://github.com/yulong88888/Arduino_ESP8266_WeChat_AirkissAndNFF) (Arduino)
 - RT-Thread
      - [heyuanjie87](http://packages.rt-thread.org/itemDetail.html?package=airkissOpen)

