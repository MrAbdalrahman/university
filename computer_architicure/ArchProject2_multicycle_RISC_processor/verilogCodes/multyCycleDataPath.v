`include "ALU.v"
`include "controlUnit.v"
`include "DataMemory.v"
`include "InstructionDecoder.v"
`include "InstructionMemory.v"
`include "PC.v"
`include "RegisterFile.v"
`include "PcControl.v"
`include "Extender.v"
`include "LoadExtender.v"
`include "miniPcControl.v"

module MultiCycleDataPath(
input clk,reset
);


// ----------- signals -----------


    // program counter signals
    wire [15:0] pc_in;
    wire [15:0] pc_out;
    reg [15:0] pc_out_reg;


    // instruction memory signals
    wire [15:0] instruction;

    // IF_ID signals
    reg [15:0] INSTR_OUT_IF_ID;
    reg [15:0] IF_ID_pc_out;




    // control unit and instruction decoder signals and some register file signals
    wire [3:0] Op ; // op code 
    wire [2:0] Rs1, Rs2, Rd; // register sources and destenations idS
    wire [4:0] Imm; // immideate
    wire [15:0] EXT_Imm;
    wire [15:0] SImm;
    wire [11:0] JumpOffset; // jump ammount for jump instructions
    wire m; // mode bit
    wire  ExtOp, MemWr, RegWr;
    wire  RegDst;
    wire [1:0] WBdata;
    wire ALUSrc;
    wire [2:0] ALUOp;
    wire [1:0] pc_src1;
    wire [1:0] pc_src2;
    wire [1:0] pc_src3;

    wire [1:0] func;
    wire branch;
    wire store;
    wire LType;
    wire sv;
    wire Memr;


// register file signals
    wire [15:0] busA, busB;
    wire [15:0] WriteData;
    wire [2:0] WriteReg;
    wire RegWrite;

     // ID_EX_controlUnite
    reg ID_EX_MemWr,ID_EX_RegWr,ID_EX_branch;
    reg [1:0] ID_EX_WBdata;
    reg [3:0] ID_EX_Op;
    reg [2:0] ID_EX_ALUOp;
    reg ID_EX_sv;
    // ID_EX_instructionDecoder


    reg [2:0] ID_EX_Rd;     // destination register
    reg [15:0] ID_EX_SImm;    // immediate value for S-Type
    reg ID_EX_m ;             // mode bit for I-Type instructions

    //ID_EX Register file

    reg [15:0] ID_EX_busB;
    reg [15:0] ID_EX_busA;
    wire [15:0] aluB;
    wire [15:0] U_Byte;
    wire [15:0] S_Byte;

    

    // ALU_signals
    wire [15:0] alu_result;
    wire zero, carry, overflow, Negative;

    //EX_MEM ALU

    reg [15:0] EX_MEM_address_in;
    reg EX_MEM_MemWr;
    

    // memory output
   wire [15:0] mem_data_out;

// MEM_WB MEM
    reg [15:0] MEM_WB_mem_data_out ;
    reg  EX_MEM_RegWr;
    reg [1:0] EX_MEM_WBdata;
    reg [2:0] EX_MEM_Rd;

    reg [15:0] MEM_WB_alu_result;
    reg [1:0] MEM_WB_WBdata;
    reg EX_MEM_Memr;
    reg ID_EX_Memr; 
    reg [15:0] EX_MEM_data_in;
    reg [15:0] ID_EX_EXT_EXT_Imm;
    reg ID_EX_LType;
    reg EX_MEM_LType;
    reg EX_MEM_m;
    reg [15:0] ID_EX_pc_out;
    reg [15:0] EX_MEM_pc_out;
    reg [15:0] MEM_WB_pc_out;


// --------- end of signals --------------





// ----------- stage1 : IF -------------

   // instantiate the register module as the program counter
    pc pc (
        .reset(reset),
        .clk(clk),
        .d(pc_in),
        .q(pc_out)
    );


    //instantiate the instruction memory
    InstructionMemory im (
        .addr(pc_out),        
        .instr(instruction)
    );

    

// ------------- end of stage 1 -----------




// -------------- stage2: ID ----------------------

   

    ControlUnit control_unit(
        // inputs
        .instruction(INSTR_OUT_IF_ID),
        // outputs
        .JR(JR),
        .LType(LType),
        .store(store),
        .sv(sv),
        .Op(Op),
        .func(func),
        .RegDst(RegDst),
        .ExtOp(ExtOp),
        .ALUSrc(ALUSrc),
        .ALUOp(ALUOp),
        .MemRd(Memr),
        .MemWr(MemWr),
        .WBdata(WBdata),
        .RegWr(RegWr),
        .branch(branch)

    );

    InstructionDecoder instruction_decoder(
        //inputs
        .instr(INSTR_OUT_IF_ID),
        .func(func),
        //outputs
        .Rd(Rd),
        .Rs1(Rs1),
        .Rs2(Rs2),
        .Imm(Imm),
        .SImm(SImm),
        .JumpOffset(JumpOffset),
        .m(m)
    );


    Extender extender(
        // inputs
        .ExtOp(ExtOp),
        .Imm_in(Imm),
        // outputs
        .Imm_out(EXT_Imm)
    );



    // instantiate the register file
    RegisterFile register_file(
        // inputs
        .store(store),
        .clk(clk),
        .branch(branch),
        .m(m),
        .Rd(Rd),
        .Rs1(Rs1),
        .Rs2(Rs2),
        .WriteData(WriteData),
        .WriteReg(WriteReg),
        .RegWrite(RegWrite),
        // outputs
        .BusA(busA),
        .BusB(busB)
    );

      miniPcControl mini_pc_control(
        // inputs
        .Op(Op),
        // outputs
        .PCSrc(pc_src2)
    );

// ------------------- end of stage 2 ------------------


// ------------------------------stage 3 EX -------------------

    // Instantiate the ALU
    ALU alu(
        // inputs
        .a(ID_EX_busA),
        .b(aluB),
        .ALUOp(ID_EX_ALUOp),
        // outputs
        .result(alu_result),
        .zero(zero),
        .carry(carry),
        .overflow(overflow),
        .Negative(Negative)
    );



    PcControl pc_control(
        // inputs
        .Op(ID_EX_Op),
        .Zero(zero),
        .Negative(Negative),
        .Carry(Carry),
        .Overflow(overflow),
        .branch(ID_EX_branch),
        .m(ID_EX_m),
        // outputs
        .PCSrc(pc_src1)
    );


assign pc_src3 = (JR)? pc_src2:pc_src1;

// next pc logic mux
    always @(*) begin
        case (pc_src3)
            2'b00: npc = pc_out  + 2; // next instruction
            2'b01: npc = {pc_out[15:12], (JumpOffset << 1)}; // instruction label to jumbed to
            2'b10: npc = BTA; // instruction label to be branched for 
            2'b11: npc = register_file.registers[7];  // instruction in R7
        endcase
    end

assign aluB = ALUSrc? ID_EX_EXT_EXT_Imm : ID_EX_busB;


// ------------------- end of stage 3 ------------------

// -------------------- stage 4 MEM ----------------------


    // instantiate the Data Memory
    DataMemory data_memory(
        // inputs
        .clk(clk),
        .Rdata(EX_MEM_Memr),
        .WBdata(EX_MEM_MemWr),
        .addr(EX_MEM_address_in),
        .Data_in(EX_MEM_data_in),
        // outputs
        .Data_out(mem_data_out)
    );


    LoadExtender load_extender(
        // inputs
        .loaded_data(mem_data_out),
        // outputs
        .U_Byte(U_Byte),
        .S_Byte(S_Byte)
    );

    

// ----------------- end of stage 4 ------------------------------ 

// ----------------- stage 5 WB ------------------------------------
    
    assign WriteData =  (WBdata == 2'b01)? MEM_WB_mem_data_out : (WBdata == 2'b00)? MEM_WB_alu_result : (MEM_WB_pc_out + 4);
    assign WriteReg = (RegDst)? 3'b111 : Rd; 
    assign RegWrite = (RegWr)? 1:0;
// ----------------------------- end of stage 5 ---------------------------


wire [15:0] BTA = EXT_Imm + pc_out;
reg [15:0] npc;

     // initialize pc at 0
    initial begin 
        pc_out_reg = 0;
    end

// control finite state machine for multicycle operation
//---------------------------------
    localparam FETCH      = 3'b000,
               DECODE     = 3'b001,
               EXECUTE    = 3'b010,
               MEM_ACCESS = 3'b011,
               WRITE_BACK = 3'b100;

    reg [2:0] state, next_state;

    always @(posedge clk or posedge reset) begin
        if (reset) 
            state <= FETCH;
        else 
            state <= next_state;
end
//---------------------------------



    
always @(*) begin
    case (state)

        FETCH: next_state = DECODE;


        DECODE: 
        
        if (Op ==4'b1100 || Op ==4'b1110)
        begin 
                next_state = FETCH;
                pc_out_reg = npc;
            end


        else
        next_state = EXECUTE;


        EXECUTE: begin
            if (Op == 4'b1000 || Op == 4'b1001 || Op == 4'b1010 || Op == 4'b1011) begin 
                next_state = FETCH;
                pc_out_reg = npc;
            end

            else
                next_state = MEM_ACCESS; 
        end

        MEM_ACCESS: begin
            if (Op == 4'b1111 || Op == 4'b0111) begin
                next_state = FETCH; 
                pc_out_reg = npc;
            end

            else begin
                next_state = WRITE_BACK; 
            end
        end

        WRITE_BACK: begin
            next_state = FETCH;
            pc_out_reg = npc;
        end

    endcase
end

assign pc_in = pc_out_reg;

    always @(posedge clk or posedge reset) begin
        if (reset) begin
        end else begin
            case (state)
                FETCH: begin
                    // fetch stage actions:
                    
                    INSTR_OUT_IF_ID = instruction;
                    IF_ID_pc_out = pc_out;

                end
                DECODE: begin
                    // decode stage actions:

                    ID_EX_busA = busA;
                    ID_EX_busB = busB;
                    ID_EX_EXT_EXT_Imm = EXT_Imm;
                    ID_EX_ALUOp = ALUOp;
                    ID_EX_MemWr = MemWr;
                    ID_EX_Memr = Memr;
                    ID_EX_Op = Op;
                    ID_EX_branch= branch;
                    ID_EX_m = m;
                    ID_EX_RegWr = RegWr;
                    ID_EX_WBdata = WBdata;
                    ID_EX_Rd = Rd;
                    ID_EX_sv = sv;
                    ID_EX_SImm = SImm;
                    ID_EX_LType = LType;
                    ID_EX_pc_out = IF_ID_pc_out;

                end
                EXECUTE: begin
                    // execute stage actions:

                    EX_MEM_LType = ID_EX_LType;
                    EX_MEM_address_in = (ID_EX_sv)? ID_EX_busA : alu_result;
                    EX_MEM_Memr = ID_EX_Memr;
                    EX_MEM_MemWr = ID_EX_MemWr;
                    EX_MEM_data_in = (ID_EX_sv)? ID_EX_SImm : ID_EX_busB;
                    EX_MEM_RegWr = ID_EX_RegWr;
                    EX_MEM_WBdata = ID_EX_WBdata;
                    EX_MEM_Rd = ID_EX_Rd;
                    EX_MEM_m = ID_EX_m;
                    EX_MEM_pc_out = ID_EX_pc_out;

                    
                     
                end
                MEM_ACCESS: begin
                    // memory access stage actions: 
                    
                    MEM_WB_mem_data_out = (EX_MEM_LType == 1)? mem_data_out : ({EX_MEM_LType,EX_MEM_m} == 00)?  U_Byte: ({EX_MEM_LType,EX_MEM_m} == 01)? S_Byte:16'bx;
                    MEM_WB_alu_result = EX_MEM_address_in;
                    MEM_WB_WBdata = EX_MEM_WBdata;
                    MEM_WB_pc_out = EX_MEM_pc_out;

                end
                WRITE_BACK: begin
                end
                        // no specefied Write back data actions needed
            endcase
        end
    end



   






endmodule