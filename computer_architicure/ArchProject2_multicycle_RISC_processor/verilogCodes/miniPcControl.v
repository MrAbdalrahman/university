module miniPcControl (
    input [3:0] Op,
    output reg [1:0] PCSrc  // pc source control
    );

    always @(*) begin
        case (Op)
            4'b1100: begin // JMP
                PCSrc = 2'b01; // jump target address
            end
            4'b1110: begin // RET
                PCSrc = 2'b11; // return address
            end
            endcase
    end 
    endmodule