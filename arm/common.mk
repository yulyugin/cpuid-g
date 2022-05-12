all: app driver
clean: clean-app clean-driver

app:
	$(MAKE) -C app

clean-app:
	$(MAKE) -C app clean

.PHONY: app
