MauvilleCity_MapBorder::
	.incbin "data/layouts/MauvilleCity/border.bin"

MauvilleCity_MapBlockdata::
	.incbin "data/layouts/MauvilleCity/map.bin"

	.align 2
MauvilleCity_Layout::
	.4byte 36
	.4byte 24
	.4byte MauvilleCity_MapBorder
	.4byte MauvilleCity_MapBlockdata
	.4byte gTileset_General
	.4byte gTileset_Slateport

