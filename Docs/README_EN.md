# 🐦 CawDrive - Brushless Motor Driver

---

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/H2H3PQZVW)

# 🚀 Introduction

- Supports 24V input

- External encoder interface support: Hall, SPI, I2C

- Main control chip: STM32F446, motor driver chip: DRV8323RSRGZR

- Onboard magnetic encoder: MT6835GT-STD (21-bit)

- Supports motor coil temperature sampling and driver temperature sampling

- Supports 3-phase low-side current sampling

- Supports bus voltage sampling

- Supports CAN and USART communication

- Supports SWD download and debugging

- Supports 6xPWM and 3xPWM control

# 🗂️ Directory

* Firmware: Legacy code (deprecated)

* Firmware_F446: Actual control code for FOC control and STM32CubeMX configuration

* PCB: FOC driver schematics, PCB, and generated Gerber files

# 📦 Firmware Compilation (Under Development)

## Configure the caw-embedded Library

The current firmware compilation depends on the caw-embedded project: https://github.com/fake-rick/caw-embedded

Dependency Addition Method 1:

You can directly clone the caw-embedded project into the `caw-drive/Firmware` directory:

```bash
cd caw-drive/Firmware
git clone git@github.com:fake-rick/caw-embedded.git caw_embedded
```

Dependency Addition Method 2:

Clone the caw-embedded project and create a symbolic link (Linux). Assuming the caw-embedded project is placed in the same level directory as caw-drive:

```bash
git clone git@github.com:fake-rick/caw-embedded.git
cd caw-drive/Firmware
ln -s ../../caw-embedded caw_embedded
```

# 🛠 VSCode Configuration

Install and configure the stm32-for-vscode plugin

# 👻 Image Showcase

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawdrive-1.png)

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawdrive-2.png)

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawdrive-3.png)

# 🖥️ Upper Computer

![](https://github.com/fake-rick/caw-drive/blob/master/Docs/imgs/cawstudio.png)

# 📝 References

https://github.com/bgkatz/3phase_integrated
