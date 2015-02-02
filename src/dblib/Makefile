export CFG=/usr/bin/mysql_config
MYSQLFLAGS=`${CFG} --cflags`
MYSQLLIBS=`${CFG} --libs`
CFLAGS = -g
LDFLAGS  =  -fPIC  -shared -lrt -Wl,-soname,$@
DB_LIB=libdbop.so
DB_OBJ=dbop.o DAO.o
DB_TEST=test
GCC=gcc
 
all:${DB_LIB}

${DB_LIB}:${DB_OBJ}
	$(GCC) ${CFLAGS} ${MYSQLFLAGS} ${MYSQLLIBS} ${DB_OBJ} ${LDFLAGS} -o ${DB_LIB} 
dbop.o:dbop.c dbop.h
	${GCC} ${CFLAGS} -c -fPIC -o dbop.o `${CFG} --cflags` dbop.c `${CFG} --libs`
DAO.o:DAO.c DAO.h
	${GCC} ${CFLAGS} -c -fPIC -o DAO.o `${CFG} --cflags` DAO.c `${CFG} --libs`
test:dbop.c dbop.h
	${GCC} -DTEST_MAIN  ${CFLAGS} -o ${DB_TEST} `${CFG} --cflags` dbop.c `${CFG} --libs`
clean:
	rm -f ${DB_TEST} ${DB_OBJ} ${DB_LIB}
