all:
	make -C luam/
	make -C lua51/ linux 
	make -C astdump/  
	
clean:
	make -C luam/ clean
	make -C lua51/ clean 
	make -C astdump/ clean 
