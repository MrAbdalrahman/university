module LoadExtender(
input [15:0] loaded_data,
output [15:0] U_Byte,
output [15:0] S_Byte
);

    wire [7:0] Byte;
    assign Byte = loaded_data[7:0];
    assign U_Byte =  {8'b0, Byte};
    assign S_Byte =  $signed({{8{Byte[7]}}, Byte});
endmodule