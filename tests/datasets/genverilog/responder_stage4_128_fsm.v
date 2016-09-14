/**
 reg fsm_noti_cond;
reg fsm_np_stop_pkts;
reg fsm_np_afull;
reg fsm_hdr_avail_mux;
reg fsm_hdr_get_mux;
reg fsm_hdr_noprocess_mux;
reg fsm_delayed_noprocess;
reg fsm_stored_nodata;
reg fsm_stored_qw;
reg fsm_data_end;
reg fsm_drain_end;
reg fsm_data_single_qw;
reg fsm_data_avail_mux;

wire fsm_hdr_take;
wire fsm_data_take;
wire fsm_state_lock;
wire fsm_state_get;
wire fsm_state_data;
wire fsm_state_drain;
wire fsm_mod_last_wait;

responder_stage4_128_fsm responder_stage4_128_fsm_I (

    .clk(), 
    .res_n(), 
	//-- Inputs
	.noti_cond(fsm_noti_cond), 
	.np_stop_pkts(fsm_np_stop_pkts), 
	.np_afull(fsm_np_afull), 
	.hdr_avail_mux(fsm_hdr_avail_mux), 
	.hdr_get_mux(fsm_hdr_get_mux), 
	.hdr_noprocess_mux(fsm_hdr_noprocess_mux), 
	.delayed_noprocess(fsm_delayed_noprocess), 
	.stored_nodata(fsm_stored_nodata), 
	.stored_qw(fsm_stored_qw), 
	.data_end(fsm_data_end), 
	.drain_end(fsm_drain_end), 
	.data_single_qw(fsm_data_single_qw), 
	.data_avail_mux(fsm_data_avail_mux), 

	//-- Outputs
	.hdr_take(fsm_hdr_take), 
	.data_take(fsm_data_take), 
	.state_lock(fsm_state_lock), 
	.state_get(fsm_state_get), 
	.state_data(fsm_state_data), 
	.state_drain(fsm_state_drain), 
	.mod_last_wait(fsm_mod_last_wait)
);

 */
module responder_stage4_128_fsm ( 
    input wire clk, 
    input wire res_n, 

    // Inputs
    //------------ 
    input wire noti_cond, 
    input wire np_stop_pkts, 
    input wire np_afull, 
    input wire hdr_avail_mux, 
    input wire hdr_get_mux, 
    input wire hdr_noprocess_mux, 
    input wire delayed_noprocess, 
    input wire stored_nodata, 
    input wire stored_qw, 
    input wire data_end, 
    input wire drain_end, 
    input wire data_single_qw, 
    input wire data_avail_mux, 

    // Outputs
    //------------ 
    output wire hdr_take, 
    output wire data_take, 
    output wire state_lock, 
    output wire state_get, 
    output wire state_data, 
    output wire state_drain, 
    output wire mod_last_wait
 );

localparam IDLE = 7'b0000000;
localparam LOCK1_NOPROC = 7'b1010000;
localparam LOCK1_PROC = 7'b1110000;
localparam GET1 = 7'b1001000;
localparam LOCK2 = 7'b1010001;
localparam GET2_DATA = 7'b1101001;
localparam GET2_NOPROC = 7'b1001001;
localparam DATA = 7'b0100100;
localparam DRAIN_DATA = 7'b0100010;
localparam DATA_WAIT = 7'b0000001;
localparam DATA_FLUSH = 7'b0000100;

reg [6:0] current_state, next_state;
assign {hdr_take, data_take, state_lock, state_get, state_data, state_drain, mod_last_wait} = current_state;

wire [12:0] inputvector;
assign inputvector = {noti_cond, np_stop_pkts, np_afull, hdr_avail_mux, hdr_get_mux, hdr_noprocess_mux, delayed_noprocess, stored_nodata, stored_qw, data_end, drain_end, data_single_qw, data_avail_mux};


always @(*) begin
  casex({inputvector, current_state})
    {13'b0x0xxxxxxxxx1, IDLE}:   next_state = IDLE;
    {13'b1x11x1xxxxxxx, IDLE}:   next_state = IDLE;
    {13'b10x0x1xxxxxxx, IDLE}:   next_state = IDLE;
    {13'b00x1x1xxxxxx0, IDLE}:   next_state = IDLE;
    {13'b0xx0x1xxxxxx0, IDLE}:   next_state = IDLE;
    {13'b11x0x1xxxxxx0, IDLE}:   next_state = IDLE;
    {13'bx110x1xxxxxx1, IDLE}:   next_state = IDLE;
    {13'b001xx1xxxxxx1, IDLE}:   next_state = IDLE;
    {13'bx101x1xxxxxx0, IDLE}:   next_state = IDLE;
    {13'b1100xxxxxxxx1, IDLE}:   next_state = IDLE;
    {13'bx01xx0xxxxxx1, IDLE}:   next_state = IDLE;
    {13'b11x1x0xxxxxx1, IDLE}:   next_state = IDLE;
    {13'b1000x0xxxxxx1, IDLE}:   next_state = IDLE;
    {13'b1101x1xxxxxx1, IDLE}:   next_state = IDLE;
    {13'b1110x0xxxxxx1, IDLE}:   next_state = IDLE;
    {13'b0111x1xxxxxxx, IDLE}:   next_state = IDLE;
    {13'b011xx0xxxxxx1, IDLE}:   next_state = IDLE;
    {13'bxxxxx0xxxxxx0, IDLE}:   next_state = IDLE;
    {13'b100101xxxxxxx, IDLE}:   next_state = LOCK1_NOPROC;
    {13'b100100xxxxxx1, IDLE}:   next_state = LOCK1_PROC;
    {13'b100110xxxxxx1, IDLE}:   next_state = GET1;
    {13'b100111xxxxxxx, IDLE}:   next_state = GET1;
    {13'bxxxxxxxxxxxxx, LOCK1_NOPROC}:   next_state = LOCK2;
    {13'bxxxxxxxxxxxxx, LOCK1_PROC}:   next_state = LOCK2;
    {13'bxxxxxx0xxxxxx, GET1}:   next_state = GET2_DATA;
    {13'bxxxxxx1xxxxxx, GET1}:   next_state = GET2_NOPROC;
    {13'b0x0xxxxxxxxx1, LOCK2}:   next_state = IDLE;
    {13'b1x11x1xxxxxxx, LOCK2}:   next_state = IDLE;
    {13'b10x0x1xxxxxxx, LOCK2}:   next_state = IDLE;
    {13'b00x1x1xxxxxx0, LOCK2}:   next_state = IDLE;
    {13'b0xx0x1xxxxxx0, LOCK2}:   next_state = IDLE;
    {13'b11x0x1xxxxxx0, LOCK2}:   next_state = IDLE;
    {13'bx110x1xxxxxx1, LOCK2}:   next_state = IDLE;
    {13'b001xx1xxxxxx1, LOCK2}:   next_state = IDLE;
    {13'bx101x1xxxxxx0, LOCK2}:   next_state = IDLE;
    {13'b1100xxxxxxxx1, LOCK2}:   next_state = IDLE;
    {13'bx01xx0xxxxxx1, LOCK2}:   next_state = IDLE;
    {13'b11x1x0xxxxxx1, LOCK2}:   next_state = IDLE;
    {13'b1000x0xxxxxx1, LOCK2}:   next_state = IDLE;
    {13'b1101x1xxxxxx1, LOCK2}:   next_state = IDLE;
    {13'b1110x0xxxxxx1, LOCK2}:   next_state = IDLE;
    {13'b0111x1xxxxxxx, LOCK2}:   next_state = IDLE;
    {13'b011xx0xxxxxx1, LOCK2}:   next_state = IDLE;
    {13'bxxxxx0xxxxxx0, LOCK2}:   next_state = IDLE;
    {13'b100100xxxxxx1, LOCK2}:   next_state = LOCK1_PROC;
    {13'b100101xxxxxxx, LOCK2}:   next_state = LOCK1_NOPROC;
    {13'b100110xxxxxx1, LOCK2}:   next_state = GET1;
    {13'b100111xxxxxxx, LOCK2}:   next_state = GET1;
    {13'bxx1xx1xxx1x1x, GET2_DATA}:   next_state = IDLE;
    {13'bxx11x0xxx1x11, GET2_DATA}:   next_state = IDLE;
    {13'bxxx0x0xxx1x11, GET2_DATA}:   next_state = IDLE;
    {13'b000xx1xxx1x10, GET2_DATA}:   next_state = IDLE;
    {13'b0x01xxxxx1x11, GET2_DATA}:   next_state = IDLE;
    {13'b1x00x1xxx1x10, GET2_DATA}:   next_state = IDLE;
    {13'bx000x1xxx1x11, GET2_DATA}:   next_state = IDLE;
    {13'bx101x1xxx1x10, GET2_DATA}:   next_state = IDLE;
    {13'b110xx1xxx1x11, GET2_DATA}:   next_state = IDLE;
    {13'b1101x0xxx1x11, GET2_DATA}:   next_state = IDLE;
    {13'b0100x1xxx1x1x, GET2_DATA}:   next_state = IDLE;
    {13'bxxxxx0xxx1x10, GET2_DATA}:   next_state = IDLE;
    {13'b100110xxx1x11, GET2_DATA}:   next_state = GET1;
    {13'b100111xxx1x1x, GET2_DATA}:   next_state = GET1;
    {13'b100100xxx1x11, GET2_DATA}:   next_state = LOCK1_PROC;
    {13'b100101xxx1x1x, GET2_DATA}:   next_state = LOCK1_NOPROC;
    {13'bxx0xxxxxx0xxx, GET2_DATA}:   next_state = DATA;
    {13'bxxxxxxxxx1x0x, GET2_DATA}:   next_state = DATA_FLUSH;
    {13'bxx1xxxxxx0xxx, GET2_DATA}:   next_state = DATA_WAIT;
    {13'b0x0xxxx1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'b1x11x1x1xxxxx, GET2_NOPROC}:   next_state = IDLE;
    {13'b10x0x1x1xxxxx, GET2_NOPROC}:   next_state = IDLE;
    {13'b00x1x1x1xxxx0, GET2_NOPROC}:   next_state = IDLE;
    {13'b0xx0x1x1xxxx0, GET2_NOPROC}:   next_state = IDLE;
    {13'b11x0x1x1xxxx0, GET2_NOPROC}:   next_state = IDLE;
    {13'bx110x1x1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'b001xx1x1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'bx101x1x1xxxx0, GET2_NOPROC}:   next_state = IDLE;
    {13'b1100xxx1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'bx01xx0x1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'b11x1x0x1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'b1000x0x1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'b1101x1x1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'b1110x0x1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'b0111x1x1xxxxx, GET2_NOPROC}:   next_state = IDLE;
    {13'b011xx0x1xxxx1, GET2_NOPROC}:   next_state = IDLE;
    {13'bxxxxx0x1xxxx0, GET2_NOPROC}:   next_state = IDLE;
    {13'b100110x1xxxx1, GET2_NOPROC}:   next_state = GET1;
    {13'b100111x1xxxxx, GET2_NOPROC}:   next_state = GET1;
    {13'bxxxxxxx0xxxxx, GET2_NOPROC}:   next_state = DRAIN_DATA;
    {13'b100100x1xxxx1, GET2_NOPROC}:   next_state = LOCK1_PROC;
    {13'b100101x1xxxxx, GET2_NOPROC}:   next_state = LOCK1_NOPROC;
