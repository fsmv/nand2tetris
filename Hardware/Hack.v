module Hack(
	KEY,
	LEDG,
	SW
);

input[7:0] SW;
input[3:0] KEY;
output[7:0] LEDG;

Add16(.a(SW), .b(~KEY), .out(LEDG));

endmodule
