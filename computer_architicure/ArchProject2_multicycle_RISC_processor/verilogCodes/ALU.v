module ALU (
    input [15:0] a, // 1st input 
    input [15:0] b, // 2nd input
    input [2:0] ALUOp, // alu control signal
    output reg signed [15:0] result, // result of ALU
    output zero, // zero signal
    output Negative, // negative signal
    output reg carry, // carry signal 
    output reg overflow // overflow signal
   
);

    // define ALU control signals for the multiplixer
    localparam ALU_AND = 3'b000; // and code
    localparam ALU_ADD = 3'b001; // add code
    localparam ALU_SUB = 3'b010; // sub code

    always @(*) begin
        carry = 0;
        overflow = 0;
        case (ALUOp)
            ALU_AND: begin
                result = a & b;
            end
            ALU_ADD: begin
                {carry, result} = a + b;
                overflow = (a[15] == b[15]) && (result[15] != a[15]);
            end
            ALU_SUB: begin
                {carry, result} = a - b;
                overflow = (a[15] != b[15]) && (result[15] != a[15]);
            end
            default: begin
                result = 16'b0;
            end
        endcase
    end

    // zero flag
    assign zero = (result == 16'b0);

    // negative flag
    assign Negative = result[15];

endmodule
