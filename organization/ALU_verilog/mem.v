// read me !!! if you want to run for simulation 1 remove comments from simulation 1 if you want to run for simulation 2 remvoe comments from simulation 2
// read me !!! the memory should be 2^11 * 16/8 which is  2^12 which is 
// RAM of the computer
// (clock | dataIn | rwite | read | adress | dataOut )
module mem (clk,data_in,write,read,address,data_out);
	input clk;
	input [15:0] data_in; // data to be saved 
	input write; // write 
	input read; // read
	input [0:10] address; // address 
	output reg [15:0] data_out; // data to be extracted
	reg [15:0] ram [0:63]; // array of 2047 (2 ^ 11) registers each register holds 16 bits of data

	//for simulation 2 
	/*initial begin
		ram[20] = 16'd2; 
		ram[21] = 16'd3;
		ram[22] = 16'd5;
		ram[23] = 16'd8;
		ram[24] = -16'd5;
		ram[0] = 16'h1815; // load mem[21];
		ram[1] = 16'h5816; // mul mem[22];
		ram[2] = 16'h3814; // add mem[20];
		ram[3] = 16'h4005; // sub 5;
		ram[4] = 16'h2819; // store mem[25];
		ram[5] = 16'h1817; //load mem[23];
		ram[6] = 16'h3818; // add mem[24];
		ram[7] = 16'h3001; // add 1;
		ram[8] = 16'h281F; // store mem[32];
		ram[9] = 16'h1819; //load mem[25];
		ram[10] = 16'h681F;	//div mem[32];
		ram[11] = 16'h2819; // store mem[25];
	
	end*/

	//for simulation 1
	/*initial begin
		ram[0] = 16'h180A;
		ram[1] = 16'h580B;
		ram[2] = 16'h3005;
		ram[3] = 16'h280C;
		ram[10] = 16'h0009;
		ram[11] = 16'hFFFC;
		ram[12] = 16'h0000;
		ram[4] = 16'h0000;
	end*/


	always @ ( posedge clk ) begin
		if (write)
			ram[address] = data_in;
			else if (read)
		data_out =  ram[address];	
		end	
endmodule