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



