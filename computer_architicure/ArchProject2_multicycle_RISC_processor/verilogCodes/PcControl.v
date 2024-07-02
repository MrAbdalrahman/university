module PcControl (
    input [3:0] Op,         // Opcode
    input Zero,             // Zero flag from ALU
    input Negative,         // Negative flag from ALU
    input Carry,            // Carry flag from ALU
    input Overflow,         // Overflow flag from ALU
    input branch,           // Branch control signal
    input m,                // Mode bit for specific instructions
    output reg [1:0] PCSrc  // PC source control
);

    always @(*) begin
        case (Op)
            4'b1101: begin // CALL
                PCSrc = 2'b01; // Jump target address
            end
            4'b1000, 4'b1001, 4'b1010, 4'b1011: begin // Branch instructions
                if (branch && (
                        (Op == 4'b1000 && ((m == 1'b0 && Zero == 0 && (Negative == Overflow)) || 
                                           (m == 1'b1 && Zero == 0 && (Negative == Overflow)))) || // BGT, BGTZ

                        (Op == 4'b1001 && ((m == 1'b0 && (Negative != Overflow)) || 
                                           (m == 1'b1 && (Negative != Overflow)))) || // BLT, BLTZ

                        (Op == 4'b1010 && ((m == 1'b0 && Zero == 1) || (m == 1'b1 && Zero == 1))) || // BEQ, BEQZ

                        (Op == 4'b1011 && ((m == 1'b0 && Zero == 0) || (m == 1'b1 && Zero == 0)))    // BNE, BNEZ
                    )) begin
                    PCSrc = 2'b10; // Branch target address
                end else begin
                    PCSrc = 2'b00; // Next sequential instruction (PC + 2)
                end
            end
          default:   PCSrc = 2'b00; // Next sequential instruction (PC + 2)
        endcase
    end
endmodule