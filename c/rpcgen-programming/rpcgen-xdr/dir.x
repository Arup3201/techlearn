/*
* dir.x implements some local data types that are not available in libnsl
*/

const MAXDIRLEN = 255;

typedef string nametype<MAXDIRLEN>;

typedef struct namenode *namelist;

typedef struct namenode namenode;

struct namenode {
	nametype name; 
	namelist next;
};

union readdir_res switch(int error) {
	case 0:
		namelist list;
	default:
		void;
};


program DIRPROG {
	version DIRVERS {
		readdir_res
		READDIR(nametype) = 1;
	} = 1;
} = 0x20000001;
