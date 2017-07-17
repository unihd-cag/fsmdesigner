
VERSION=5.2.0
DVERSION:=$(VERSION)-0
DEBKEY?=5D88B0DB

deb-src: DIST?=UNRELEASED
deb-src:
	@echo "Making Deb Source Package..."
	@echo "Packing and Unpacking Stage..."
	@rm -Rf .deb && mkdir -p .deb/fsmdesigner_$(VERSION).orig
	@cp -Rfv src include CMakeLists.txt Copyright.txt git-get-revision.bat macros scripts .deb/fsmdesigner_$(VERSION).orig
	@cd .deb/ && tar -caf fsmdesigner_$(VERSION).orig.tar.gz fsmdesigner_$(VERSION).orig
	@echo "Generating Changelog from top folder, will move debian to subdirectory later..."
	@gbp dch --snapshot --force-distribution --ignore-branch --auto --distribution=$(DIST)
	@cp -Rf debian .deb/fsmdesigner_$(VERSION).orig/
	@cd .deb/fsmdesigner_$(VERSION).orig/ && debuild -d -k$(DEBKEY) -S

deb-build: DISTRIBUTION?=UNRELEASED
deb-build: ARCHITECTURE?=amd64
deb-build:
	@mkdir -p .deb/$(DISTRIBUTION)/$(ARCHITECTURE)/
	@rm -Rf .deb/$(DISTRIBUTION)/$(ARCHITECTURE)/*
	@sudo pbuilder --create --distribution $(DISTRIBUTION) --architecture $(ARCHITECTURE)
	@sudo pbuilder --update --distribution $(DISTRIBUTION) --architecture $(ARCHITECTURE)
	@sudo pbuilder --build --distribution $(DISTRIBUTION) --architecture $(ARCHITECTURE) .deb/*.dsc
	@cp -v /var/cache/pbuilder/result/fsmdesigner* .deb/$(DISTRIBUTION)/$(ARCHITECTURE)/
	@debsign --re-sign -k$(DEBKEY) .deb/$(DISTRIBUTION)/$(ARCHITECTURE)/*.changes

# @sudo pbuilder --create --distribution $(DISTRIBUTION) --architecture $(ARCHITECTURE)
