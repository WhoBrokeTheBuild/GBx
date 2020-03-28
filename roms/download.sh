#!/bin/bash

cd "${0%/*}"

get_rom() {
    local ROM="$1"
    local URL="$2"
    local ZIP="$ROM.zip"
    if [ ! -f "$ROM" ]; then
        wget "$URL" -O "$ZIP"
        local FILE=$(zipinfo -1 "$ZIP")
        unzip "$ZIP"
        mv "$FILE" "$ROM"
        rm "$ZIP"
        echo "Downloaded $ROM"
    else
        echo "Found existing $ROM"
    fi
}

# get_rom "LoZ_Links_Awakening.gb" "https://s1roms.cc/s3roms/Gameboy/Legend%20of%20Zelda%2C%20The%20-%20Link%27s%20Awakening%20%28U%29%20%28V1.2%29%20%5B%21%5D.zip"
# get_rom "PokemonBlue.gb" "https://s1roms.cc/s3roms/Gameboy/Pokemon%20-%20Blue%20Version%20%28UE%29%20%5BS%5D%5B%21%5D.zip"

# https://gbdev.gg8.se/files/roms/bootroms/
wget https://gbdev.gg8.se/files/roms/bootroms/dmg0_rom.bin -O dmg0_rom.bin
wget https://gbdev.gg8.se/files/roms/bootroms/sgb_boot.bin -O sgb_boot.bin
# wget https://gbdev.gg8.se/files/roms/bootroms/cgb_bootstrap.bin -O cgb_bootstrap.bin

[[ -d gb-test-roms ]] || {
    wget https://gekkio.fi/files/mooneye-gb/latest/mooneye-gb_hwtests.zip -O mooneye-gb_hwtests.zip
    unzip mooneye-gb_hwtests.zip -d mooneye
    rm mooneye-gb_hwtests.zip
}

[[ -d gb-test-roms ]] || git clone https://github.com/retrio/gb-test-roms.git

wait
echo "All Files Downloaded"
