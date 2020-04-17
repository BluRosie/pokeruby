CASTFORMGFXDIR := graphics/pokemon/castform
CHERRIMFORMGFXDIR := graphics/pokemon/cherrim

$(CASTFORMGFXDIR)/back.4bpp: $(CASTFORMGFXDIR)/back_normal_form.4bpp \
                             $(CASTFORMGFXDIR)/back_sunny_form.4bpp \
                             $(CASTFORMGFXDIR)/back_rainy_form.4bpp \
                             $(CASTFORMGFXDIR)/back_snowy_form.4bpp
	@cat $(CASTFORMGFXDIR)/back_normal_form.4bpp $(CASTFORMGFXDIR)/back_sunny_form.4bpp $(CASTFORMGFXDIR)/back_rainy_form.4bpp $(CASTFORMGFXDIR)/back_snowy_form.4bpp >$@

$(CASTFORMGFXDIR)/anim_front.4bpp: $(CASTFORMGFXDIR)/anim_front_normal_form.4bpp \
                                   $(CASTFORMGFXDIR)/anim_front_sunny_form.4bpp \
                                   $(CASTFORMGFXDIR)/anim_front_rainy_form.4bpp \
                                   $(CASTFORMGFXDIR)/anim_front_snowy_form.4bpp
	@cat $(CASTFORMGFXDIR)/anim_front_normal_form.4bpp $(CASTFORMGFXDIR)/anim_front_sunny_form.4bpp $(CASTFORMGFXDIR)/anim_front_rainy_form.4bpp $(CASTFORMGFXDIR)/anim_front_snowy_form.4bpp >$@

$(CASTFORMGFXDIR)/normal.gbapal: $(CASTFORMGFXDIR)/normal_normal_form.gbapal \
                                 $(CASTFORMGFXDIR)/normal_sunny_form.gbapal \
                                 $(CASTFORMGFXDIR)/normal_rainy_form.gbapal \
                                 $(CASTFORMGFXDIR)/normal_snowy_form.gbapal
	@cat $(CASTFORMGFXDIR)/normal_normal_form.gbapal $(CASTFORMGFXDIR)/normal_sunny_form.gbapal $(CASTFORMGFXDIR)/normal_rainy_form.gbapal $(CASTFORMGFXDIR)/normal_snowy_form.gbapal >$@

$(CASTFORMGFXDIR)/shiny.gbapal: $(CASTFORMGFXDIR)/shiny_normal_form.gbapal \
                                $(CASTFORMGFXDIR)/shiny_sunny_form.gbapal \
                                $(CASTFORMGFXDIR)/shiny_rainy_form.gbapal \
                                $(CASTFORMGFXDIR)/shiny_snowy_form.gbapal
	@cat $(CASTFORMGFXDIR)/shiny_normal_form.gbapal $(CASTFORMGFXDIR)/shiny_sunny_form.gbapal $(CASTFORMGFXDIR)/shiny_rainy_form.gbapal $(CASTFORMGFXDIR)/shiny_snowy_form.gbapal >$@

$(CHERRIMFORMGFXDIR)/back.4bpp: $(CHERRIMFORMGFXDIR)/back_normal.4bpp \
                                $(CHERRIMFORMGFXDIR)/back_sun.4bpp
	@cat $(CHERRIMFORMGFXDIR)/back_normal.4bpp $(CHERRIMFORMGFXDIR)/back_sun.4bpp >$@

$(CHERRIMFORMGFXDIR)/front.4bpp: $(CHERRIMFORMGFXDIR)/front_normal.4bpp \
                                 $(CHERRIMFORMGFXDIR)/front_sun.4bpp
	@cat $(CHERRIMFORMGFXDIR)/front_normal.4bpp $(CHERRIMFORMGFXDIR)/front_sun.4bpp >$@

$(CHERRIMFORMGFXDIR)/normal.gbapal: $(CHERRIMFORMGFXDIR)/normal_normal.gbapal \
                                    $(CHERRIMFORMGFXDIR)/normal_sun.gbapal
	@cat $(CHERRIMFORMGFXDIR)/normal_normal.gbapal $(CHERRIMFORMGFXDIR)/normal_sun.gbapal >$@

$(CHERRIMFORMGFXDIR)/shiny.gbapal: $(CHERRIMFORMGFXDIR)/shiny_normal.gbapal \
                                   $(CHERRIMFORMGFXDIR)/shiny_sun.gbapal
	@cat $(CHERRIMFORMGFXDIR)/shiny_normal.gbapal $(CHERRIMFORMGFXDIR)/shiny_sun.gbapal >$@
