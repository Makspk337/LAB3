package datastructures

import (
	"bytes"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestQueue(t *testing.T) {
	t.Run("Push and Pop", func(t *testing.T) {
		q := NewQueue("test")
		assert.True(t, q.IsEmpty())
		
		q.Push("a")
		assert.False(t, q.IsEmpty())
		assert.Equal(t, "a", q.Peek())
		
		q.Push("b")
		q.Push("c")
		assert.Equal(t, "a", q.Peek())
		
		assert.Equal(t, "a", q.Pop())
		assert.Equal(t, "b", q.Pop())
		assert.Equal(t, "c", q.Pop())
		assert.Equal(t, "", q.Pop())
		assert.True(t, q.IsEmpty())
	})

	t.Run("Read", func(t *testing.T) {
		q := NewQueue("test")
		q.Push("a")
		q.Push("b")
		q.Push("c")
		assert.Equal(t, "a b c", q.Read())
		
		q.Pop()
		assert.Equal(t, "b c", q.Read())
		
		q.Push("d")
		assert.Equal(t, "b c d", q.Read())
	})

	t.Run("SerializeBinary", func(t *testing.T) {
		q := NewQueue("test")
		q.Push("hello")
		q.Push("world")
		q.Push("test")
		
		var buf bytes.Buffer
		err := q.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newQueue := NewQueue("new")
		err = newQueue.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, q.Read(), newQueue.Read())
		assert.Equal(t, q.Pop(), newQueue.Pop())
		assert.Equal(t, q.Pop(), newQueue.Pop())
		assert.Equal(t, q.Pop(), newQueue.Pop())
	})

	t.Run("SerializeText", func(t *testing.T) {
		q := NewQueue("test")
		q.Push("hello")
		q.Push("world")
		q.Push("test")
		
		var buf bytes.Buffer
		err := q.SerializeText(&buf)
		require.NoError(t, err)
		
		newQueue := NewQueue("new")
		err = newQueue.DeserializeText(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, q.Read(), newQueue.Read())
		assert.Equal(t, q.Pop(), newQueue.Pop())
		assert.Equal(t, q.Pop(), newQueue.Pop())
		assert.Equal(t, q.Pop(), newQueue.Pop())
	})

	t.Run("Empty Queue", func(t *testing.T) {
		q := NewQueue("test")
		assert.Equal(t, "", q.Peek())
		assert.Equal(t, "", q.Pop())
		assert.Equal(t, "", q.Read())
		assert.True(t, q.IsEmpty())
	})
}
