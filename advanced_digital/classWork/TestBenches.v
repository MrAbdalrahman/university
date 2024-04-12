module compare();
  reg A, B, C, D, A1, B1, C1, D1;
  wire F, F_beh;
  
  figure1 fg(.A(A), .B(B), .C(C), .D(D), .F(F));
  figure1_beh fg1(.A(A1), .B(B1), .C(C1), .D(D1), .F(F_beh));
  
  initial begin
    A = 1'b0;
    B = 1'b0;
    C = 1'b0;
    D = 1'b0;
    
    A1 = 1'b0;
    B1 = 1'b0;
    C1 = 1'b0;
    D1 = 1'b0;
    
    $display("Test Successful");
    $finish;
  end
  
  always #80 begin
    D = ~D;
    D1 = ~D1;
  end
  always #40begin
    C = ~C;
    C1 = ~C1;
  end
  always #20 begin
    B = ~B;
    B1 = ~B1;
  end
  always #10 begin
    A = ~A;
    A1 = ~A1;
  end
  
  always @ (F or F_beh) begin
    if (F !== F_beh) begin
      $display("Test Failed");
      $finish;
    end
  end
endmodule



module figure_beh_tb ();
  
  reg A,B,C,D;
  wire F;
  
  figure1_beh fg1(.A(A), .B(B) ,.C(C) ,.D(D),.F(F));
  
  initial begin
	$monitor("A: %b , B: %b, C:%b, D:%b, F:%b",A,B,C,D,F);
   	
    #10
    A = 1'b0;
    B = 1'b1;
    C = 1'b0;
    D = 1'b1;
    
    #10
    A = ~A;
    
    #30
    B = ~B;
    
    #50
    C = ~C;
    
    #100
    D = ~D;
    

  end 
endmodule


module figure_tb ();
  
  reg A,B,C,D;
  wire F;
  
  figure1 fg(.A(A), .B(B) ,.C(C) ,.D(D),.F(F));
  
  initial begin
	$monitor("A: %b , B: %b, C:%b, D:%b, F:%b",A,B,C,D,F);
    
    #10
    A = 1'b0;
    B = 1'b1;
    C = 1'b0;
    D = 1'b1;
    
    #10
    A = ~A;
    
    #30
    B = ~B;
    
    #50
    C = ~C;
    
    #100
    D = ~D;
    

  end 
endmodule
