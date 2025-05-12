![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/CawDrive-Fixed.png)

# 🐦 CawDrive - 无刷电机驱动器

---

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/H2H3PQZVW)

中文 | [English](https://github.com/fake-rick/caw-drive/blob/master/Docs/README_EN.md)

# 🚀 简介

- 支持 24 电压输入

- 外部编码器接口支持：霍尔，SPI，I2C

- 主控芯片：STM32F446，电机驱动芯片为 DRV8323RSRGZR

- 板载磁编码器：MT6835GT-STD（21bit）

- 支持电机线圈温度采样和驱动器温度采样

- 支持双路低端相电流采样

- 支持母线电压采样

- 支持CAN和USART通讯

- 支持SWD下载和调试

- 支持 6xPWM 和 3xPWM 的控制

# 🗂️ 目录

* Firmware：旧版代码（弃用）

* Firmware_F446：FOC控制的实际控制代码以及STM32CubeMX配置

* PCB：FOC驱动器原理图、PCB以及生成的Gerber文件

# 📦 固件编译（开发中）

## 配置caw-embedded库

当前固件编译依赖caw-embedded项目: https://github.com/fake-rick/caw-embedded

依赖添加方式1：

可以直接将caw-embedded项目克隆到caw-drive/Firmware目录下:

```bash
cd caw-drive/Firmware
git clone git@github.com:fake-rick/caw-embedded.git caw_embedded
```

依赖添加方式2：

将caw-embedded项目克隆后建立链接（Linux），假设将caw-embedded放在与caw-drive同级目录下:

```bash
git clone git@github.com:fake-rick/caw-embedded.git
cd caw-drive/Firmware
ln -s ../../caw-embedded caw_embedded
```

# 🛠 VSCode配置

安装并配置stm32-for-vscode插件

# 👻 图片展示

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawdrive-1.png)

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawdrive-4.png)

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawdrive-2.png)

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawdrive-3.png)

# 🖥️ 上位机

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawstudio.png)

# 📝 参考资料

https://github.com/bgkatz/3phase_integrated
