`include "claa.v"

`timescale 1ns / 1ps // sets the base time unit to 1 nanosecond and the precision to 1 picosecond.


module multiplier4xN2 (A,B,P); 

    parameter N = 4; // parameter needed for generic multiplier

    input [3:0] A;       // 4-bit multiplicand 
    input [N-1:0] B;     // N-bit multiplier
    output [N+3:0] P;     // N+4-bit product

    // all are packed

    wire [3:0] anded [N-1:0];  //  anded inputs togother 
    wire [3:0] sum [N-2:0]; // summations of each ripple adder
    wire [N-2:0] c;         // carry outs of each ripple adder

    // here we had packed and unpacked

    genvar i; // a variable to use in the generate statement

    // this generate statement is used to and A with B in a generic loop 
    generate
        for (i = 0; i < N; i = i + 1) begin : andInput // runs N-1 times
        // in each run it ands A with all B[i] 
            and #(4) (anded[i][0], A[0], B[i]);
            and #(4) (anded[i][1], A[1], B[i]);
            and #(4) (anded[i][2], A[2], B[i]);
            and #(4) (anded[i][3], A[3], B[i]);
        end
    endgenerate

    // a generate statement to pass the data into the ripple adders
    generate
        for (i = 0; i < N-1; i = i + 1) begin : adder // runs N-2 times
            if (i == 0) // this case is only for the first adder
                lookAheadAdder4 adder0 (anded[1], {1'b0, anded[0][3:1]}, 1'b0,sum[i], c[i]);
            else // this case is for the rest of the adders 
                lookAheadAdder4 adderi ({c[i-1], sum[i-1][3:1]},anded[i+1], 1'b0, sum[i], c[i]);
        end
    endgenerate

    // finally we need to assign the final product in a dynamic way

    genvar j; // a variable to use in the generate statement
    generate
        for (j = 0; j < N-1; j = j + 1) begin : finalProduct
            assign P[j+1] = sum[j][0]; // assin the result to have the first sum of each adder result exept the first one
        end
    endgenerate

    assign P[0] = anded[0][0]; // for the first bit its always the first bit of first anded
    assign P[N-1] = sum[N-2][0]; // for the (N-1) bit its always the 1st bit in the last sum 
    assign P[N] = sum[N-2][1]; // for the (N) bit its always the 2nd bit in the last sum 
    assign P[N+1] = sum[N-2][2]; // for the (N+1) bit its always the 3rd bit in the last sum 
    assign P[N+2] = sum[N-2][3]; // for the (N+2) bit its always the 4th bit in the last sum 
    assign P[N + 3] = c[N-2]; // for the last bit its always the last adders carry out result 

endmodule
