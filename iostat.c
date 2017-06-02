#include <stdint.h>
#include "iostat.h"

#define IOKIT	1	/* to get io_name_t in device_types.h */

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/IOBlockStorageDriver.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/IOBSD.h>

static int getdrivestat(io_registry_entry_t d, DriveStats *stat);
static int fillstat(io_registry_entry_t d, DriveStats *stat);

int
readdrivestat(DriveStats a[], int n)
{
	mach_port_t port;
	CFMutableDictionaryRef match;
	io_iterator_t drives;
	io_registry_entry_t d;
	kern_return_t status;
	int na, rv;

	IOMasterPort(bootstrap_port, &port);
	match = IOServiceMatching("IOMedia");
	CFDictionaryAddValue(match, CFSTR(kIOMediaWholeKey), kCFBooleanTrue);
	status = IOServiceGetMatchingServices(port, match, &drives);
	if(status != KERN_SUCCESS)
		return -1;

	na = 0;
	while(na < n && (d=IOIteratorNext(drives)) > 0){
		rv = getdrivestat(d, &a[na]);
		if(rv < 0)
			return -1;
		if(rv > 0)
			na++;
		IOObjectRelease(d);
	}
	IOObjectRelease(drives);
	return na;
}

static int
getdrivestat(io_registry_entry_t d, DriveStats *stat)
{
	io_registry_entry_t parent;
	kern_return_t status;
	CFDictionaryRef props;
	CFStringRef name;
	CFNumberRef num;
	int rv;

	memset(stat, 0, sizeof *stat);
	status = IORegistryEntryGetParentEntry(d, kIOServicePlane, &parent);
	if(status != KERN_SUCCESS)
		return -1;
	if(!IOObjectConformsTo(parent, "IOBlockStorageDriver")){
		IOObjectRelease(parent);
		return 0;
	}

	status = IORegistryEntryCreateCFProperties(d, (CFMutableDictionaryRef *)&props, kCFAllocatorDefault, kNilOptions);
	if(status != KERN_SUCCESS){
		IOObjectRelease(parent);
		return -1;
	}
	name = (CFStringRef)CFDictionaryGetValue(props, CFSTR(kIOBSDNameKey));
	CFStringGetCString(name, stat->name, NAMELEN, CFStringGetSystemEncoding());
	num = (CFNumberRef)CFDictionaryGetValue(props, CFSTR(kIOMediaSizeKey));
	CFNumberGetValue(num, kCFNumberSInt64Type, &stat->size);
	num = (CFNumberRef)CFDictionaryGetValue(props, CFSTR(kIOMediaPreferredBlockSizeKey));
	CFNumberGetValue(num, kCFNumberSInt64Type, &stat->blocksize);
	CFRelease(props);

	rv = fillstat(parent, stat);
	IOObjectRelease(parent);
	if(rv < 0)
		return -1;
	return 1;
}

static int
fillstat(io_registry_entry_t d, DriveStats *stat)
{
	CFDictionaryRef props, v;
	CFNumberRef num;
	kern_return_t status;

	status = IORegistryEntryCreateCFProperties(d, (CFMutableDictionaryRef *)&props, kCFAllocatorDefault, kNilOptions);
	if(status != KERN_SUCCESS)
		return -1;
	v = (CFDictionaryRef)CFDictionaryGetValue(props, CFSTR(kIOBlockStorageDriverStatisticsKey));
	if(v == NULL){
		CFRelease(props);
		return -1;
	}

	num = (CFNumberRef)CFDictionaryGetValue(v, CFSTR(kIOBlockStorageDriverStatisticsBytesReadKey));
	if(num)
		CFNumberGetValue(num, kCFNumberSInt64Type, &stat->read);
	num = (CFNumberRef)CFDictionaryGetValue(v, CFSTR(kIOBlockStorageDriverStatisticsBytesWrittenKey));
	if(num)
		CFNumberGetValue(num, kCFNumberSInt64Type, &stat->written);

	num = (CFNumberRef)CFDictionaryGetValue(v, CFSTR(kIOBlockStorageDriverStatisticsReadsKey));
	if(num)
		CFNumberGetValue(num, kCFNumberSInt64Type, &stat->nread);
	num = (CFNumberRef)CFDictionaryGetValue(v, CFSTR(kIOBlockStorageDriverStatisticsWritesKey));
	if(num)
		CFNumberGetValue(num, kCFNumberSInt64Type, &stat->nwrite);

	num = (CFNumberRef)CFDictionaryGetValue(v, CFSTR(kIOBlockStorageDriverStatisticsLatentReadTimeKey));
	if(num)
		CFNumberGetValue(num, kCFNumberSInt64Type, &stat->readtime);
	num = (CFNumberRef)CFDictionaryGetValue(v, CFSTR(kIOBlockStorageDriverStatisticsLatentWriteTimeKey));
	if(num)
		CFNumberGetValue(num, kCFNumberSInt64Type, &stat->writetime);

	CFRelease(props);
	return 0;
}
