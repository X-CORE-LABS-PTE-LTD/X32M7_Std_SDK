# X32M7xx Standard Peripheral SDK

Official Standard Peripheral Driver SDK for X-CORE LABS X32M7xx series microcontrollers.

## Overview
The X32M7xx SDK provides a standardized set of APIs for hardware peripherals, built on top of the ARM® Cortex®-M7 core and CMSIS architecture.

## Repository Structure
- **Drivers/CMSIS**: Hardware abstraction layer and register definitions.
- **Drivers/X32M7xx_StdPeriph_Driver**: Standard peripheral drivers (GPIO, UART, SPI, etc.).
- **Middlewares**: Optional middleware components (USB, etc.).

## Getting Started
1. Include the `Drivers` folder in your project.
2. Add the following paths to your compiler's include paths:
   - `Drivers/CMSIS/Include`
   - `Drivers/X32M7xx_StdPeriph_Driver/inc`
3. Include `x32m7xx.h` in your application code.

## License
This SDK is proprietary software owned by **X-CORE LABS PTE. LTD.** 
Please refer to [LICENSE.md](LICENSE.md) for detailed terms and conditions.

---
© 2026 X-CORE LABS PTE. LTD. All Rights Reserved.
