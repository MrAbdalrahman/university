`include "mult2.v"

`timescale 1ns / 1ps // sets the base time unit to 1 nanosecond and the precision to 1 picosecond.

module multiplier4xNTest;

    parameter N =31;  // first we define N as a parameter to use it in the testbench
    localparam DELAY =   10*(N+1)   ;  // here we calculate delay based on N  which is the maximum latency

    // the clock cycle time is 2 * delay time 

    // during testing i found out that the best value to delay is 10n + 10 

    // here we cant say that the delay is constant 

    // also in this test bench you cant make N > 31 since urandom does not support

    reg clk; // a clock to use on the flipflops
    reg [3:0] AReg;  // A register as flip flops
    reg [N-1:0] BReg; // B registor as flipflops
    reg [3:0] A; // input A 
    reg [N-1:0] B; // input B
    wire [N+3:0] P; // outoput product
    reg [N+3:0] PReg; // output registor as flipflops
    reg [N+3:0] expected; // the expected output should be saved here


    // then i instantiate  parameterized multiplier
    multiplier4xN2 #(N) test (A,B,P);

    always begin
        #DELAY clk = ~clk;  // clock period is 10 time units
    end


    integer i; // integers as counter 
    integer errors; // integer to save how many errors occures

    initial begin // the initial statement to initialize data
        clk = 0; // initializing clock to be 0
        errors = 0;  // initialize error counter
 
        if (N < 2 ) begin // first check vaildity of N 
            $display("invalid number of bits: bits must be more than 1  "); // print the error 
            $finish; // and finish the simulation
        end


        // here i generated all posible test cases
        for (i = 0; i < 8*N; i = i + 1) begin
            AReg = $urandom % 16;
            BReg = $urandom % (1 << N);
            expected = AReg * BReg;  // calculate expected product
            
            // pass on clk posedge
            @(posedge clk);
            A <= AReg; // now  A get the values in the AReg
            B <= BReg; // and B get the values in the BReg
            

            // getting output on clock edge after delay
            //repeat(DELAY / 20)
            @(posedge clk);
            PReg <= P;

            @(posedge clk);
            if (PReg === expected)  // if the result is as expected print success message
                $display("test %0d: success: A = %b, B = %b, P = %b, expected = %b", i+1, AReg, BReg, PReg, expected);
             else begin  // else print error message 
                $display("test %0d: error: A = %b, B = %b, P = %b, expected = %b", i+1, AReg, BReg, PReg, expected);
                errors = errors + 1;  // increment error counter if error occured
             
        end
    end

        // finally print the number of errors
        $display("total number of errors: %0d", errors);

        // finish the simulation
        $finish;
    end

endmodule

// tested time on N = 16 : 1069547350000 (1s)