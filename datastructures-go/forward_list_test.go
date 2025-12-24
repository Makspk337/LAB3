package datastructures

import (
	"bytes"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestForwardList(t *testing.T) {
	t.Run("PushHead and Read", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushHead("c")
		fl.PushHead("b")
		fl.PushHead("a")
		assert.Equal(t, "a b c", fl.Read())
	})

	t.Run("PushTail and Read", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("b")
		fl.PushTail("c")
		assert.Equal(t, "a b c", fl.Read())
	})

	t.Run("PushBefore", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("c")
		
		fl.PushBefore("c", "b")
		assert.Equal(t, "a b c", fl.Read())
		
		fl.PushBefore("a", "x")
		assert.Equal(t, "x a b c", fl.Read())
	})

	t.Run("PushAfter", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("c")
		
		fl.PushAfter("a", "b")
		assert.Equal(t, "a b c", fl.Read())
		
		fl.PushAfter("c", "d")
		assert.Equal(t, "a b c d", fl.Read())
	})

	t.Run("DeleteHead", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("b")
		fl.PushTail("c")
		
		fl.DeleteHead()
		assert.Equal(t, "b c", fl.Read())
		
		fl.DeleteHead()
		assert.Equal(t, "c", fl.Read())
		
		fl.DeleteHead()
		assert.Equal(t, "", fl.Read())
		
		fl.DeleteHead()
		assert.Equal(t, "", fl.Read())
	})

	t.Run("DeleteTail", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("b")
		fl.PushTail("c")
		
		fl.DeleteTail()
		assert.Equal(t, "a b", fl.Read())
		
		fl.DeleteTail()
		assert.Equal(t, "a", fl.Read())
		
		fl.DeleteTail()
		assert.Equal(t, "", fl.Read())
		
		fl.DeleteTail()
		assert.Equal(t, "", fl.Read())
	})

	t.Run("DeleteBefore", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("b")
		fl.PushTail("c")
		fl.PushTail("d")
		
		fl.DeleteBefore("c")
		assert.Equal(t, "a c d", fl.Read())
		
		fl.DeleteBefore("a")
		assert.Equal(t, "a c d", fl.Read())
		
		fl.DeleteBefore("d")
		assert.Equal(t, "a d", fl.Read())
	})

	t.Run("DeleteAfter", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("b")
		fl.PushTail("c")
		fl.PushTail("d")
		
		fl.DeleteAfter("b")
		assert.Equal(t, "a b d", fl.Read())
		
		fl.DeleteAfter("a")
		assert.Equal(t, "a d", fl.Read())
		
		fl.DeleteAfter("d")
		assert.Equal(t, "a d", fl.Read())
	})

	t.Run("DeleteValue", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("b")
		fl.PushTail("c")
		fl.PushTail("d")
		
		fl.DeleteValue("b")
		assert.Equal(t, "a c d", fl.Read())
		
		fl.DeleteValue("a")
		assert.Equal(t, "c d", fl.Read())
		
		fl.DeleteValue("d")
		assert.Equal(t, "c", fl.Read())
		
		fl.DeleteValue("x")
		assert.Equal(t, "c", fl.Read())
	})

	t.Run("Find", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("b")
		fl.PushTail("c")
		
		assert.True(t, fl.Find("a"))
		assert.True(t, fl.Find("b"))
		assert.True(t, fl.Find("c"))
		assert.False(t, fl.Find("x"))
		assert.False(t, fl.Find(""))
	})

	t.Run("ReadReverse", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("a")
		fl.PushTail("b")
		fl.PushTail("c")
		
		assert.Equal(t, "c b a", fl.ReadReverse())
	})

	t.Run("SerializeBinary", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("hello")
		fl.PushTail("world")
		fl.PushTail("test")
		
		var buf bytes.Buffer
		err := fl.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newFl := NewForwardList("new")
		err = newFl.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, fl.Read(), newFl.Read())
	})

	t.Run("SerializeText", func(t *testing.T) {
		fl := NewForwardList("test")
		fl.PushTail("hello")
		fl.PushTail("world")
		fl.PushTail("test")
		
		var buf bytes.Buffer
		err := fl.SerializeText(&buf)
		require.NoError(t, err)
		
		newFl := NewForwardList("new")
		err = newFl.DeserializeText(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, fl.Read(), newFl.Read())
	})
}
