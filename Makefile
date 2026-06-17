PHP_VERSION ?= 8.3
PHP_VARIANT ?= cli
MARKDOWN_VERSION ?= $(shell git describe --tags --dirty --always 2>/dev/null || git rev-parse --short HEAD 2>/dev/null || printf unknown)
DOCKER_IMAGE ?= php:$(PHP_VERSION)-$(PHP_VARIANT)
TESTS ?= tests
MD4C_REPO ?= https://github.com/mity/md4c.git
MD4C_REF ?= master
MD4C_DIR := third_party/md4c
MD4C_FILES := LICENSE.md entity.c entity.h md4c.c md4c.h md4c-html.c md4c-html.h

.PHONY: test test-docker md4c-version md4c-check-upgrade md4c-upgrade

test: test-docker

test-docker:
	docker run --rm \
		-v "$(CURDIR):/workspace:ro" \
		-w /workspace \
		-e TESTS="$(TESTS)" \
		-e MARKDOWN_VERSION="$(MARKDOWN_VERSION)" \
		$(DOCKER_IMAGE) \
		sh -lc 'set -eu; \
			rm -rf /tmp/markdown; \
			cp -a /workspace /tmp/markdown; \
			cd /tmp/markdown; \
			phpize; \
			./configure --enable-markdown; \
			make -j"$$(nproc)"; \
			make test TESTS="$${TESTS}"'

md4c-version:
	@set -eu; \
	if [ ! -f "$(MD4C_DIR)/VERSION" ]; then \
		printf 'No MD4C version metadata found at %s\n' "$(MD4C_DIR)/VERSION"; \
		exit 1; \
	fi; \
	printf 'Bundled MD4C:\n'; \
	sed 's/^/  /' "$(MD4C_DIR)/VERSION"

md4c-check-upgrade:
	@set -eu; \
	current=$$(sed -n 's/^commit=//p' "$(MD4C_DIR)/VERSION"); \
	latest=$$(git ls-remote "$(MD4C_REPO)" "$(MD4C_REF)" 2>/dev/null | awk '/\^\{\}$$/{commit=$$1} {last=$$1} END{print commit ? commit : last}'); \
	if [ -z "$$latest" ]; then \
		printf 'Could not resolve %s from %s\n' "$(MD4C_REF)" "$(MD4C_REPO)"; \
		exit 1; \
	fi; \
	printf 'Bundled MD4C: %s\n' "$$current"; \
	printf 'Upstream MD4C (%s): %s\n' "$(MD4C_REF)" "$$latest"; \
	if [ "$$current" = "$$latest" ]; then \
		printf 'MD4C is up to date.\n'; \
	else \
		printf 'MD4C update available. Run: make md4c-upgrade MD4C_REF=%s\n' "$(MD4C_REF)"; \
	fi

md4c-upgrade:
	@set -eu; \
	tmp=$$(mktemp -d); \
	trap 'rm -rf "$$tmp"' EXIT; \
	if ! git clone --depth 1 --branch "$(MD4C_REF)" "$(MD4C_REPO)" "$$tmp/md4c" >/dev/null 2>&1; then \
		git clone --depth 1 "$(MD4C_REPO)" "$$tmp/md4c"; \
		git -C "$$tmp/md4c" fetch --depth 1 origin "$(MD4C_REF)"; \
		git -C "$$tmp/md4c" checkout --detach FETCH_HEAD; \
	fi; \
	for file in $(MD4C_FILES); do \
		cp "$$tmp/md4c/src/$$file" "$(MD4C_DIR)/$$file" 2>/dev/null || cp "$$tmp/md4c/$$file" "$(MD4C_DIR)/$$file"; \
	done; \
	commit=$$(git -C "$$tmp/md4c" rev-parse HEAD); \
	date=$$(git -C "$$tmp/md4c" log -1 --format=%cs); \
	{ \
		printf 'source=%s\n' "$(MD4C_REPO)"; \
		printf 'ref=%s\n' "$(MD4C_REF)"; \
		printf 'commit=%s\n' "$$commit"; \
		printf 'date=%s\n' "$$date"; \
	} > "$(MD4C_DIR)/VERSION"; \
	$(MAKE) md4c-version
