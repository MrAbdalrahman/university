
`timescale 1ns / 1ps // sets the base time unit to 1 nanosecond and the precision to 1 picosecond.


module fullAdder (A,B,cin,sum,cout);

    input A,B,cin; // inputs number 1 2 and the carry in  
    output sum,cout; // outputs the summation and the carry out

    wire w1, w2, w3; // wires to connect gates


    // calculating summation
    xor #(7) g0 (w1, A, B); // w1 = A ^ B 
    xor #(7) g1 (sum, w1, cin); // sum = w1 ^ cin  = (A ^ B) ^ Cin

    // calculating carry out
    and #(4) g2 (w2, A, B);  // w2 = w2 = A & B
    and #(4) g3 (w3, w1, cin); // w3 = w1 & Cin = (A ^ B) & Cin 
    or  #(4) g4 (cout, w2, w3); // cout = w2 | w3 = (A & B) | ((A ^ B) & Cin)

endmodule
