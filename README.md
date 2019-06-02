# Pokémon Ruby and Sapphire

Pokémon Ruby and Sapphire, but with various edits to show off just what is all super easily done with a decompilation.
Everything here is public to use, no permission required.  Only requirement is that the credits are given where credits are due.

Current implemented features:
* a ton of decapitalization
* moves are mostly updated to current generation, exceptions noted in src/data/battle_moves.c
* gen vi exp. share, toggleable from the bag and everything
* poison fades after reaching 1 hp in the overworld
* ball expansion
* hidden abilities
* gen 4 bike (toggle between acro and mach using the r button)
* run toggle using the r button
* fix snow
* add fog (replaces weather_fog_3)
* add snow and fog corresponding to in-battle
* gender differences
* redesigned party menus

In Progress:
* ability expansion (thanks znogyrop and dizzy for ability descriptions)
* item expansion - maybe item pictures too? (all battle items in gen iv have been implemented)
* move expansion (thanks dizzy for the base data for the moves)
* eventually, pokémon expansion (huge thanks to dizzy & everyone there.  the ds-style 64x64 sprite projects as well)

It builds the following rom:

* pokeruby.gba

To set up the repository, see [INSTALL.md](INSTALL.md).

## See also, done by the pret team

* Disassembly of [**Pokémon Red/Blue**][pokered]
* Disassembly of [**Pokémon Yellow**][pokeyellow]
* Disassembly of [**Pokémon Gold**][pokegold]
* Disassembly of [**Pokémon Crystal**][pokecrystal]
* Disassembly of [**Pokémon Pinball**][pokepinball]
* Disassembly of [**Pokémon TCG**][poketcg]
* Disassembly of [**Pokémon Fire Red**][pokefirered]
* Disassembly of [**Pokémon Emerald**][pokeemerald]
* Discord: [**pret**][Discord]
* irc: **irc.freenode.net** [**#pret**][irc]

[pokered]: https://github.com/pret/pokered
[pokeyellow]: https://github.com/pret/pokeyellow
[pokegold]: https://github.com/pret/pokegold
[pokecrystal]: https://github.com/pret/pokecrystal
[pokepinball]: https://github.com/pret/pokepinball
[poketcg]: https://github.com/pret/poketcg
[pokefirered]: https://github.com/pret/pokefirered
[pokeemerald]: https://github.com/pret/pokeemerald
[Discord]: https://discord.gg/6EuWgX9
[irc]: https://kiwiirc.com/client/irc.freenode.net/?#pret
[travis]: https://travis-ci.org/pret/pokeruby
[travis-badge]: https://travis-ci.org/pret/pokeruby.svg?branch=master
