module CPU (clk,write,read,data_in,IR,MBR,AC,PC,MAR,Carry,Zero,Overflow,Negative,state);

	// inputs and outputs
	reg [31:0] temp;
	input clk; // a clock
	input  [15:0] data_in; // data to be saved 
	output reg write; // write 
	output reg read; // read 

	// registers
	output reg signed [15:0] MBR,AC; // 3 registers wich holds the same size as the cell size in the memory
	output reg [15:0] IR;
	output reg [10:0] PC,MAR; // 2 registers wich holds the same size as the address of the memory cell
	output reg [4:0] state; // state of the program (where did the cpu reched in the operations of instruction exexution ) from adress passing to data saving
	output reg  Carry, Zero, Overflow, Negative;

	//parameters
	parameter  load = 4'b0001 ,store = 4'b0010, add = 4'b0011,sub = 4'b0100 ,mul = 4'b0101,div = 4'b0110, Branch = 4'b0111,BRZ = 4'b1000; // some constants to make the program readable

	initial begin 
		// setting flags to zero
		Zero = 1'b1;
		Carry = 1'b0;
		Overflow = 1'b0;
		Negative = 1'b0;

		// setting the program counter on adress 10 to start fetching instructions from that adress
		PC = 0;

		// setting the state of the cpu to 00 to fetch instruction
		state = 0;

		end

	always @ (posedge clk ) begin 
		case (state) // now cpu decide what to do depending on the state value

			0: begin 
				MAR = PC; // sending the adress in the pc to the MAR to fetch it 
				state = 1; // setting state to the next state whic is fetching Instruction
			end
			1: begin 
				read = 1;
				 // the instruction is fetched from the a cell address
				PC = PC + 1; // incrementing pc to move to the next instruction 
				state = 2 ;  // setting state to the next state which is decoding the instruction
			end 
				2:
					state = 3 ;
				3:
					state = 4 ;
				4:
					state = 5 ;
				5:
					state = 6 ;	
				6:begin
					state = 7 ;
					IR = data_in;
					read = 0;
					end
			7: begin 
				MAR = IR[10:0]; // sending instruction to the ir without the opcode to decode it (opcode is sent to control unite)
				state = 8; // setting state to the next state which is fetching operand
			end
			8: begin // now lets fetch the operend
				case (IR[15:12]) // case depends on the opcode

					add,load,sub,mul,div : begin 
					if (IR[11]) begin 
						 // if the opcode is arithmatic or load the data of the given adress will be fitched to the MBR
						read = 1;
					end
					else 
					MBR = IR[10:0];
					end
					store : MBR = AC; // else if the opcode is store then store AC data in MBR

					BRZ : begin 
					if (Zero)
						PC = MAR;
					end 
					Branch : 
					PC = MAR;

										// else if the opcode is jump then the PC will be changed into the given adress
				endcase
				state = 9; // setting state to the next state which is execution (add in our computer)
			end	
			9:
			state = 10;
			10:state = 11;
			11:state=12;
			12:state=13;
			13:begin
		if (read)
		MBR = data_in;
			state=14;	
		end
				14:begin
			state = 15;
			read=0;
			end

			15: begin // now its time for execution
				if (IR[15:12] == add) begin // if opcode is add , add data in the mbr to ac
					{Carry,AC} =  AC + MBR; 
					state = 0;
					Overflow = (Carry != AC[15]);
				end
				else if (IR[15:12] == sub) begin 
					AC = AC - MBR;
					state = 0 ;
				end
					else if (IR[15:12] == mul) begin 
					temp = AC * MBR;
					AC = temp[15:0];
					state = 0 ;
				Overflow =	(temp > 32767 || temp < -32768);	
				end
					else if (IR[15:12] == div) begin
				if (MBR != 0) 
					AC = AC / MBR;
				else AC = 16'd404;
					state = 0 ;
				end

				else if (IR[15:12] == load) begin // else if opcode is load, load the data to the AC
					AC = MBR ; 
					state = 0;
				end
				else if (IR[15:12] == store ) begin // else if the opcode is store, store the ac data on the memory					
				write = 1;
				state = 16;
				end
				else if (IR [15:12] == Branch || IR [15:12] == BRZ ) begin  // else if the opcode is jump, store the ac data on the memory, just reset the state
					state = 0 ;
				end
				Negative = AC[15];
				Zero = (AC == 16'h0000);
			end
			16: 
			state = 17;
			17:state = 18;
			18: state = 19;
			19:state = 20;
			20 : begin
				state = 0;
				write = 0;
		end
		endcase
	end
endmodule 


// | (4-bit) opcode | (12-bit) address| // the address mode here is direct addressing