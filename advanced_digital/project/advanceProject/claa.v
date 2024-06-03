
`timescale 1ns / 1ps // sets the base time unit to 1 nanosecond and the precision to 1 picosecond.


module lookAheadAdder4 (A, B, cin, sum, cout);

    input [3:0] A, B; // inputs to summate
    input cin; // carry in 
    output [3:0] sum; // summation 
    output cout; // carry of summation

    wire [3:0] ands, xors, carries; // wires ands xors and carries between As and Bs
    wire w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12, w13, w14, w15, w16, w17, w18, w19, w20;

    // ands and xors
    and #(4) and0 (ands[0], A[0], B[0]); 
    and #(4) and1 (ands[1], A[1], B[1]);
    and #(4) and2 (ands[2], A[2], B[2]);
    and #(4) and3 (ands[3], A[3], B[3]); //g

    xor #(7) xor0 (xors[0], A[0], B[0]);
    xor #(7) xor1 (xors[1], A[1], B[1]);
    xor #(7) xor2 (xors[2], A[2], B[2]);
    xor #(7) xor3 (xors[3], A[3], B[3]); //p

    // carry assignments 
    assign carries[0] = cin;

    // 2nd carry assignment carries[1] = ands[0] + (xors[0] * carries[0])
    and #(4) g0 (w1, xors[0], cin);
    or  #(4) g1 (carries[1], ands[0], w1);

    // 3rd carry assignment carries[2] = ands[1] + (xors[1] * carries[1]) + (xors[1] * xors[0] * carries[0])
    and #(4) g2 (w2, xors[1], ands[0]);
    and #(4) g3 (w3, xors[1], w1);
    or  #(4) g4 (carries[2], ands[1], w2, w3);

    // 4th carry assignment carries[3] = ands[2] + (xors[2] * ands[1]) + (xors[2] * xors[1] * ands[0]) + (xors[2] * xors[1] * xors[0] * carries[0])
    and #(4) g5 (w4, xors[2], xors[1], ands[0]);
    and #(4) g6 (w5, xors[2], w3);
    and #(4) g7 (w6, xors[2], ands[1]);
    or  #(4) g8 (carries[3], ands[2], w6, w4, w5);

    // final carry-out assignment cout = ands[3] + (xors[3] * ands[2]) + (xors[3] * xors[2] * ands[1]) + (xors[3] * xors[2] * xors[1] * ands[0]) + (xors[3] * xors[2] * xors[1] * xors[0] * carries[0])
    and #(4) g9 (w7, xors[3], w4);
    and #(4) g10 (w8, xors[3], w5);
    and #(4) g11 (w9, xors[3], w6);
    and #(4) g12 (w10, xors[3], ands[2]);
    or  #(4) g13 (cout, ands[3], w10, w9, w7, w8);

    // sum calculation
    xor #(7) sumXor0 (sum[0], xors[0], carries[0]);
    xor #(7) sumXor1 (sum[1], xors[1], carries[1]);
    xor #(7) sumXor2 (sum[2], xors[2], carries[2]);
    xor #(7) sumXor3 (sum[3], xors[3], carries[3]);

endmodule
