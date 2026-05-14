//warning this code is vibe coded

#include "jsonparser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct { const char *p; const char *end; } Cursor;

static int    cur_ok  (Cursor c) { return c.p < c.end; }
static char   cur_ch  (Cursor c) { return cur_ok(c) ? *c.p : '\0'; }
static Cursor cur_adv (Cursor c) { if (cur_ok(c)) c.p++; return c; }

static Cursor skip_ws(Cursor c) {
    while (cur_ok(c)) {
        char ch = *c.p;
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') break;
        c.p++;
    }
    return c;
}

static Cursor skip_value(Cursor c);

static Cursor skip_string(Cursor c) {
    c = cur_adv(c);
    while (cur_ok(c) && cur_ch(c) != '"') {
        if (cur_ch(c) == '\\') c = cur_adv(c);
        c = cur_adv(c);
    }
    return cur_adv(c);
}

static Cursor skip_object(Cursor c) {
    c = cur_adv(c);
    c = skip_ws(c);
    if (cur_ch(c) == '}') return cur_adv(c);
    while (cur_ok(c)) {
        c = skip_ws(c);
        if (cur_ch(c) == '"') c = skip_string(c);
        c = skip_ws(c);
        if (cur_ch(c) == ':') c = cur_adv(c);
        c = skip_ws(c);
        c = skip_value(c);
        c = skip_ws(c);
        if (cur_ch(c) == ',') { c = cur_adv(c); continue; }
        if (cur_ch(c) == '}') return cur_adv(c);
        break;
    }
    return c;
}

static Cursor skip_array(Cursor c) {
    c = cur_adv(c);
    c = skip_ws(c);
    if (cur_ch(c) == ']') return cur_adv(c);
    while (cur_ok(c)) {
        c = skip_ws(c);
        c = skip_value(c);
        c = skip_ws(c);
        if (cur_ch(c) == ',') { c = cur_adv(c); continue; }
        if (cur_ch(c) == ']') return cur_adv(c);
        break;
    }
    return c;
}

static Cursor skip_primitive(Cursor c) {
    while (cur_ok(c)) {
        char ch = cur_ch(c);
        if (ch==',' || ch=='}' || ch==']' ||
            ch==' ' || ch=='\t' || ch=='\n' || ch=='\r') break;
        c.p++;
    }
    return c;
}

static Cursor skip_value(Cursor c) {
    c = skip_ws(c);
    switch (cur_ch(c)) {
        case '"': return skip_string(c);
        case '{': return skip_object(c);
        case '[': return skip_array(c);
        default:  return skip_primitive(c);
    }
}

static Cursor find_key(Cursor c, const char *key, size_t klen) {
    const Cursor not_found = { NULL, c.end };
    c = skip_ws(c);
    if (cur_ch(c) != '{') return not_found;
    c = cur_adv(c);
    while (cur_ok(c)) {
        c = skip_ws(c);
        if (cur_ch(c) == '}') return not_found;
        if (cur_ch(c) != '"') return not_found;
        const char *key_start = c.p + 1;
        c = skip_string(c);
        size_t key_len = (size_t)(c.p - 1 - key_start);
        c = skip_ws(c);
        if (cur_ch(c) != ':') return not_found;
        c = cur_adv(c);
        c = skip_ws(c);
        if (key_len == klen && memcmp(key_start, key, klen) == 0)
            return c;
        c = skip_value(c);
        c = skip_ws(c);
        if (cur_ch(c) == ',') { c = cur_adv(c); continue; }
        break;
    }
    return not_found;
}

static char *extract_string(Cursor c) {
    c = skip_ws(c);
    if (cur_ch(c) != '"') return NULL;
    c = cur_adv(c);
    size_t len = 0;
    Cursor scan = c;
    while (cur_ok(scan) && cur_ch(scan) != '"') {
        if (cur_ch(scan) == '\\') {
            scan = cur_adv(scan);
            if (cur_ch(scan) == 'u') {
                len += 6;
                int d = 4; while (d-- && cur_ok(scan)) scan = cur_adv(scan);
            } else { len++; }
        } else { len++; }
        scan = cur_adv(scan);
    }
    char *out = malloc(len + 1);
    if (!out) return NULL;
    size_t i = 0;
    while (cur_ok(c) && cur_ch(c) != '"') {
        if (cur_ch(c) == '\\') {
            c = cur_adv(c);
            switch (cur_ch(c)) {
                case '"':  out[i++] = '"';  break;
                case '\\': out[i++] = '\\'; break;
                case '/':  out[i++] = '/';  break;
                case 'n':  out[i++] = '\n'; break;
                case 'r':  out[i++] = '\r'; break;
                case 't':  out[i++] = '\t'; break;
                case 'b':  out[i++] = '\b'; break;
                case 'f':  out[i++] = '\f'; break;
                case 'u':
                    out[i++] = '\\'; out[i++] = 'u';
                    { int d = 4; while (d-- && cur_ok(c)) { c = cur_adv(c); out[i++] = cur_ch(c); } }
                    break;
                default: out[i++] = cur_ch(c); break;
            }
        } else {
            out[i++] = cur_ch(c);
        }
        c = cur_adv(c);
    }
    out[i] = '\0';
    return out;
}

static char *extract_primitive(Cursor c) {
    c = skip_ws(c);
    const char *start = c.p;
    c = skip_primitive(c);
    size_t len = (size_t)(c.p - start);
    if (len == 0) return NULL;
    char *out = malloc(len + 1);
    if (!out) return NULL;
    memcpy(out, start, len);
    out[len] = '\0';
    return out;
}

static char *extract_raw(Cursor c) {
    c = skip_ws(c);
    const char *start = c.p;
    c = skip_value(c);
    size_t len = (size_t)(c.p - start);
    if (len == 0) return NULL;
    char *out = malloc(len + 1);
    if (!out) return NULL;
    memcpy(out, start, len);
    out[len] = '\0';
    return out;
}

/* ── StrVec ── */
typedef struct { char **data; int len, cap, oom; } StrVec;
static void sv_init(StrVec *v) { v->data=NULL; v->len=v->cap=v->oom=0; }
static void sv_push(StrVec *v, char *s) {
    if (v->oom) { free(s); return; }
    if (v->len >= v->cap) {
        int nc = v->cap ? v->cap*2 : 16;
        char **t = realloc(v->data, (size_t)nc * sizeof(char *));
        if (!t) { v->oom=1; free(s); return; }
        v->data=t; v->cap=nc;
    }
    v->data[v->len++] = s;
}
static void sv_free(StrVec *v) {
    for (int i=0; i<v->len; i++) free(v->data[i]);
    free(v->data); sv_init(v);
}

/* ── nav_collect / foreach_elem ── */
static void nav_collect(Cursor c, const char *seg, StrVec *out);

static void foreach_elem(Cursor c, const char *seg, StrVec *out) {
    c = skip_ws(c);
    if (cur_ch(c) != '[') return;
    c = cur_adv(c);
    c = skip_ws(c);
    if (cur_ch(c) == ']') return;
    while (cur_ok(c)) {
        c = skip_ws(c);
        if (cur_ch(c) == ']') break;
        nav_collect(c, seg, out);
        if (out->oom) return;
        c = skip_value(c);
        c = skip_ws(c);
        if (cur_ch(c) == ',') { c = cur_adv(c); continue; }
        if (cur_ch(c) == ']') break;
        break;
    }
}

static void nav_collect(Cursor c, const char *seg, StrVec *out) {
    c = skip_ws(c);
    if (!cur_ok(c) || out->oom) return;
    if (!seg || seg[0] == '\0') {
        if (cur_ch(c) == '[') foreach_elem(c, "", out);
        else { char *r = extract_raw(c); if (r) sv_push(out, r); }
        return;
    }
    const char *seg_end = seg;
    while (*seg_end && *seg_end != '.') seg_end++;
    size_t      seg_len = (size_t)(seg_end - seg);
    const char *rest    = (*seg_end == '.') ? seg_end+1 : seg_end;
    if (cur_ch(c) == '[') { foreach_elem(c, seg, out); return; }
    if (cur_ch(c) != '{') return;
    Cursor val = find_key(c, seg, seg_len);
    if (!val.p) return;
    nav_collect(val, rest, out);
}

/* ── Public API ── */

void parasejson(const char *input, char **output, const char *element) {
    if (!input || !output || !element) return;
    *output = NULL;

    const char *p   = input;
    const char *end = input + strlen(input);
    while (p < end && *p != '{') p++;
    if (p >= end) return;

    Cursor c = { p, end };
    const char *seg = element;
    const char *seg_end;

    while (*seg) {
        seg_end = seg;
        while (*seg_end && *seg_end != '.') seg_end++;
        size_t seg_len = (size_t)(seg_end - seg);
        if (seg_len == 0) { seg = (*seg_end=='.') ? seg_end+1 : seg_end; continue; }
        if (cur_ch(c) != '{') return;
        c = find_key(c, seg, seg_len);
        if (!c.p) return;
        c = skip_ws(c);
        seg = (*seg_end == '.') ? seg_end+1 : seg_end;
    }

    *output = (cur_ch(c) == '"') ? extract_string(c) : extract_primitive(c);
}

void paraselistjson(const char *input, char ***items, int *count, const char *element) {
    if (!input || !items || !count) return;
    *items = NULL; *count = 0;

    const char *p   = input;
    const char *end = input + strlen(input);
    while (p < end && *p != '{') p++;
    if (p >= end) return;

    Cursor c = { p, end };
    StrVec out; sv_init(&out);
    nav_collect(c, (element && element[0]) ? element : "", &out);
    if (out.oom) { sv_free(&out); return; }
    *items = out.data;
    *count = out.len;
}