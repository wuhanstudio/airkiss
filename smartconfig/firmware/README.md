# Airkiss 一键配网

这是利用 Airkiss 进行一一键配网的固件源码。

## 目录结构

| 目录    | 说明             |
| ------- | ---------------- |
| dht11   | 温湿度传感器节点 |
| ds18b20 | 温度传感器节点   |
| oled    | 液晶显示节点     |
| relay   | 继电器节点       |


## 编译运行

可以参考 [这里](https://github.com/esp8266/Arduino) 配置好 Arduino 的 esp8266 开发环境，然后直接在 Arduino IDE 编译上传就可以了。

当然，记得根据自己的板子选择合适的开发板型号，不同开发板的 flash 大小不一样，所以代码和文件系统的分配是不同的。

## 联系方式

- 维护：吴晗 张信峰
- 主页：<http://wuhanstudio.cc>
- 邮箱：[wuhanstudio@hust.edu.cn](mailto:wuhanstudio@hust.edu.cn)
