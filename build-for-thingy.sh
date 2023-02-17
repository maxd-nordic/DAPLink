#!/bin/bash
rm -rf 0258_nrf5*.hex
rm -rf firmware_0258/
rm -rf projectfiles/

python tools/progen_compile.py nrf5340_thingy91_if nrf5340_bl nrf52840_thingy91_bl nrf52840_thingy91_if --release --clean

python3 mergehex.py projectfiles/make_gcc_arm/nrf5340_bl/build/nrf5340_bl.hex firmware_0258/0258_nrf5340_thingy91_0x10000.hex -o 0258_nrf5340_thingy91_w_bl.hex
python3 mergehex.py projectfiles/make_gcc_arm/nrf52840_thingy91_bl/build/nrf52840_thingy91_bl.hex firmware_0258/0258_nrf52840_thingy91_0x10000.hex -o 0258_nrf52840_thingy91_w_bl.hex
