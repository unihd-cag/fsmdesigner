/**
 reg fsm_axi_sof;
reg fsm_axi_header_has_data;
reg fsm_axi_eof;
reg fsm_axi_frame_valid;
reg fsm_axi_ready;
reg fsm_axi_single_cycle;
reg fsm_no_last_data;
reg fsm_axi_header_complete;

wire fsm_write_header;
wire fsm_write_data;
wire fsm_dummy;
wire fsm_dummy1;

rx rx_I (

    .clk(), 
    .res_n(), 
	//-- Inputs
	.axi_sof(fsm_axi_sof), 
	.axi_header_has_data(fsm_axi_header_has_data), 
	.axi_eof(fsm_axi_eof), 
	.axi_frame_valid(fsm_axi_frame_valid), 
	.axi_ready(fsm_axi_ready), 
	.axi_single_cycle(fsm_axi_single_cycle), 
	.no_last_data(fsm_no_last_data), 
	.axi_header_complete(fsm_axi_header_complete), 

	//-- Outputs
	.write_header(fsm_write_header), 
	.write_data(fsm_write_data), 
	.dummy(fsm_dummy), 
	.dummy1(fsm_dummy1)
);

 */
module rx ( 
    input wire clk, 
    input wire res_n, 

    // Inputs
    //------------ 
    input wire axi_sof, 
    input wire axi_header_has_data, 
    input wire axi_eof, 
    input wire axi_frame_valid, 
    input wire axi_ready, 
    input wire axi_single_cycle, 
    input wire no_last_data, 
    input wire axi_header_complete, 

    // Outputs
    //------------ 
    output wire write_header, 
    output wire write_data, 
    output wire dummy, 
    output wire dummy1
 );

localparam Idle = 4'b0000;
localparam Write_Header_Data = 4'b0000;
localparam Write_Last_data = 4'b0101;
localparam Write_Header = 4'b1000;
localparam Write_Data = 4'b0100;
localparam Write_Last_Data_And_Header = 4'b1100;

reg [3:0] current_state, next_state;
assign {write_header, write_data, dummy, dummy1} = current_state;

wire [7:0] inputvector;
assign inputvector = {axi_sof, axi_header_has_data, axi_eof, axi_frame_valid, axi_ready, axi_single_cycle, no_last_data, axi_header_complete};


always @(*) begin
  casex({inputvector, current_state})
    {8'bxxx0xxxx, Idle}:   next_state = Idle;
    {8'b0xxxxxxx, Idle}:   next_state = Idle;
    {8'b1x11xxxx, Idle}:   next_state = Write_Header_Data;
    {8'b1x01xxx1, Idle}:   next_state = Write_Header_Data;
    {8'b1x01xxx0, Idle}:   next_state = Write_Header;
    {8'b0xxx1x10, Write_Header_Data}:   next_state = Idle;
    {8'b0xxx11x0, Write_Header_Data}:   next_state = Idle;
    {8'bxxx01xx0, Write_Header_Data}:   next_state = Idle;
    {8'b1x1111xx, Write_Header_Data}:   next_state = Write_Header_Data;
    {8'bxxxx0xxx, Write_Header_Data}:   next_state = Write_Header_Data;
    {8'bxxxxxxx1, Write_Header_Data}:   next_state = Write_Header_Data;
    {8'b1x011xx0, Write_Header_Data}:   next_state = Write_Header;
    {8'b1x1110x0, Write_Header_Data}:   next_state = Write_Header;
    {8'b0x011000, Write_Header_Data}:   next_state = Write_Data;
    {8'b0x111000, Write_Header_Data}:   next_state = Write_Last_data;
    {8'bxx1xx1x0, Write_Last_data}:   next_state = Write_Last_data;
    {8'bxx1xx10x, Write_Last_data}:   next_state = Write_Last_data;
    {8'bxx1xxx10, Write_Last_data}:   next_state = Write_Last_data;
    {8'bx11xxx1x, Write_Last_data}:   next_state = Write_Last_data;
    {8'bx1x1xx10, Write_Last_data}:   next_state = Write_Last_data;
    {8'bx101xxx0, Write_Last_data}:   next_state = Write_Last_data;
    {8'b11xxxx10, Write_Last_data}:   next_state = Write_Last_data;
    {8'b110xxxx0, Write_Last_data}:   next_state = Write_Last_data;
    {8'bxxx0x1xx, Write_Last_data}:   next_state = Write_Last_data;
    {8'bx1xxx1xx, Write_Last_data}:   next_state = Write_Last_data;
    {8'b0xxxx1xx, Write_Last_data}:   next_state = Write_Last_data;
    {8'b0xx1xxxx, Write_Last_data}:   next_state = Write_Last_data;
    {8'bxx10xxxx, Write_Last_data}:   next_state = Write_Last_data;
    {8'b0x1xxxxx, Write_Last_data}:   next_state = Write_Last_data;
    {8'b1xx0xxxx, Write_Last_data}:   next_state = Write_Last_data;
    {8'bxxxx0xxx, Write_Last_data}:   next_state = Write_Last_data;
    {8'b0x0010xx, Write_Last_data}:   next_state = Idle;
    {8'b1x11100x, Write_Last_data}:   next_state = Write_Header_Data;
    {8'b110110x1, Write_Last_data}:   next_state = Write_Header_Data;
    {8'b10011xxx, Write_Last_data}:   next_state = Write_Header;
    {8'b10111x11, Write_Last_data}:   next_state = Write_Header;
    {8'bx1010xx0, Write_Header}:   next_state = Write_Data;
    {8'b1x010xx0, Write_Header}:   next_state = Write_Data;
    {8'b0101xxxx, Write_Header}:   next_state = Write_Data;
    {8'b10x10xxx, Write_Header}:   next_state = Write_Data;
    {8'b1111xxx0, Write_Header}:   next_state = Write_Last_Data_And_Header;
    {8'b0111xxxx, Write_Header}:   next_state = Write_Last_data;
    {8'bxxx0xxxx, Write_Header}:   next_state = Write_Header;
    {8'b10x11xxx, Write_Header}:   next_state = Write_Header;
    {8'b11011xx0, Write_Header}:   next_state = Write_Header;
    {8'b00x1xxxx, Write_Header}:   next_state = Idle;
    {8'b1101xxx1, Write_Header}:   next_state = Write_Header_Data;
    {8'b1111xxx1, Write_Header}:   next_state = Write_Header_Data;
    {8'bxxxxx1xx, Write_Data}:   next_state = Write_Data;
    {8'bxxx0xxxx, Write_Data}:   next_state = Write_Data;
    {8'bxx0xxxxx, Write_Data}:   next_state = Write_Data;
    {8'b0x11x00x, Write_Data}:   next_state = Write_Last_data;
    {8'b1x11x00x, Write_Data}:   next_state = Write_Last_Data_And_Header;
    {8'b1x11x01x, Write_Data}:   next_state = Write_Header;
    {8'b0x11x01x, Write_Data}:   next_state = Idle;
    {8'bxxxx0xxx, Write_Last_Data_And_Header}:   next_state = Write_Last_Data_And_Header;
    {8'bxxx0xxxx, Write_Last_Data_And_Header}:   next_state = Write_Last_Data_And_Header;
    {8'b1xxxxxxx, Write_Last_Data_And_Header}:   next_state = Write_Last_Data_And_Header;
    {8'b0x111xxx, Write_Last_Data_And_Header}:   next_state = Write_Last_data;
    {8'b0x011xxx, Write_Last_Data_And_Header}:   next_state = Write_Data;
    default:  next_state = Idle;
  endcase
end

`ifdef ASYNC_RES
 always @(posedge clk or negedge res_n ) begin
`else
 always @(posedge clk) begin
`endif
    if (!res_n)
    begin
        current_state <= Idle;
    end
    else begin
        current_state <= next_state;
    end
end

`ifdef CAG_COVERAGE
// synopsys translate_off

	// State coverage
	//--------

	//-- Coverage group declaration
	covergroup cg_states @(posedge clk);
		states : coverpoint current_state {
			bins Idle = {Idle};
			bins Write_Header_Data = {Write_Header_Data};
			bins Write_Last_data = {Write_Last_data};
			bins Write_Header = {Write_Header};
			bins Write_Data = {Write_Data};
			bins Write_Last_Data_And_Header = {Write_Last_Data_And_Header};
		}
	endgroup : cg_states

	//-- Coverage group instanciation
	cg_states state_cov_I;
	initial begin
		state_cov_I = new();
		state_cov_I.set_inst_name("state_cov_I");
	end

	// Transitions coverage
	//--------

	tc_One_cycle_packet_or_header_is_already_complete_c0: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x11xxxx) &&(current_state == Idle)|=> (current_state == Write_Header_Data));

	tc_One_cycle_packet_or_header_is_already_complete_c1: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x01xxx1) &&(current_state == Idle)|=> (current_state == Write_Header_Data));

	tc_trans_12_default: cover property( @(posedge clk) disable iff (!res_n)if (current_state == Idle))|=> (current_state == Idle) );

	tc_Normal_packet_sof: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x01xxx0) &&(current_state == Idle)|=> (current_state == Write_Header));

	tc_eop_default: cover property( @(posedge clk) disable iff (!res_n)if (current_state == Write_Header_Data))|=> (current_state == Idle) );

	tc_Another_one_cycle_packet_already_c0: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x1111xx) &&(current_state == Write_Header_Data)|=> (current_state == Write_Header_Data));

	tc_Another_one_cycle_packet_already_c1: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'bxxxx0xxx) &&(current_state == Write_Header_Data)|=> (current_state == Write_Header_Data));

	tc_Another_one_cycle_packet_already_c2: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'bxxxxxxx1) &&(current_state == Write_Header_Data)|=> (current_state == Write_Header_Data));

	tc_already_normal_packet_c0: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x011xx0) &&(current_state == Write_Header_Data)|=> (current_state == Write_Header));

	tc_already_normal_packet_c1: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x1110x0) &&(current_state == Write_Header_Data)|=> (current_state == Write_Header));

	tc_trans_40: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b0x011000) &&(current_state == Write_Header_Data)|=> (current_state == Write_Data));

	tc_trans_41: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b0x111000) &&(current_state == Write_Header_Data)|=> (current_state == Write_Last_data));

	tc_trans_22: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b0x0010xx) &&(current_state == Write_Last_data)|=> (current_state == Idle));

	tc_trans_23_c0: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x11100x) &&(current_state == Write_Last_data)|=> (current_state == Write_Header_Data));

	tc_trans_23_c1: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b110110x1) &&(current_state == Write_Last_data)|=> (current_state == Write_Header_Data));

	tc_trans_24_c0: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b10011xxx) &&(current_state == Write_Last_data)|=> (current_state == Write_Header));

	tc_trans_24_c1_SOFEOF0DWNP: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b10111x11) &&(current_state == Write_Last_data)|=> (current_state == Write_Header));

	tc_not_ready_default: cover property( @(posedge clk) disable iff (!res_n)if (current_state == Write_Last_data))|=> (current_state == Write_Last_data) );

	tc_2_cycles_eofsof: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1111xxx0) &&(current_state == Write_Header)|=> (current_state == Write_Last_Data_And_Header));

	tc_data_default: cover property( @(posedge clk) disable iff (!res_n)if (current_state == Write_Header))|=> (current_state == Write_Data) );

	tc_2_cycles_eof: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b0111xxxx) &&(current_state == Write_Header)|=> (current_state == Write_Last_data));

	tc_stall_c0_Stall: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'bxxx0xxxx) &&(current_state == Write_Header)|=> (current_state == Write_Header));

	tc_stall_c1_No_data_Another_Sof: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b10x11xxx) &&(current_state == Write_Header)|=> (current_state == Write_Header));

	tc_stall_c2_Another_SOF__Data__header_not_complete: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b11011xx0) &&(current_state == Write_Header)|=> (current_state == Write_Header));

	tc_trans_48_No_data: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b00x1xxxx) &&(current_state == Write_Header)|=> (current_state == Idle));

	tc_sof_header_complete_data_c0_SOFHeadercompleteData: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1101xxx1) &&(current_state == Write_Header)|=> (current_state == Write_Header_Data));

	tc_sof_header_complete_data_c1_SOFEOFHeaderCompleteD: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1111xxx1) &&(current_state == Write_Header)|=> (current_state == Write_Header_Data));

	tc_end_eof: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b0x11x00x) &&(current_state == Write_Data)|=> (current_state == Write_Last_data));

	tc_end_and_start_eofsof: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x11x00x) &&(current_state == Write_Data)|=> (current_state == Write_Last_Data_And_Header));

	tc_Just_write__stall_default: cover property( @(posedge clk) disable iff (!res_n)if (current_state == Write_Data))|=> (current_state == Write_Data) );

	tc_eofsofnolast: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b1x11x01x) &&(current_state == Write_Data)|=> (current_state == Write_Header));

	tc_trans_39: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b0x11x01x) &&(current_state == Write_Data)|=> (current_state == Idle));

	tc_there_was_sof_but_already_eof_single_cycle: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b0x111xxx) &&(current_state == Write_Last_Data_And_Header)|=> (current_state == Write_Last_data));

	tc_keep_on: cover property( @(posedge clk) disable iff (!res_n)(inputvector ==? 8'b0x011xxx) &&(current_state == Write_Last_Data_And_Header)|=> (current_state == Write_Data));

	tc_trans_42_default: cover property( @(posedge clk) disable iff (!res_n)if (current_state == Write_Last_Data_And_Header))|=> (current_state == Write_Last_Data_And_Header) );

// synopsys translate_on
`endif


endmodule
