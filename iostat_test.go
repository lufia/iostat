package iostat

import (
	"testing"
)

func TestReadDriveStats(t *testing.T) {
	a, err := ReadDriveStats()
	if err != nil {
		t.Fatal(err)
	}
	if len(a) == 0 {
		t.Errorf("ReadDriveStats() = 0; want >1")
		return
	}
	for _, stat := range a {
		t.Run(stat.Name, func(t *testing.T) {
			if stat.Size <= 0 {
				t.Errorf("Size = %d; want >0", stat.Size)
			}
			if stat.BlockSize <= 0 {
				t.Errorf("BlockSize = %d; want >0", stat.BlockSize)
			}
			if stat.BytesRead <= 0 {
				t.Errorf("BytesRead = %d; want >0", stat.BytesRead)
			}
			t.Logf("Size = %d\n", stat.Size)
			t.Logf("BlockSize = %d\n", stat.BlockSize)
			t.Logf("BytesRead = %d\n", stat.BytesRead)
			t.Logf("BytesWritten = %d\n", stat.BytesWritten)
			t.Logf("NumRead = %d\n", stat.NumRead)
			t.Logf("NumWrite = %d\n", stat.NumWrite)
			t.Logf("TotalReadTime = %v\n", stat.TotalReadTime)
			t.Logf("TotalWriteTime = %v\n", stat.TotalWriteTime)
			t.Logf("ReadLatency = %v\n", stat.ReadLatency)
			t.Logf("WriteLatency = %v\n", stat.WriteLatency)
		})
	}
}

func TestReadCPUStats(t *testing.T) {
	cpu, err := ReadCPUStats()
	if err != nil {
		t.Fatal(err)
	}
	if cpu.User == 0 {
		t.Errorf("User = %d; want >0", cpu.User)
	}
	if cpu.Sys == 0 {
		t.Errorf("Sys = %d; want >0", cpu.Sys)
	}
	t.Logf("User = %d\n", cpu.User)
	t.Logf("Nice = %d\n", cpu.Nice)
	t.Logf("Sys = %d\n", cpu.Sys)
	t.Logf("Idle = %d\n", cpu.Idle)
}

func TestReadLoadAvg(t *testing.T) {
	load, err := ReadLoadAvg()
	if err != nil {
		t.Fatal(err)
	}
	if load.Load1 <= 0.0 {
		t.Errorf("Load1 = %4.2f; want >0", load.Load1)
	}
	if load.Load5 <= 0.0 {
		t.Errorf("Load5 = %4.2f; want >0", load.Load5)
	}
	if load.Load15 <= 0.0 {
		t.Errorf("Load15 = %4.2f; want >0", load.Load15)
	}
	t.Logf("Load1 = %4.2f", load.Load1)
	t.Logf("Load5 = %4.2f", load.Load5)
	t.Logf("Load15 = %4.2f", load.Load15)
}
