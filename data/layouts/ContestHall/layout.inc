ContestHall_MapBorder::
	.incbin "data/layouts/ContestHall/border.bin"

ContestHall_MapBlockdata::
	.incbin "data/layouts/ContestHall/map.bin"

	.align 2
ContestHall_Layout::
	.4byte 0x15
	.4byte 0x12
	.4byte ContestHall_MapBorder
	.4byte ContestHall_MapBlockdata
	.4byte gTileset_Building
	.4byte gTileset_Contest

