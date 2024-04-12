module dff(	input clr , clk , D,output reg Q);
	
	always @(posedge clk or negedge clr)  
    	
		if (~clr) 
        	Q <= 1'b0;
     	else 
        	Q <= D; 
			
endmodule	


module jcs(Q, input clr,clk); 	 
	
	parameter n = 100 ;
	output [n-1:0] Q;
	genvar i;			
	wire [n-1:0] Qs ;
	
	generate   
		for(i = 0; i < n ; i = i + 1) begin			   
			
			if (i == 0 ) begin 
				dff first_dff(.clr(clr), .clk(clk), .D(~Qs[n-1]), .Q(Qs[0]));
			end
			
			else begin
				dff d(.clr(clr),.clk(clk),.D(Qs[i-1]),.Q(Qs[i]));
			end
		
		end
	
	endgenerate
	
	assign Q = Qs;
	
endmodule						   
	
	
module jcs_T();									
	
	parameter n = 4;
	reg clrT,clkT;
	wire [n-1:0] QT ;

	jcs #(.n(n)) Test(.clr(clrT),.clk(clkT),.Q(QT));

	initial begin
    	clrT = 0;
    	#10 clrT = 1; 
    
		$display("clear clock Q");
		$monitor("%b    %b    %b",clrT, clkT, QT);
	
		clkT = 0;
		
		repeat(30)
			#5 clkT = ~clkT;


		#100		 
	
    	clrT = 0;
    	#10 clrT = 1;
    
    	#100;
    
    	$finish;
	end
	
endmodule
