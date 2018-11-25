BattleTower_Corridor_MapBorder::
	.incbin "data/layouts/BattleTower_Corridor/border.bin"

BattleTower_Corridor_MapBlockdata::
	.incbin "data/layouts/BattleTower_Corridor/map.bin"

	.align 2
BattleTower_Corridor_Layout::
	.4byte 0x11
	.4byte 0x5
	.4byte BattleTower_Corridor_MapBorder
	.4byte BattleTower_Corridor_MapBlockdata
	.4byte gTileset_Building
	.4byte gTileset_BattleTower

