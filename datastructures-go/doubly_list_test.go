package datastructures

import (
	"bytes"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestDoublyList(t *testing.T) {
	t.Run("PushHead and Read", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushHead("c")
		dl.PushHead("b")
		dl.PushHead("a")
		assert.Equal(t, "a b c", dl.Read())
		assert.Equal(t, "c b a", dl.ReadReverse())
	})

	t.Run("PushTail and Read", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("b")
		dl.PushTail("c")
		assert.Equal(t, "a b c", dl.Read())
		assert.Equal(t, "c b a", dl.ReadReverse())
	})

	t.Run("PushBefore", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("c")
		
		dl.PushBefore("c", "b")
		assert.Equal(t, "a b c", dl.Read())
		assert.Equal(t, "c b a", dl.ReadReverse())
		
		dl.PushBefore("a", "x")
		assert.Equal(t, "x a b c", dl.Read())
		assert.Equal(t, "c b a x", dl.ReadReverse())
	})

	t.Run("PushAfter", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("c")
		
		dl.PushAfter("a", "b")
		assert.Equal(t, "a b c", dl.Read())
		assert.Equal(t, "c b a", dl.ReadReverse())
		
		dl.PushAfter("c", "d")
		assert.Equal(t, "a b c d", dl.Read())
		assert.Equal(t, "d c b a", dl.ReadReverse())
	})

	t.Run("DeleteHead", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("b")
		dl.PushTail("c")
		
		dl.DeleteHead()
		assert.Equal(t, "b c", dl.Read())
		assert.Equal(t, "c b", dl.ReadReverse())
		
		dl.DeleteHead()
		assert.Equal(t, "c", dl.Read())
		
		dl.DeleteHead()
		assert.Equal(t, "", dl.Read())
		
		dl.DeleteHead()
		assert.Equal(t, "", dl.Read())
	})

	t.Run("DeleteTail", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("b")
		dl.PushTail("c")
		
		dl.DeleteTail()
		assert.Equal(t, "a b", dl.Read())
		assert.Equal(t, "b a", dl.ReadReverse())
		
		dl.DeleteTail()
		assert.Equal(t, "a", dl.Read())
		
		dl.DeleteTail()
		assert.Equal(t, "", dl.Read())
		
		dl.DeleteTail()
		assert.Equal(t, "", dl.Read())
	})

	t.Run("DeleteBefore", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("b")
		dl.PushTail("c")
		dl.PushTail("d")
		
		dl.DeleteBefore("c")
		assert.Equal(t, "a c d", dl.Read())
		assert.Equal(t, "d c a", dl.ReadReverse())
		
		dl.DeleteBefore("a")
		assert.Equal(t, "a c d", dl.Read())
		
		dl.DeleteBefore("d")
		assert.Equal(t, "a d", dl.Read())
		assert.Equal(t, "d a", dl.ReadReverse())
	})

	t.Run("DeleteAfter", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("b")
		dl.PushTail("c")
		dl.PushTail("d")
		
		dl.DeleteAfter("b")
		assert.Equal(t, "a b d", dl.Read())
		assert.Equal(t, "d b a", dl.ReadReverse())
		
		dl.DeleteAfter("a")
		assert.Equal(t, "a d", dl.Read())
		assert.Equal(t, "d a", dl.ReadReverse())
		
		dl.DeleteAfter("d")
		assert.Equal(t, "a d", dl.Read())
	})

	t.Run("DeleteValue", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("b")
		dl.PushTail("c")
		dl.PushTail("d")
		
		dl.DeleteValue("b")
		assert.Equal(t, "a c d", dl.Read())
		assert.Equal(t, "d c a", dl.ReadReverse())
		
		dl.DeleteValue("a")
		assert.Equal(t, "c d", dl.Read())
		
		dl.DeleteValue("d")
		assert.Equal(t, "c", dl.Read())
		
		dl.DeleteValue("x")
		assert.Equal(t, "c", dl.Read())
	})

	t.Run("Find", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("b")
		dl.PushTail("c")
		
		assert.True(t, dl.Find("a"))
		assert.True(t, dl.Find("b"))
		assert.True(t, dl.Find("c"))
		assert.False(t, dl.Find("x"))
		assert.False(t, dl.Find(""))
	})

	t.Run("SerializeBinary", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("hello")
		dl.PushTail("world")
		dl.PushTail("test")
		
		var buf bytes.Buffer
		err := dl.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newDl := NewDoublyList("new")
		err = newDl.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, dl.Read(), newDl.Read())
		assert.Equal(t, dl.ReadReverse(), newDl.ReadReverse())
	})

	t.Run("SerializeText", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("hello")
		dl.PushTail("world")
		dl.PushTail("test")
		
		var buf bytes.Buffer
		err := dl.SerializeText(&buf)
		require.NoError(t, err)
		
		newDl := NewDoublyList("new")
		err = newDl.DeserializeText(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, dl.Read(), newDl.Read())
		assert.Equal(t, dl.ReadReverse(), newDl.ReadReverse())
	})

	t.Run("Clear via deserialize", func(t *testing.T) {
		dl := NewDoublyList("test")
		dl.PushTail("a")
		dl.PushTail("b")
		dl.PushTail("c")
		
		assert.Equal(t, "a b c", dl.Read())
		
		var buf bytes.Buffer
		dl.SerializeBinary(&buf)
		
		dl.DeserializeBinary(&buf)
		assert.Equal(t, "a b c", dl.Read())
	})
}
