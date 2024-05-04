# CNC_Chuck
Arduino based Automatic Chuck control with LCD and Servo motors

### Connections

- D0 -> LIMIT_SW_1 (PULL_UP)
- D13 -> LIMIT_SW_2 (PULL_UP)
- D3 -> LIMIT_SW_3 (PULL_UP)
- D2 -> LIMIT_SW_4 (PULL_UP)

- D3 -> Clamp Servo PWM
- D5 -> JAW Servo 1 PWM
- D6 -> JAW Servo 2 PWM
- D9 -> JAW Servo 3 PWM
- D10 -> JAW Servo 4 PWM

- D12 ->  LENGTH_BUTTON (PULL_UP)
- D11 -> BREADTH_BUTTON (PULL_UP)
- D8 -> RESET_BUTTON (PULL_UP)
- D7 -> SUBMIT_BUTTON (PULL_UP)

- A0 -> MOTOR_SPEED_POT

- A4 -> LCD_SCL 
- A5 -> LCD_SDA
