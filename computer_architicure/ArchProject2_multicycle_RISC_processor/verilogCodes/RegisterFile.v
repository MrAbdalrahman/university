module RegisterFile (
    input clk,
    input branch,
    input m,
    input [2:0] Rd,         // Rd address
    input [2:0] Rs1,        // Rs1 address
    input [2:0] Rs2,        // Rs2 address
    input [15:0] WriteData, // data to write back
    input [2:0] WriteReg,   // register to write back to
    input RegWrite,          // write enable
    input store,
    output reg [15:0] BusA, // Rs1 data
    output reg [15:0] BusB // Rs2 data
    
);

    // 8 registers, each 16 bits wide
    reg [15:0] registers [0:7];

     // read operations
    always @(*) begin
        if (branch) begin
            BusA = registers[Rd];
            if (m == 1'b0) begin
                BusB = registers[Rs1];
            end else begin
                BusB = 16'b0; // busB is zero if m is 1
            end
        end else begin
            BusA = registers[Rs1];
            if (store) begin
                BusB = registers[Rd];
            end else begin
                BusB = registers[Rs2];
            end
        end
    end

    // write operation
    always @(posedge clk) begin
        if (RegWrite == 1 && WriteReg != 3'b000) begin // any write attempt on R0 will be discarded
            registers[WriteReg] <= WriteData;
        end
    end

    // initialize registers
    initial begin
        registers[0] = 16'b0; // R0 is hardwired to 0
    end
endmodule
