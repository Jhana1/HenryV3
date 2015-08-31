# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\xavieran\Desktop\ECE3091\PSOCProjects\ENG3091_MandM\ENG3091_MandM.cydsn\ENG3091_MandM.cyprj
# Date: Mon, 31 Aug 2015 06:41:08 GMT
#set_units -time ns
create_clock -name {CAM_CLK(routed)} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dclk_0}]]
create_clock -name {Clock(routed)} -period 100000 -waveform {0 50000} [list [get_pins {ClockBlock/dclk_5}]]
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_clock -name {CyBUS_CLK(fixed-function)} -period 41.666666666666664 -waveform {0 20.8333333333333} [get_pins {ClockBlock/clk_bus_glb_ff}]
create_generated_clock -name {CAM_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 3 5} [list [get_pins {ClockBlock/dclk_glb_1}]]
create_clock -name {Clock_2(fixed-function)} -period 83.333333333333329 -waveform {0 41.6666666666667} [get_pins {ClockBlock/dclk_glb_ff_1}]
create_generated_clock -name {SCLOCK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 601 1201} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_clock -name {SCLOCK(fixed-function)} -period 25000 -waveform {0 41.6666666666667} [get_pins {ClockBlock/dclk_glb_ff_2}]
create_generated_clock -name {M1Clock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 601 1201} [list [get_pins {ClockBlock/dclk_glb_3}]]
create_clock -name {M1Clock(fixed-function)} -period 25000 -waveform {0 41.6666666666667} [get_pins {ClockBlock/dclk_glb_ff_3}]
create_generated_clock -name {M2Clock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 601 1201} [list [get_pins {ClockBlock/dclk_glb_4}]]
create_clock -name {M2Clock(fixed-function)} -period 25000 -waveform {0 41.6666666666667} [get_pins {ClockBlock/dclk_glb_ff_4}]
create_generated_clock -name {Clock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2401 4801} [list [get_pins {ClockBlock/dclk_glb_5}]]
create_generated_clock -name {timer_clock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2401 4801} [list [get_pins {ClockBlock/dclk_glb_6}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for C:\Users\xavieran\Desktop\ECE3091\PSOCProjects\ENG3091_MandM\ENG3091_MandM.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\xavieran\Desktop\ECE3091\PSOCProjects\ENG3091_MandM\ENG3091_MandM.cydsn\ENG3091_MandM.cyprj
# Date: Mon, 31 Aug 2015 06:41:01 GMT
