# CawFOC (Rust)

---

本项目为CawFOC的新版固件，采用Rust语言开发，使用Embassy进行异步设计


## 注意

* DAP-LINK需要连接CawDrive的Reset引脚，否则probe-rs调试时会报错
* 烧录后需要拔掉DAP-LINK，或者将Reset线拔掉，否则STM32无法正常工作