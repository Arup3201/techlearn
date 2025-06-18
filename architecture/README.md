# Computer Organization and Architecture

## Bit and Byte

The smallest unit of information in a digital system is **bit**. A bit is binary which holds either of the two values 0 or 1. A number of bits are placed together to represent larger range of values. A **byte** is composed of 8 bits together to form a value. The byte is the smallest unit of information that be read from or written to by most modern processor.

## 6502 microprocessor

6502 is a famous processor that was used in console games like atari and such. It has word length of 8-bit. It means it access memory by reading or writing 8 bits to register or memory. It has 16 bit memory bus, which enables it to access 64KB memory.

The processor contains 5 8-bit registers - A, X, Y, S and P. A register works as the accumulator for arithmetic operations. X and Y can be used as index registers in calculating memory addresses. Another register is there which is 16-bit. It is the PC register. It is the program counter that points to the next instruction to be executed by the processor. Because of 16-bit size it can access all the memory addresses in 6502 microprocessor. Every instruction has 1-byte opcode, another 2 bytes optional operands depending on the type of instruction.

The S register is the stack pointer. It points to memory address of range $0100 to $01FF. The stack memory of 256-bytes is used for temporary storage of parameters in subroutines, stores the return address when a subroutine is called.

To learn more about the instruction set, go visit this [site](http://www.6502.org/users/obelisk/6502/instructions.html).

If you want to emulate the 6502 microprocessor instructions, use this [easy 6502 by skilldrick](https://skilldrick.github.io/easy6502/).

## Digital Circuits

### Electrical circuit 

V = IR - Ohm's Law

If we take the example of a pipe, the pressure of the water is the voltage V. The speed at which the water flows is the current I and the resistance the water feels due to the faucet of the pipe is R.

If we increase the faucet or the size of the valve at the beginning of the pipe, the water pressure will increase, if we increase the resistance then water speed will decrease. More water pressure means more water speed.

A circuit has multiple parts. There is a battery, wire and current. Batter has two ends, one end has zero voltage which is the voltage reference point - with a voltage of zero.

