package datastructures

import (
	"bytes"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestStack(t *testing.T) {
	t.Run("Push and Pop", func(t *testing.T) {
		s := NewStack("test")
		assert.True(t, s.IsEmpty())
		
		s.Push("a")
		assert.False(t, s.IsEmpty())
		assert.Equal(t, "a", s.Peek())
		
		s.Push("b")
		s.Push("c")
		assert.Equal(t, "c", s.Peek())
		
		assert.Equal(t, "c", s.Pop())
		assert.Equal(t, "b", s.Pop())
		assert.Equal(t, "a", s.Pop())
		assert.Equal(t, "", s.Pop())
		assert.True(t, s.IsEmpty())
	})

	t.Run("Read", func(t *testing.T) {
		s := NewStack("test")
		s.Push("a")
		s.Push("b")
		s.Push("c")
		assert.Equal(t, "c b a", s.Read())
		
		s.Pop()
		assert.Equal(t, "b a", s.Read())
		
		s.Push("d")
		assert.Equal(t, "d b a", s.Read())
	})

	t.Run("SerializeBinary", func(t *testing.T) {
		s := NewStack("test")
		s.Push("hello")
		s.Push("world")
		s.Push("test")
		
		assert.Equal(t, "test world hello", s.Read())
		
		var buf bytes.Buffer
		err := s.SerializeBinary(&buf)
		require.NoError(t, err)
		
		newStack := NewStack("new")
		err = newStack.DeserializeBinary(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, "test world hello", newStack.Read())
		assert.Equal(t, "test", newStack.Pop())
		assert.Equal(t, "world", newStack.Pop())
		assert.Equal(t, "hello", newStack.Pop())
	})

	t.Run("SerializeText", func(t *testing.T) {
		s := NewStack("test")
		s.Push("hello")
		s.Push("world")
		s.Push("test")
		
		assert.Equal(t, "test world hello", s.Read())
		
		var buf bytes.Buffer
		err := s.SerializeText(&buf)
		require.NoError(t, err)
		
		newStack := NewStack("new")
		err = newStack.DeserializeText(&buf)
		require.NoError(t, err)
		
		assert.Equal(t, "test world hello", newStack.Read())
		assert.Equal(t, "test", newStack.Pop())
		assert.Equal(t, "world", newStack.Pop())
		assert.Equal(t, "hello", newStack.Pop())
	})
}
