// Package iostat
package iostat

import "time"

type DriveStats struct {
	Name      string // drive name
	Size      int64  // total drive size in bytes
	BlockSize int64  // block size in bytes

	BytesRead    int64
	BytesWritten int64
	NumReads     int64
	NumWrites    int64
	ReadLatency  time.Duration
	WriteLatency time.Duration
}
