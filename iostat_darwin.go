// +build darwin

package iostat

// #cgo LDFLAGS: -framework CoreFoundation -framework IOKit
// #include <stdint.h>
// #include "iostat.h"
import "C"

func ReadDriveStats() ([]*DriveStats, error) {
	var buf [C.NDRIVE]C.DriveStats
	n, err := C.readdrivestat(&buf[0], C.int(len(buf)))
	if err != nil {
		return nil, err
	}
	stats := make([]*DriveStats, n)
	for i := 0; i < int(n); i++ {
		stats[i] = &DriveStats{
			Name: C.GoString(&buf[i].name[0]),
		}
	}
	return stats, nil
}
