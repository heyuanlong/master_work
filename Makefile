Dir=busi/ \
busi/busi_test/ \
core/ \
event/ \
conn/ \
net/ \
os/


#---------------------------
Src=$(foreach dir,$(Dir), $(wildcard $(dir)*.c)) main.c
FileSrc=$(notdir $(Src))


#---------------------------
MasterObjDir=obj/master


#---------------------------obj
MasterObj=$(patsubst %.c,$(MasterObjDir)/%.o,$(FileSrc))

#---------------------------bin
master=./build/master/bin/masterbin
clientTest =./build/master/bin/clienttestnet

#---------------------------
Inc=$(foreach dir, $(Dir), -I $(dir) )
CC=gcc
Lib= -lpthread

#---------------------------
all:makeDir copySrc $(master)  $(clientTest) 
	@echo all is ok

makeDir:
	mkdir -p $(MasterObjDir)
	mkdir -p ./build/master/{bin,conf,log}
	@echo makeDir ok

copySrc:
	rsync -avzP $(Src) $(MasterObjDir)


#---------------------------


MasterFlags= -g -w -O0 $(Inc) -DKO_MASTER_MASTER -DKO_CACHE_REDIS
$(master):$(MasterObj) 
	@echo make master
	$(CC) -g -w  -o $@  $^ $(Lib)
	
$(MasterObjDir)/%.o:$(MasterObjDir)/%.c
	@echo make master src
	$(CC) $(MasterFlags) -c $< -o $@


$(clientTest): clientTest/client.h clientTest/client.cpp busi/vs_interface.h
	g++ -g -std=c++0x $(Inc) $(Lib) -o $@ $^

#---------------------------clean


cleanall:
	rm -rf ./obj 
	rm -rf $(master)
	rm -rf $(clientTest)
	@echo cleanall ok!

clean:
	rm -f $(MasterObjDir)/*
	rm -f $(master)
	rm -f $(clientTest)
	@echo clean ok! 
