# Top-level helpers for n64-educator
# Requires N64_INST for ROM targets (see docs/guide/setup.md).

.PHONY: help lessons l01 l02 l03 l04 l06 l07 l08 l09 l10 l11 l12 l13 l14 l15 l16 l26 l27 l28 l29 l30 l31 l32 l33 l34 m3lab m4toy cove checkpoint docs docs-dev clean-lessons assets

help:
	@echo "Targets:"
	@echo "  make l01..l16 l26..l34 m3lab m4toy cove checkpoint assets"
	@echo "  make lessons | docs | docs-dev | clean-lessons"

l01:
	$(MAKE) -C lessons/l01-hello-rom

l02:
	$(MAKE) -C lessons/l02-game-loop

l03:
	$(MAKE) -C lessons/l03-controllers

l04:
	$(MAKE) -C lessons/l04-dfs

checkpoint:
	$(MAKE) -C lessons/m0-checkpoint

l06:
	$(MAKE) -C lessons/l06-vectors

l07:
	$(MAKE) -C lessons/l07-matrices

l08:
	$(MAKE) -C lessons/l08-camera

l09:
	$(MAKE) -C lessons/l09-meshes

l10:
	$(MAKE) -C lessons/l10-color-light

l11:
	$(MAKE) -C lessons/l11-t3d-quad

l12:
	$(MAKE) -C lessons/l12-depth-objects

l13:
	$(MAKE) -C lessons/l13-orbit-camera

l14:
	$(MAKE) -C lessons/l14-lighting

l15:
	$(MAKE) -C lessons/l15-load-model

l16:
	$(MAKE) -C lessons/l16-vertex-color

m3lab:
	$(MAKE) -C lessons/m3-asset-lab

l26:
	$(MAKE) -C lessons/l26-move

l27:
	$(MAKE) -C lessons/l27-anim-drive

l28:
	$(MAKE) -C lessons/l28-follow-cam

l29:
	$(MAKE) -C lessons/l29-collision

l30:
	$(MAKE) -C lessons/l30-entities

l31:
	$(MAKE) -C lessons/l31-game-state

m4toy:
	$(MAKE) -C lessons/m4-toy-cove

l32:
	$(MAKE) -C lessons/l32-audio

l33:
	$(MAKE) -C lessons/l33-hud

l34:
	$(MAKE) -C lessons/l34-juice

cove:
	$(MAKE) -C capstone/starshard-cove

assets:
	./scripts/build-course-assets.sh

lessons:
	./scripts/build-all.sh

docs:
	npm run docs:build

docs-dev:
	npm run docs:dev

clean-lessons:
	@for d in lessons/*/; do \
	  if [ -f "$$d/Makefile" ]; then $(MAKE) -C "$$d" clean; fi; \
	done
