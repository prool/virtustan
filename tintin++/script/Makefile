#install prool's tintin++ with scripts
# install binaries to /usr/local/bin and scripts to home dir /home/prool
LOCALBIN = /usr/local/bin
HOMEDIR = /home/prool

install:
	cp ../src/tt++ $(LOCALBIN)/tt++.elf
	cp tt++ $(LOCALBIN)
	iconv --from-code=UTF-8 --to-code=koi8-r < tintin++.cmd > tintin++koi.cmd
	cp *.cmd $(HOMEDIR)
