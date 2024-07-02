module InstructionDecoder(
    input [15:0] instr,       // the 16-bit instruction
    input [1:0] func,         // the function code to determine the type of instruction
    output reg [2:0] Rd,      // destination register
    output reg [2:0] Rs1,     // first source register
    output reg [2:0] Rs2,     // second source register
    output reg [4:0] Imm,     // immediate value for I-Type
    output reg [15:0] SImm,    // mmmediate value for S-Type
    output reg [11:0] JumpOffset, // jump offset
    output reg m              // mode bit for I-Type instructions
);

    // decode the instruction format based on the `func` signal
    always @(*) begin
        // default values 
        Rd = 3'b0;
        Rs1 = 3'b0;
        Rs2 = 3'b0;
        Imm = 5'b0;
        SImm = 16'b0;
        JumpOffset = 12'b0;
        m = 1'b0;

        case (func)
            2'b00: begin
                // R-Type
                 Rd = instr[11:9];
                Rs1 = instr[8:6];
                Rs2 = instr[5:3];
            end
            2'b01: begin
                // I-Type
                m = instr[11]; // mode bit
                Rd = instr[10:8];
                Rs1 = instr[7:5];
                Imm = instr[4:0];   
            end
            2'b10: begin
                // J-Type
                JumpOffset = instr[11:0];
            end
            2'b11: begin
                // S-Type
                Rs1 = instr[11:9];
                SImm =  $signed({{8{instr[8]}}, instr[8:1]});
            end
        endcase
    end
endmodule
