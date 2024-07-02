module ControlUnit(
    input [15:0]instruction,
    output [3:0] Op,         // opcode from the instruction
    output reg [1:0] func,  // function code for R-type instructions
    output reg RegDst,      // register destination control
    output reg ExtOp,       // signal to determine the type of immediate extension
    output reg ALUSrc,      // ALU source (register or immediate)
    output reg [2:0] ALUOp, // ALU operation type
    output reg MemRd,       // memory read enable
    output reg MemWr,       // memory write enable
    output reg [1:0] WBdata,  // write-back data source (ALU or memory)
    output reg RegWr,       // register write enable
    output reg branch,
    output reg store,
    output reg sv,
    output reg LType,
    output reg JR
    
);

    // define opcode values for instructions
    localparam AND   = 4'b0000;
    localparam ADD   = 4'b0001;
    localparam SUB   = 4'b0010;
    localparam ADDI  = 4'b0011;
    localparam ANDI  = 4'b0100;
    localparam LW    = 4'b0101;
    localparam LB   = 4'b0110;
    localparam SW    = 4'b0111;
    localparam BGT   = 4'b1000;
    localparam BLT   = 4'b1001;
    localparam BEQ   = 4'b1010;
    localparam BNE   = 4'b1011;
    localparam JMP   = 4'b1100;
    localparam CALL  = 4'b1101;
    localparam RET   = 4'b1110;
    localparam SV    = 4'b1111;
    

   assign Op = instruction[15:12]; // op code

    always @(*) begin
        
        case (Op)
        
            AND: begin
        
                func = 2'b00; 
                RegDst = 0;
                ExtOp = 1'bx;
                ALUSrc = 0; 
                ALUOp = 3'b000;
                MemRd = 0;
                MemWr = 0;
                WBdata = 2'b00;
                RegWr = 1;
                branch = 0;
                store = 0;
                sv= 0;
                LType = 1'bx;
                JR = 0;
                
                
            end
            ADD: begin
                
                func = 2'b00; 
                RegDst = 0;
                ExtOp = 1'bx;
                ALUSrc = 0; 
                ALUOp = 3'b001; 
                MemRd = 0;
                MemWr = 0;
                WBdata = 2'b00;
                RegWr = 1; 
                branch = 0;
                store = 0;
                sv= 0;
                LType = 1'bx;
                JR = 0;

            end
            SUB: begin

                func = 2'b00; 
                RegDst = 0;
                ExtOp = 1'bx;
                ALUSrc = 0; 
                ALUOp = 3'b010; 
                MemRd = 0;
                MemWr = 0;
                WBdata = 2'b00;
                RegWr = 1; 
                branch = 0;
                store = 0;
                sv= 0;
                LType = 1'bx;
                JR = 0;


            end
            ADDI: begin

                func = 2'b01; 
                RegDst = 0;
                ExtOp = 1'b1; 
                ALUSrc = 1;  
                ALUOp = 3'b001; 
                MemRd = 0;
                MemWr = 0;
                WBdata = 2'b00;
                RegWr = 1;  
                branch = 0;
                store = 0;
                sv= 0;
                LType = 1'bx;
                JR = 0;


            end
            ANDI: begin

                func = 2'b01; 
                RegDst = 0;
                ExtOp = 1'b0; 
                ALUSrc = 1;  
                ALUOp = 3'b000; 
                MemRd = 0;
                MemWr = 0;
                WBdata = 2'b00;
                RegWr = 1;  
                branch = 0;
                store = 0;
                sv= 0;
                LType = 1'bx;
                JR = 0;

            end
            LW: begin

                func = 2'b01; 
                RegDst = 0;
                ExtOp = 1'b1; 
                ALUSrc = 1; 
                ALUOp = 3'b001; 
                MemRd = 1; 
                MemWr = 0;
                WBdata = 2'b01; 
                RegWr = 1; 
                branch = 0;
                store = 0;
                sv= 0;
                LType = 1'b1; 
                JR = 0;



            end
            LB: begin // load byte unsigned (zeros)

                func = 2'b01; 
                RegDst = 0;
                ExtOp = 1'b1; 
                ALUSrc = 1; 
                ALUOp = 3'b001; 
                MemRd = 1; 
                MemWr = 0;
                WBdata = 2'b01; 
                RegWr = 1;  
                branch = 0;
                store = 0;
                sv= 0;
                LType = 1'b0; 
                JR = 0;

            end
          

            SW: begin

                func = 2'b01; 
                RegDst = 1'bx;
                ExtOp = 1'b1;
                ALUSrc = 1; 
                ALUOp = 3'b001; 
                MemRd = 0; 
                MemWr = 1;
                WBdata = 2'bxx;
                RegWr = 0;  
                branch = 0;
                store = 1;
                sv= 0;
                LType = 1'bx; 
                JR = 0;

            end
            BGT: begin

                func = 2'b01; 
                RegDst = 1'bx;
                ExtOp = 1'b1;
                ALUSrc = 0; 
                ALUOp = 3'b010;
                MemRd = 0; 
                MemWr = 0;
                WBdata = 2'bxx;
                RegWr = 0;  
                branch = 1;
                store = 0;
                sv= 0;
                LType = 1'bx; 
                JR = 0;

            end
            
            BLT: begin
                func = 2'b01; 
                RegDst = 1'bx;
                ExtOp = 1'b1;
                ALUSrc = 0; 
                ALUOp = 3'b010;
                MemRd = 0; 
                MemWr = 0;
                WBdata = 2'bxx;
                RegWr = 0;  
                branch = 1;
                store = 0;
                sv= 0;
                LType = 1'bx; 
                JR = 0;
            end
            

            BEQ: begin
                func = 2'b01; 
                RegDst = 1'bx;
                ExtOp = 1'b1;
                ALUSrc = 0; 
                ALUOp = 3'b010;
                MemRd = 0; 
                MemWr = 0;
                WBdata = 2'bxx;
                RegWr = 0;  
                branch = 1;
                store = 0;
                sv= 0;
                LType = 1'bx; 
                JR = 0;
            end

            BNE: begin
                func = 2'b01; 
                RegDst = 1'bx;
                ExtOp = 1'b1;
                ALUSrc = 0; 
                ALUOp = 3'b010;
                MemRd = 0; 
                MemWr = 0;
                WBdata = 2'bxx;
                RegWr = 0;  
                branch = 1;
                store = 0;
                sv= 0;
                LType = 1'bx; 
                JR = 0;
            end
            
            JMP: begin

                func = 2'b10; 
                RegDst = 1'bx;
                ExtOp = 1'b0;
                ALUSrc = 1'bx; 
                ALUOp = 3'bxxx;
                MemRd = 0; 
                MemWr = 0;
                WBdata = 2'bxx;
                RegWr = 0;  
                branch = 0;
                store = 0;
                sv= 1'bx;
                LType = 1'bx; 
                JR = 1; 


            end
            CALL: begin

                func = 2'b10; 
                RegDst = 1'b1; 
                ExtOp = 1'b0;
                ALUSrc = 1'bx; 
                ALUOp = 3'bxxx;
                MemRd = 0; 
                MemWr = 0;
                WBdata = 2'b10; 
                RegWr = 1; 
                branch = 0;
                store = 0;
                sv= 1'bx;
                LType = 1'bx; 
                JR = 0;





            end
            RET: begin  


                func = 2'b10; 
                RegDst = 1'bx;
                ExtOp = 1'bx;
                ALUSrc = 1'bx; 
                ALUOp = 3'bxxx;
                MemRd = 0; 
                MemWr = 0;
                WBdata = 2'bxx;
                RegWr = 0;  
                branch = 0;
                store = 0;
                sv= 1'bx;
                LType = 1'bx; 
                JR = 1; 

            end
            SV: begin

                func = 2'b11; 
                RegDst = 1'bx;
                ExtOp = 1'bx;
                ALUSrc = 1'bx; 
                ALUOp = 3'bxxx; 
                MemRd = 0; 
                MemWr = 1;
                WBdata = 2'bxx;
                RegWr = 0;  
                branch = 0;
                store = 0;
                sv= 1;
                LType = 1'bx; 
                JR = 0;

            end
        endcase
    end
endmodule
