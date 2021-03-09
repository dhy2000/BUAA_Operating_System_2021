target=fibo
src_dir=code
inc_dir=include
obj_dir=code

src=main.c fibo.c
inc=fibo.h
obj=main.o fibo.o

$(target): $(addprefix $(obj_dir)/,$(obj))
	gcc -o $@ $^

$(obj_dir)/%.o: $(src_dir)/%.c $(inc_dir)/$(inc)
	gcc -o $@ -c $< -I $(inc_dir)

.PHONY: clean

clean:
	rm $(obj_dir)/fibo.o
	rm $(obj_dir)/main.o
	rm $(target)
