Route117_MapBorder::
	.incbin "data/layouts/Route117/border.bin"

Route117_MapBlockdata::
	.incbin "data/layouts/Route117/map.bin"

	.align 2
Route117_Layout::
	.4byte 0x24
	.4byte 0x1c
	.4byte Route117_MapBorder
	.4byte Route117_MapBlockdata
	.4byte gTileset_General
	.4byte gTileset_Slateport

