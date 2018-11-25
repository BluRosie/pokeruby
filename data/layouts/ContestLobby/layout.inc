ContestLobby_MapBorder::
	.incbin "data/layouts/ContestLobby/border.bin"

ContestLobby_MapBlockdata::
	.incbin "data/layouts/ContestLobby/map.bin"

	.align 2
ContestLobby_Layout::
	.4byte 0xf
	.4byte 0x7
	.4byte ContestLobby_MapBorder
	.4byte ContestLobby_MapBlockdata
	.4byte gTileset_Building
	.4byte gTileset_Contest

