module InstructionMemory (
    input [15:0] addr, // address from pc
    output  [15:0] instr // instruction out
);

    reg [15:0] memory [0:255];

    // initialize memory with a program
    initial begin
                memory[0] = 16'b0110000100000000; 
            memory[1] = 16'b0110101000000001; 
            memory[2] = 16'b0011010100000010; 
            memory[3] = 16'b0010011001010000; 
            memory[4] = 16'b1000101100000110; 
            memory[5] = 16'b1001101100001000; 
            memory[6] = 16'b1010101100001010; 
            memory[7] = 16'b1111101000000010; 
            memory[8] = 16'b1100000000011111; 
            memory[9] = 16'b1111101111111110; 
            memory[10] = 16'b1100000000011111; 
            memory[11] = 16'b1111101000000000;
    end


    // read instruction at the given address
     assign instr = memory[addr[15:1]];

endmodule