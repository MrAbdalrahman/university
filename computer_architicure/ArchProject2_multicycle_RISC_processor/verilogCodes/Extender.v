module Extender(
input [4:0] Imm_in,
input ExtOp,
output [15:0] Imm_out
);

    assign Imm_out = (ExtOp ? $signed({{11{Imm_in[4]}}, Imm_in}) : {11'b0, Imm_in});

endmodule

