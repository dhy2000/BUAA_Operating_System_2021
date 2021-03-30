
# ========= Configuration =========

INCLUDES	  := -I../include/

# ======== End of configuration ===

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $< 

%.o: %.c
	$(CC) $(DEFS) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: my_cal.o my_driver.o

clean:
	rm -rf *~ *.o

include ../include.mk
