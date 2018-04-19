## libnice快速入门

### 功能

An implementation of the IETF’s Interactive Connectivity Establishment (ICE) standard。

[RFC5245](https://tools.ietf.org/html/rfc5245)

libnice库是基于ICE协议实现的一套通信连接库。 
主要功能是实现p2p连接及媒体数据流发送接收,其类似于webrtc源码中自带的libjingo。我们可在我们的项目中使用libnice库来实现端到端的ICE连接和数据流发送接收。以及candidates(候选地址)和SDP（媒体描述文件）的相互交换。 

libnice库是基于glibc语言的，跨平台，在linux和手机端都可使用，但需依赖于glib库。

### 例子
#### simple-example

使用libnice来协商两个客户机之间的UDP连接，可能在相同的网络上或在不同的NATs和/或有状态的防火墙后面。

[《WebRtc音视频实时通信--libnice库介绍》](https://blog.csdn.net/chenFteng/article/details/77429453)
#### thread-example

类似于simple_example,只是以多线程方式实现。

#### sdp-example

功能类似于前2个示例，但candidates候选地址不是直接发送给对端的，而是置于sdp文件中发送给对方的，本示例主要演示SDP文件的使用。
 通过将以上示例中的文本数据的发送转换成采集到的音视频数据包，便实现了流媒体数据交互。



### 源码下载

+ [源码地址](https://cgit.freedesktop.org/libnice/libnice/)
+ [源码Github镜像](https://github.com/libnice/libnice)



