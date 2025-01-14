该引擎已被弃用，但新的引擎已经在开发中了：  
https://github.com/BDFFZI/Light
---
# BDXKEngine
我在试图自己开发一个游戏引擎，当然纯造轮子玩的那种。</br>
如果你有Unity的使用经验，入门将会非常轻松，因为我大多地方都是通过模仿Unity编写的。</br>
但实际上我并未看过Unity源码，所以只能靠对Unity表层做实验抠字眼外加一点灵感，来得到编写思路。</br>
既然没法得到Unity的源码，那我就自己做一款假的，这就是我最初的想法。</br>
开发进展演示：https://space.bilibili.com/35949109/channel/collectiondetail?sid=556316
## 关于该引擎的说明
- 目前并不是一个跨平台引擎，仅支持**Windows**平台，项目基于**sln**而不是**cmake**。
- 本质上是一个库而非应用程序，需要编译源文件使用，Editor部分会额外使用一些资源文件。
- 分为Engine和Editor两部分。Editor是基于Engine实现的扩展库，在打包正式游戏时可以去除。
- 一切引擎功能的开发都优先实现效果，而非性能优化。所以请将这一切都看作demo，仅供参考。
- 有关源码的信息请查看sources文件夹及其README.md的内容，一定要看！
## 如何编译项目
### 1. 确保Visutal Studio满足以下要求
- 安装了**使用C++的桌面开发**
- 安装了**适用于最新生成工具的 C++ ATL**
- 安装了**vcpkg**
### 2. 直接打开sln文件并运行即可
