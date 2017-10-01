module read_switches (
	input CLK100,
	input [3:0] SWITCH,
	output SPI_MISO,
	input SPI_MOSI, SPI_SCK, SPI_SS
);

	wire clk = CLK100;
	wire [7:0] spi_txdata, spi_rxdata;
	wire spi_rxready;

	assign spi_txdata = {4'b0,~SWITCH};

	spislave SPI (
		.clk(clk),
		.txdata(spi_txdata),
		.rxdata(spi_rxdata),
		.rxready(spi_rxready),
		.mosi(SPI_MOSI),
		.miso(SPI_MISO),
		.sck(SPI_SCK),
		.ss(SPI_SS)
	);

endmodule

module spislave #(parameter WIDTH=8, LOGWIDTH=3) (
	input clk,
	input [WIDTH-1:0] txdata,      // slave to master
	output [WIDTH-1:0] rxdata,     // master to slave
	output txready,       // txdata captured on this cycle
	output reg rxready,   // rxdata valid on this cycle
	input mosi, sck, ss,  // master to slave
	output miso           // slave to master
);

	// data registers
	reg [WIDTH:0] shiftreg;       // left shift mosi in, miso out
	reg [LOGWIDTH-1:0] bitcount;  // # valid mosi bits in shiftreg
	assign miso = shiftreg[WIDTH];
	assign rxdata = shiftreg[WIDTH-1:0];

	// synchronise SCK and SS (from outside clock domain)
	reg [2:0] sck_sync, ss_sync;
	always @(posedge clk) begin
		sck_sync <= {sck,sck_sync[2:1]};
		ss_sync <= {ss,ss_sync[2:1]};
	end
	wire sck_rising = (sck_sync[1:0] == 2'b10);
	wire sck_falling = (sck_sync[1:0] == 2'b01);
	wire selected = ~ss_sync[1];
	assign txready = selected && sck_rising && bitcount == 0;

	// Mode 1: CPOL=0 CPHA=1
	always @(posedge clk) begin
		if (~selected) begin
			bitcount <= 0;
			shiftreg <= 0;
			rxready <= 0;
		end else if (sck_rising) begin
			// set up transmitted data on sck leading edge
			if (bitcount == 0)
				shiftreg <= {txdata,1'b0};
			else
				shiftreg <= shiftreg << 1;
		end else if (sck_falling) begin
			// sample received data on sck trailing edge
			shiftreg[0] <= mosi;
			if (bitcount == WIDTH-1)
				rxready <= 1;
			bitcount <= bitcount + 1;
		end else
			rxready <= 0;
	end

endmodule
