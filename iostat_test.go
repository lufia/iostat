package iostat

import (
	"testing"
)

func TestXxx(t *testing.T) {
	a, err := ReadDriveStats()
	if err != nil {
		t.Fatal(err)
	}
	t.Logf("%d", len(a))
}
