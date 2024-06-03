`include "1bfa.v"

`timescale 1ns / 1ps // sets the base time unit to 1 nanosecond and the precision to 1 picosecond.


module rippleAdder4 (A,B,cin,sum,cout);

    input [3:0] A,B; // 2 packed 4-bits inputs
    input cin; // carry in input
    output [3:0] sum; // summation output
    output cout; // carry out output

    wire c1, c2, c3; // wires to save carries between the full adders

    fullAdder fa0 (.A(A[0]), .B(B[0]), .cin(cin), .sum(sum[0]), .cout(c1));  // first bit adder
    fullAdder fa1 (.A(A[1]), .B(B[1]), .cin(c1), .sum(sum[1]), .cout(c2));   // first bit adder
    fullAdder fa2 (.A(A[2]), .B(B[2]), .cin(c2), .sum(sum[2]), .cout(c3));   // first bit adder
    fullAdder fa3 (.A(A[3]), .B(B[3]), .cin(c3), .sum(sum[3]), .cout(cout)); // first bit adder

endmodule
