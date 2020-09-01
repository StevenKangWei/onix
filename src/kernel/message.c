#include <onix/message.h>
#include <onix/string.h>

void reset_message(Message *message)
{
    memset(message, 0, sizeof(Message));
}
