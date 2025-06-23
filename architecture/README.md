# Computer Organization and Architecture

## Bit and Byte

The smallest unit of information in a digital system is **bit**. A bit is binary which holds either of the two values 0 or 1. A number of bits are placed together to represent larger range of values. A **byte** is composed of 8 bits together to form a value. The byte is the smallest unit of information that be read from or written to by most modern processor.

## 6502 microprocessor

6502 is a famous processor that was used in console games like atari and such. It has word length of 8-bit. It means it access memory by reading or writing 8 bits to register or memory. It has 16 bit memory bus, which enables it to access 64KB memory.

The processor contains 5 8-bit registers - A, X, Y, S and P. A register works as the accumulator for arithmetic operations. X and Y can be used as index registers in calculating memory addresses. Another register is there which is 16-bit. It is the PC register. It is the program counter that points to the next instruction to be executed by the processor. Because of 16-bit size it can access all the memory addresses in 6502 microprocessor. Every instruction has 1-byte opcode, another 2 bytes optional operands depending on the type of instruction.

The S register is the stack pointer. It points to memory address of range $0100 to $01FF. The stack memory of 256-bytes is used for temporary storage of parameters in subroutines, stores the return address when a subroutine is called.

To learn more about the instruction set, go visit this [site](http://www.6502.org/users/obelisk/6502/instructions.html).

If you want to emulate the 6502 microprocessor instructions, use this [easy 6502 by skilldrick](https://skilldrick.github.io/easy6502/).

## Processor Elements

Following processor elements are the ones we are learning -

- a simple processor
- instruction set
- addressing modes
- instruction categories
- interrupt processing
- input/output operations

### A simple processor

The integrated circuit at the core of a computer is known as **central processing unit** (CPU), microprocessor or simply processor. A microprocessor is the single integrated circuit that implements the functions of the processor.

A typical processor has three logically distinct functional units -

- **Control unit** manages the overall operation of the device. It executes the next instruction, decoding the instruction to determine the operation to perform, and distributing the execution of the instructions to appropriate elements.
- **Arithmetic logic unit**(ALU) is a combinational circuit that does arithmetic operations and bit manipulation operations.
- **Register set**: provides temporary storage as well as source and destination locations for instruction inputs and outputs.

The following diagram shows the connection between control unit, ALU, registers and memory input/outputs -

![interaction between processor elements](./images/processor.png)

### Control Unit

Control unit interpret the processor instructions and manage their execution by calling other functional components of the processor and also using memory which is an external element.

Memory here refers to *Random Access Memory*(RAM). Here memory components could be keyboard, mouse, speaker, microphone, disk, graphical video displays. It could also be other common I/O devices like network interfaces, Wi-Fi and Bluetooth.

When a computer is powered on, it resets all internal components to a defined value. Then the CPU loads the **Program Counter**(PC) register with the memory location of the first instruction. Software engineers who write the lowest level software systems must configure their developement tools to produce a *code memory* that begins execution at the address required by the processor architecture.

> "code memory" is the segment where the processor instructions are stored.