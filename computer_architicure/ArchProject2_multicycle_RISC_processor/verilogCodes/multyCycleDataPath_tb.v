`include "multyCycleDataPath.v"

module multyCycleDataPath_tb();

reg clk ,reset;


MultiCycleDataPath test(
.clk(clk),
.reset(reset)
);



 // clock generation
    always begin
        #5 clk = ~clk;
    end




 initial begin
        // initialize clock 
        clk = 0;
        reset = 1;
        #10;
        reset = 0;


        // run tasks after delay
        #15; 
        #10000; // 10*(# clocks needed - 1)
         display_data_memory();
        display_register_file();        
        $finish;
        
    end



            wire [15:0] pc_in = test.pc_in;
           wire [15:0] pc_out = test.pc_out;
          wire [2:0] state = test.state;
         wire [15:0] instruction = test.instruction;
        wire [3:0] Op = test.Op;
       wire [2:0] Rs1 = test.Rs1;
      wire [2:0] Rs2 = test.Rs2;
     wire [2:0] Rd = test.Rd;
    wire [4:0] Imm = test.Imm;
   wire [15:0] busA = test.busA;
   wire [15:0] busB = test.busB;
    wire [15:0] alu_result = test.alu_result;
     wire zero = test.zero;
      wire overflow = test.overflow;
       wire Negative = test.Negative;
        wire [15:0] mem_data_out = test.mem_data_out;
         wire RegWrite = test.RegWrite;
          wire [1:0] pc_src3 = test.pc_src3;
           wire LType = test.LType;
            wire [15:0] SImm = test.SImm;




always @(posedge clk) begin

// display signals for verefacation
if (reset == 0)
        $display(
            "at time = %t, pc_in  = %b, pc_out= %b, state = %b, instruction = %b,Op = %b, Rs1 = %b, Rs2 = %b, Rd = %b, Imm = %b,busA = %b, busB = %b,ALU result = %b,Zero = %b,overflow = %b, Negative = %b,MemData = %b,RegWrite = %b,pc_src3 = %b,LType = %b,SImm = %b\n\n",
            $time,pc_in,pc_out,state,instruction, Op, Rs1, Rs2, Rd, Imm,busA, busB,alu_result,zero,overflow, Negative,mem_data_out,RegWrite,pc_src3,LType,SImm);
end


// --------------------------- helping tasks ---------------------

    // task to display the contents of the data memory
    task display_data_memory;
        integer i;
        begin
            $display("data memory:");
            for (i = 0; i < 16; i = i + 1) begin
                $display("|address %0d: %b|", i, test.data_memory.memory[i]);
            end
        end
    endtask

    // task to display the contents of the register file
    task display_register_file;
        integer i;
        begin
            $display("\nregister file:");
            for (i = 0; i < 8;  i= i + 1) begin
                $display("|register %0d: %b|", i, test.register_file.registers[i]);
            end
        end
    endtask

// ----------------- end of helping tasks ---------------------------------

endmodule


