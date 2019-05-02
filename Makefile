SERVERS=echo-select echo-poll

ALL= $(SERVERS)

all: $(ALL)

clean:
	rm -f $(ALL)
