all:
	make -C luam/
	make -C lua51/ linux 
	make -C lua2c/
	make -C astdump/  
	
clean:
	make -C luam/ clean
	make -C lua51/ clean 
	make -C lua2c/ clean 
	make -C astdump/ clean 