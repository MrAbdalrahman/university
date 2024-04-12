module jcb(Q,input clr,clk);
	
	parameter n = 100; 		  
	
	output reg [n-1:0] Q ;
	reg temp;
	
	always @ (posedge clk or negedge clr) begin
		if (! clr) 
			Q <=0;
		
		
		else begin 				   
			
		temp = ~Q[n-1]; // here i took the MSB and i inverted it  Q = 0001 --> temp = 1
		Q <= Q << 1; // then i shifter all bits to the left -->	  Q = 0010
		Q[0] <= temp; // then i placed the inverted MSB into the LSB --> Q = 0011  
		
		end
			
	
	end
	
endmodule	   

module jcb_T();
	
	parameter n = 4;
	
	reg clrT,clkT;
	wire [n-1:0]QT;
	
	jcb #(.n(n)) Test(.clk(clkT),.clr(clrT),.Q(QT));
	
	initial begin 
		
	clrT = 0;
	#10 clrT = 1;
	
	
	
	$display("clock clear Q");
	$monitor("%b    %b    %b",clkT,clrT,QT);
	
	clkT = 0;
	
	repeat(30)
		#5 clkT = ~clkT;
	
	#100
	
	clrT = 0;
	#10 clrT = 1;
	
	#100
		
	$finish;
	
	end
endmodule
	