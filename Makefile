SUBDIRS = 01_hello 02_pcd

all: subdirs
    
subdirs: $(SUBDIRS)
	for dir in $(SUBDIRS); do 	\
		$(MAKE) -C $$dir; 	\
	done

clean: $(SUBDIRS)
	for dir in $(SUBDIRS); do 	\
		$(MAKE) -C $$dir clean; \
	done
tag:
	ctags -R
	cscope -Rbqk

