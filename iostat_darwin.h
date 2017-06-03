typedef struct DriveStats DriveStats;

enum {
	NDRIVE = 16,
	NAMELEN = 31
};

typedef long long vlong;
struct DriveStats {
	char name[NAMELEN+1];
	vlong size;
	vlong blocksize;

	vlong read;
	vlong written;
	vlong nreads;
	vlong nwrites;
	vlong readtime;
	vlong writetime;
};

extern int readdrivestat(DriveStats a[], int n);
