# Digital Magnetic Levitation (MagLev) Control System

Closed-loop digital control system for a **magnetic levitation (MagLev)** setup designed to suspend a ferromagnetic target in mid-air using an electromagnet driven by a discrete **PID controller** on Arduino.

This project was developed as the final project for the *Digital Control Systems* course in the **B.Sc. in Electronic Engineering** program at **Universidad Mariano Gálvez de Guatemala**[cite: 2].

---

## System Overview

Magnetic levitation is an inherently open-loop unstable system: slight variations in position cause the object to either fall due to gravity or snap toward the electromagnet due to nonlinear magnetic attraction ($F_m \propto i^2 / y^2$). A real-time closed-loop digital controller continuously senses position, computes the control effort, and modulates coil power to maintain equilibrium.

### Key Hardware & Software Components
* **Microcontroller:** Arduino (8-bit AVR ADC & PWM output) with a fixed sampling period of **$T = 10\text{ ms}$** (`100 Hz` loop rate).
* **Position Sensor & Signal Conditioning:** Infrared emitter/photodiode pair with a pull-up resistor, stabilization capacitor, and an **RC low-pass filter** feeding analog pin `A0` to mitigate electromagnetic noise from the coil before derivative action.
* **Power & Actuation Stage:**
  * **TC4420VAT** high-speed MOSFET gate driver to source/sink gate charge rapidly from the Arduino PWM pin (`Pin 9`).
  * **STP32N06L** logic-level N-channel power MOSFET switching the electromagnet load (`15 V` supply).
  * **MBR1060G** Schottky flyback diode to suppress inductive voltage spikes during switching and protect the power stage.
* **Control Algorithm:** Discrete-time PID implemented via recurrence equations with integral anti-windup clamping and PWM output saturation (`0`–`255`).

---

## Control & Mathematical Modeling

### Nonlinear Dynamics & Linearization
From Newton's second law and the approximate electromagnetic force relation:

$$m \frac{d^2 y}{dt^2} = m g - F_m, \qquad F_m \approx K \frac{i^2}{y^2}$$

Linearizing around an equilibrium operating point $(y_0, i_0)$ via Taylor series yields a second-order model with an unstable pole that requires feedback stabilization.

### Discrete PID Implementation
At each $10\text{ ms}$ sampling interval, the controller computes the error and updates the control law in discrete form:

$$u[k] = K_p e[k] + K_i \sum e[k] T + K_d \frac{e[k] - e[k-1]}{T}$$

* **Proportional (`Kp = 40.0`):** Provides immediate response to position error.
* **Integral (`Ki = 0.6`):** Eliminates steady-state error, constrained with anti-windup limits to prevent integrator windup.
* **Derivative (`Kd = 1.5`):** Adds damping to reduce oscillations and improve transient stability.
* **Safety / Dropout Handling:** Resets PWM and integral state when the sensor reading exceeds the threshold (`posicionReal > 600`).

---

## Repository Structure

```text
digital-maglev-pid-control/
├── README.md
├── .gitignore
├── firmware/
│   └── maglev_pid_controller/
│       └── maglev_pid_controller.ino
├── hardware/
│   ├── ir_sensor_circuit.png
│   └── electromagnet_driver_circuit.png
├── media/
│   └── physical_setup.jpg
└── docs/
    └── Proyecto final Sistema de Control Digital (1)-1.pdf
