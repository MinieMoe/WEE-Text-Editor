#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

// The gap buffer defines an efficient insertion and deletion data structure.
typedef struct {
    char* data;
    int size;
    int insert_position;
    int second_position;
} GapBuffer;

#define DEFAULT_GAP_SIZE 4

// Create and return an empty gap buffer
// The gap size is DEFAULT_GAP_SIZE
GapBuffer* gap_create();

// Move the cursor in the gap buffer, copying data as necessary
// to each side of the gap
void gap_set_insert_position(GapBuffer* gbuf, int new_position);

// Insert a character at the insert position. This grows
// the gap buffer if necessary.
// For example, if the gbuf is
//    To be[    ]or
// then inserting 'z' makes the gbuf
//    To bez[   ]or
void gap_insert_char(GapBuffer* gbuf, char c);

// Insert count chars of a string at the cursor position. This grows
// the gap buffer if necessary.
void gap_insert_string(GapBuffer* gbuf, int count, const char* s);

// Remove a character behind the insert position
// For example, if the gbuf is
//    To be[    ]or
// then gbuf would change and become
//    To b[     ]or
void gap_remove_char(GapBuffer* gbuf);

// Get the gap buffer as a null terminated string.
// The memory for the string is allocated and must be freed
// by the receiving caller.
// For example, if the gbuf is
//    To be[    ]or
// then the return value would be "To be or"
char* gap_to_string(GapBuffer* gbuf);

// Length of the valid string in the gap buffer
// For example, if the gbuf is
//    To be[    ]or
// Then the length would be strlen("To be or") = 8
int gap_length(GapBuffer* gbuf);

// Break this gap buffer into two at the insert position, returning
// a new gap buffer for the second half. For example, if gbuf is
//     To be or[   ]not to be
// then breaking it apart would cause gbuf to become
//     To be or[             ]
// and the return value would be the new gbuf
//     not to be[     ]
GapBuffer* gap_break(GapBuffer* gbuf);

// Print a representation of a gap buffer.
// This is very useful for debugging
void gap_print(GapBuffer* gbuf);

#endif // GAP_BUFFER_H
