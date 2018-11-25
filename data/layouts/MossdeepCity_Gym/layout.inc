MossdeepCity_Gym_MapBorder::
	.incbin "data/layouts/MossdeepCity_Gym/border.bin"

MossdeepCity_Gym_MapBlockdata::
	.incbin "data/layouts/MossdeepCity_Gym/map.bin"

	.align 2
MossdeepCity_Gym_Layout::
	.4byte 0x14
	.4byte 0x1e
	.4byte MossdeepCity_Gym_MapBorder
	.4byte MossdeepCity_Gym_MapBlockdata
	.4byte gTileset_Building
	.4byte gTileset_MossdeepGym

