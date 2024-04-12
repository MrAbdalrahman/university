module decoder2to4(
  input A, B,
  output [3:0] O
);

  assign O[0] = ~A & ~B;
  assign O[1] = ~A & B;
  assign O[2] = A & ~B;
  assign O[3] = A & B;

endmodule


module mux4to1(
  input [3:0]O,
  input S0,S1,
  output reg out
);
  always@(*)
    begin
      if(~S0 & ~S1)out = O[0];
      else if(~S1 & S0)out = O[1];
      else if( S1 & ~S0)out = O[2];
      else if( S1 & S0)out = O[3];
    end
endmodule


module figure1(
  input A,B,C,D,
  output F
);
  wire [3:0]O;
  decoder2to4 dec(.A(A),.B(B), .O(O));
  mux4to1 mx(.O(O),.S0(D),.S1(C),.out(F)); 
endmodule


module figure1_beh(
    input A,B,C,D,
  output reg F
);
  always@(*)
    begin
      if( A == C & B == D) F = 1;
      else F = 0;
    end
endmodule