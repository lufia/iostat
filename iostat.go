// Package iostat presents I/O statistics.
package iostat

import "time"

// DriveStats represents I/O statistics of a drive.
type DriveStats struct {
	Name      string // drive name
	Size      int64  // total drive size in bytes
	BlockSize int64  // block size in bytes

	BytesRead    int64
	BytesWritten int64
	NumReads     int64
	NumWrites    int64
	ReadTime     time.Duration
	WriteTime    time.Duration
}
