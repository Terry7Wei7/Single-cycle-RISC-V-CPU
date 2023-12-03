# Assignment3: single-cycle RISC-V CPU
contributed by Terry7Wei7

## Complete MyCPU
The architecture diagram of MyCPU

![image](https://hackmd.io/_uploads/rkTMwAYrp.png)


According to the architecture diagram's data path and control signal, complete MyCPU and verify it through testing.This project provides several tests.
```
[info] Run completed in 2 minutes, 14 seconds.
[info] Total number of tests run: 9
[info] Suites: completed 7, aborted 0
[info] Tests: succeeded 9, failed 0, canceled 0, ignored 0, pending 0
[info] All tests passed.
```

## Waveform Analysis
### Instruction Fetch
![image](https://hackmd.io/_uploads/Hy_XyycBT.png)

io_jump_address_id is set to 0x00001000
From 2ps to 4ps, io_jump_flag_id = 1, so pc=pc+1. At the start of 6ps, io_jump_flag_id becomes 1, therefore at clock = 1, pc = 0x00001000.

### Instruction Decode
![image](https://hackmd.io/_uploads/BkB2byqSp.png)

This website can convert machine code into RISC-V instructions :
https://luplab.gitlab.io/rvcodecjs/#q=0x000022B7&abi=false&isa=RV32I
0x00A02223 --> sw x10, 4(x0), 0x000022B7 --> lui x5, 2

At 2ps, the instruction decoder received the 0x00A02223 instruction, which is an SW instruction. Therefore, the instruction decoder sets the io_memory_write_enable to 1. SW instructions do not require reading the content of memory, so the instruction decoder sets io_memory_read_enable to 0.

At 4ps, the instruction decoder received the 0x000022B7 instruction, which is a lui instruction. Consequently, the instruction decoder sets the io_memory_read_enable to 1. Lui instructions do not require modifying the content of memory, therefore the instruction decoder sets io_memory_write_enable to 0.

### Execution
![image](https://hackmd.io/_uploads/SJzaL1qBT.png)

In the Execution stage, the ALU needs to determine the operation to be executed and select the operands for the operation. In this example, the ALU selects reg1's data as op1 and reg2's data as op2.

### Combining into a CPU
Connect the signals together according to the CPU architecture diagram.

## Run my program on MyCPU
My Homework 2 program involves processing the binary representation of the input, calculating the count of leading zeros, and using this information to compute a priority encoding output.
https://hackmd.io/h94Ll9QqTJKu_9qjfaw03A?view
* Put the program into the csrc directory.
* Add the program to the BINS variable in the Makefile.
* Execute the 'make update' command. In the Makefile, gcc compiles .c files into .o (object files), then the ld linker links the object file with the linking script (link.lds) to generate an ELF executable file. As MyCPU doesn't have an operating system, it's necessary to use objcopy to convert the ELF file into a binary file.
```
make update
riscv-none-elf-as -R -march=rv32i_zicsr -mabi=ilp32 -o init.o init.S
riscv-none-elf-gcc -O0 -Wall -march=rv32i_zicsr -mabi=ilp32 -c -o fibonacci.o fibonacci.c
riscv-none-elf-ld -o fibonacci.elf -T link.lds --oformat=elf32-littleriscv fibonacci.o init.o
riscv-none-elf-objcopy -O binary -j .text -j .data fibonacci.elf fibonacci.asmbin
riscv-none-elf-gcc -O0 -Wall -march=rv32i_zicsr -mabi=ilp32 -c -o hello.o hello.c
riscv-none-elf-ld -o hello.elf -T link.lds --oformat=elf32-littleriscv hello.o init.o
riscv-none-elf-objcopy -O binary -j .text -j .data hello.elf hello.asmbin
riscv-none-elf-as -R -march=rv32i_zicsr -mabi=ilp32 -o mmio.o mmio.S
riscv-none-elf-ld -o mmio.elf -T link.lds --oformat=elf32-littleriscv mmio.o
riscv-none-elf-objcopy -O binary -j .text -j .data mmio.elf mmio.asmbin
riscv-none-elf-gcc -O0 -Wall -march=rv32i_zicsr -mabi=ilp32 -c -o quicksort.o quicksort.c
riscv-none-elf-ld -o quicksort.elf -T link.lds --oformat=elf32-littleriscv quicksort.o init.o
riscv-none-elf-objcopy -O binary -j .text -j .data quicksort.elf quicksort.asmbin
riscv-none-elf-as -R -march=rv32i_zicsr -mabi=ilp32 -o sb.o sb.S
riscv-none-elf-ld -o sb.elf -T link.lds --oformat=elf32-littleriscv sb.o
riscv-none-elf-objcopy -O binary -j .text -j .data sb.elf sb.asmbin
cp -f fibonacci.asmbin hello.asmbin mmio.asmbin quicksort.asmbin sb.asmbin ../src/main/resources
rm init.o quicksort.elf fibonacci.elf sb.elf mmio.elf hello.elf
```
Put the program into the csrc directory.
Add the program to the BINS variable in the Makefile.
```
$ make update
riscv-none-elf-as -R -march=rv32i_zicsr -mabi=ilp32 -o clz.o clz.S
riscv-none-elf-ld -o clz.elf -T link.lds --oformat=elf32-littleriscv clz.o
riscv-none-elf-ld: warning: cannot find entry symbol _start; defaulting to 00001000
riscv-none-elf-ld: clz.o: in function `main':
clz.c:(.text+0x3e8): undefined reference to `__umoddi3'
make: *** [Makefile:16: clz.elf] Error 1
```
> I rewrite the countLeadingZeros in the homework 2 program,The countLeadingZeros function we are using iterates through the bits from the most significant bit to the least significant bit using a loop. It finds the first bit that is set to 1 and calculates the count of leading zeros before it. This way, we no longer rely on any C library functions.
``` 
int countLeadingZeros(uint64_t x) {
    int count = 0;
    for (int i = 63; i >= 0; i--) {
        if ((x >> i) & 1) {
            break;
        }
        count++;
    }
    return count;
}

int main() {
    // Hard-coded input values for testing

    // Calculate leading zeros
    uint64_t x = 5;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    x |= (x >> 32);

    x -= ((x >> 1) & 0x5555555555555555);
    x = ((x >> 2) & 0x3333333333333333) + (x & 0x3333333333333333);
    x = ((x >> 4) + x) & 0x0f0f0f0f0f0f0f0f;
    x = (x * 0x200040008001ULL & 0x1111111111111111);

    int leading_zero = 64 - countLeadingZeros(x);
    int priority = 4 - 1 - leading_zero;

    // Write the results to specified memory locations
    *((volatile int *) (4)) = leading_zero;
    *((volatile int *) (8)) = priority;

    return 0;
}

```
```
$ make update
riscv-none-elf-as -R -march=rv32i_zicsr -mabi=ilp32 -o clz.o clz.S
riscv-none-elf-ld -o clz.elf -T link.lds --oformat=elf32-littleriscv clz.o
riscv-none-elf-ld: warning: cannot find entry symbol _start; defaulting to 00001000
riscv-none-elf-objcopy -O binary -j .text -j .data clz.elf clz.asmbin
cp -f clz.asmbin fibonacci.asmbin hello.asmbin mmio.asmbin quicksort.asmbin sb.asmbin ../src/main/resources
rm clz.elf
```
* Extend the Scala code in “src/test/scala/riscv/singlecycle/CPUTest.scala” to include clz test items.
```
class clz extends AnyFlatSpec with ChiselScalatestTester {
  behavior.of("Single Cycle CPU")
  it should "countLeadingZeros" in {
    test(new TestTopModule("clz.asmbin")).withAnnotations(TestAnnotations.annos) { c =>
      for (i <- 1 to 50) {
        c.clock.step(1000)
        c.io.mem_debug_read_address.poke((i * 4).U) // Avoid timeout
      }

      c.io.mem_debug_read_address.poke(4.U)
      c.clock.step()
      c.io.mem_debug_read_data.expect(2.U)

      c.io.mem_debug_read_address.poke(8.U)
      c.clock.step()
      c.io.mem_debug_read_data.expect(61.U)

    }
  }
}
```
* Run the command sbt "testOnly riscv.singlecycle.clz" to test clz.
```
$ sbt "testOnly riscv.singlecycle.clz"
[info] welcome to sbt 1.9.7 (Oracle Corporation Java 1.8.0_391)
[info] loading settings for project ca2023-lab3-build from plugins.sbt ...
[info] loading project definition from /home/terry/ca2023-lab3/project
[info] loading settings for project root from build.sbt ...
[info] set current project to mycpu (in build file:/home/terry/ca2023-lab3/)
[info] clz:
[info] Single Cycle CPU
[info] - should countLeadingZeros
[info] Run completed in 39 seconds, 446 milliseconds.
[info] Total number of tests run: 1
[info] Suites: completed 1, aborted 0
[info] Tests: succeeded 1, failed 0, canceled 0, ignored 0, pending 0
[info] All tests passed.
[success] Total time: 58 s, completed Dec , 2023 10:41:51 PM
```
