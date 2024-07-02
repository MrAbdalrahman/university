module DataMemory (
    input clk,    // clock
    input WBdata, // write enable
    input [15:0] addr, // address
    input [15:0] Data_in, // data to write
    input Rdata,     // read enable
    output [15:0] Data_out // data read

);

    // memory array (256 16-bit words)
    reg [15:0] memory [0:255];

    // read operation
    assign Data_out = !Rdata? 16'b0 : memory[addr[15:0]];

    // write operation on clock
    always @(posedge clk) begin
        if (WBdata) begin
            memory[addr[15:0]] <= Data_in;
        end
    end

    initial begin

                  memory[0] = 16'b0000000001100100;
                memory[1] = 16'b0000000001100100;

    end
endmodule
