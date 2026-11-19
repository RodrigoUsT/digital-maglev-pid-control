# Digital Magnetic Levitation (MagLev) Control System

Closed-loop digital control system for a **magnetic levitation (MagLev)** setup designed to suspend a ferromagnetic target in mid-air using an electromagnet driven by a discrete **PID controller** on Arduino.

Developed as the final project for the *Digital Control Systems* course in the **B.Sc. in Electronic Engineering** program at **Universidad Mariano Gálvez de Guatemala**.

---

## System Overview

Magnetic levitation is an inherently open-loop unstable system: slight variations in position cause the object to either fall due to gravity or snap toward the electromagnet due to nonlinear magnetic attraction ($F_m \propto i^2 / y^2$). A real-time closed-loop digital controller continuously senses position, computes the control effort, and modulates coil power to maintain equilibrium.

### Key Hardware & Software Components

* **Microcontroller:** Arduino (8-bit AVR ADC & PWM output) operating at a fixed sampling period of **$T = 10\text{ ms}$** (`100 Hz` loop rate).
* **Position Sensor & Signal Conditioning:** Infrared emitter (`D2`) and photodiode (`D1`) pair with a pull-up resistor (`R1 = 10kΩ`), supply stabilization (`R4 = 100Ω`, `C2 = 4.7µF`), and an **RC low-pass filter** (`R2 = 920Ω`, `C1 = 5.2nF`) feeding analog pin `A0` to attenuate high-frequency electromagnetic noise before derivative action.
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

Linearizing around an equilibrium operating point $(y_0, i_0)$ via Taylor series yields a second-order model with an unstable pole:

$$G(s) = \frac{K}{s^2 - a}$$

Because the open-loop plant contains a right-half-plane pole, active closed-loop feedback is required for stabilization.

### Discrete PID Implementation

At each $10\text{ ms}$ sampling interval, the controller computes the error and updates the control law in discrete recurrence form:

$$u[k] = K_p e[k] + K_i \sum e[k] T + K_d \frac{e[k] - e[k-1]}{T}$$

* **Proportional (`Kp = 40.0`):** Provides immediate corrective response to position error.
* **Integral (`Ki = 0.6`):** Eliminates steady-state error, constrained with anti-windup limits to prevent integrator windup.
* **Derivative (`Kd = 1.5`):** Adds damping to reduce oscillations and improve transient stability.
* **Safety / Dropout Handling:** Resets PWM output and integral accumulator if the target leaves the valid sensing range (`posicionReal > 600`).

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
```

---

## Key Takeaways & Lessons Learned

* **Deterministic Sampling Matters:** Maintaining a consistent $10\text{ ms}$ sampling period is essential for repeatable discrete control performance on an unstable plant.
* **Signal Conditioning Before Derivative Control:** Since derivative action amplifies high-frequency sensor fluctuations, hardware RC filtering on the IR receiver signal prevents noisy PWM switching, excess coil buzz, and MOSFET thermal stress.
* **Theory vs. Physical Tuning:** Taylor linearization and discrete-time modeling establish the controller structure, while experimental gain tuning compensates for unmodeled nonlinearities, ambient infrared interference, and coil heating.
* **Full Documentation:** The complete academic report (in Spanish), including mathematical derivations, circuit schematics, prototype photographs, and references, is available in `docs/Proyecto final Sistema de Control Digital
* .pdf`.

---

## Authors

Developed by Electronic Engineering students at **Universidad Mariano Gálvez de Guatemala** (*Sistemas de Control Digital*):

* **Rodrigo José Us Tumax**
* Adriana Lucia Aguirre
* Luis Fernando Chinchilla Mijangos
* Yankel Alberto Mazariegos Davila
* Luis Alfonso Muñoz Bonilla
* Ángel Gabriel Osoy Veliz
* Oscar Samuel Reynoso Peralta
* Heber Enrique Sandoval Cifuentes
* Sarah Judith Del Rosario Zamora López
* Jordy Donovan Escobar Monroy
