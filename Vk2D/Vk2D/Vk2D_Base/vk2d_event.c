#include "vk2d_event.h"

#include <stdlib.h>
#include <string.h>

#define EVENT_TABLE_SIZE 30
#define EVENT_NAME_MAX 32

#include <Vk2D/Vk2D_Base/vk2d_list.h>
#include <Vk2D/Vk2D_Base/vk2d_log.h>

typedef struct vk2d_event_type vk2d_event_type;
struct vk2d_event_type
{
    char name[EVENT_NAME_MAX];
    vk2d_list *listeners;
    vk2d_event_type *next;
};

static vk2d_event_type event_table[EVENT_TABLE_SIZE] = {0};

static i32 hash(const char* name)
{
    size_t len = strnlen(name, EVENT_NAME_MAX);
    u32 val = 0;

    for (i32 i = 0; i < len; i++)
        val += name[i] * 23 * i;
    
    val %= EVENT_TABLE_SIZE;

    return val;
}

static vk2d_event_type* get_event_type(const char* name)
{
    i32 index = hash(name);
    vk2d_event_type* tmp = &event_table[index];

    while (NULL != tmp && 0 != strncmp(tmp->name, name, EVENT_NAME_MAX))
        tmp = tmp->next;

    return tmp;
}

void vk2d_event_emit(const char* name, int lowParam, int highParam)
{
    i32 i = 0;
    vk2d_event_type* t = get_event_type(name);
    vk2d_event_callback cb = NULL;
    vk2d_event_callback* list = NULL;

    if (t == NULL)
    {
        vk2d_log_fatal("Vk2D Event", "failed to emit non-existing event type");
        return;
    }

    list = t->listeners->items;

    for (i = 0; i < t->listeners->count; ++i) {
        cb = *(list[i]);
        cb(lowParam, highParam);
    }
}

void vk2d_event_bind(const char* name, vk2d_event_callback callback)
{
    vk2d_event_type* t = get_event_type(name);
    u32 index = hash(name);

    if (NULL == t && NULL == event_table[index].listeners) {
        vk2d_list* listeners = vk2d_list_new(sizeof(vk2d_event_callback));
        t = &event_table[index];

        t->listeners = listeners;
        strncpy_s(t->name, EVENT_NAME_MAX, name, EVENT_NAME_MAX);
        return;
    }

    if (NULL == t && NULL != event_table[index].listeners) {
        vk2d_list* listeners = vk2d_list_new(sizeof(vk2d_event_callback));
        vk2d_event_type* tail = &event_table[index];
        vk2d_new(t, sizeof(vk2d_event_type));

        while (NULL != tail->next)
            tail = tail->next;

        if (t)
        {
            t->listeners = listeners;
            strncpy_s(t->name, EVENT_NAME_MAX, name, EVENT_NAME_MAX);
        }
    }

    if (t)
    {
        vk2d_list_add(t->listeners, &callback);
    }
}