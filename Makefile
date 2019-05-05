SERVERS=echo-select echo-poll

ALL= $(SERVERS)

all: $(ALL)

echo-select:
	cc -o echo-select echo-select.c

echo-poll:
	cc -o echo-poll echo-poll.c

clean:
	rm -f $(ALL)
