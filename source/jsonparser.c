//warning this code is vibe coded

#include "jsonparser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ─────────────────────────────────────────────────────────────────────────
 * Bounded cursor — all reads are guarded by [p, end).
 * ───────────────────────────────────────────────────────────────────────── */
typedef struct { const char *p; const char *end; } Cursor;

static int    cur_ok  (Cursor c) { return c.p < c.end; }
static char   cur_ch  (Cursor c) { return cur_ok(c) ? *c.p : '\0'; }
static Cursor cur_adv (Cursor c) { if (cur_ok(c)) c.p++; return c; }   /* bounded */

static Cursor skip_ws(Cursor c) {
    while (cur_ok(c)) {
        char ch = *c.p;
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') break;
        c.p++;
    }
    return c;
}

/* ─────────────────────────────────────────────────────────────────────────
 * Skippers — advance past a complete JSON value of each type.
 * ───────────────────────────────────────────────────────────────────────── */
static Cursor skip_value(Cursor c);

/* c must point at the opening '"' */
static Cursor skip_string(Cursor c) {
    c = cur_adv(c);                        /* consume opening '"' */
    while (cur_ok(c) && cur_ch(c) != '"') {
        if (cur_ch(c) == '\\') c = cur_adv(c);   /* skip escape prefix */
        c = cur_adv(c);
    }
    return cur_adv(c);                     /* consume closing '"' */
}

/* c must point at '{' */
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

/* c must point at '[' */
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

/* ─────────────────────────────────────────────────────────────────────────
 * find_key()
 * Walk the JSON object at c and return a cursor pointing at the *value*
 * associated with `key` (byte-length `klen`).
 * Returns {NULL, end} if not found or if c is not at an object.
 * ───────────────────────────────────────────────────────────────────────── */
static Cursor find_key(Cursor c, const char *key, size_t klen) {
    const Cursor not_found = { NULL, c.end };

    c = skip_ws(c);
    if (cur_ch(c) != '{') return not_found;
    c = cur_adv(c);

    while (cur_ok(c)) {
        c = skip_ws(c);

        if (cur_ch(c) == '}') return not_found;
        if (cur_ch(c) != '"') return not_found;    /* malformed */

        const char *key_start = c.p + 1;           /* byte after opening '"' */
        c = skip_string(c);
        /* after skip_string, c.p is one past the closing '"' */
        size_t key_len = (size_t)(c.p - 1 - key_start);

        c = skip_ws(c);
        if (cur_ch(c) != ':') return not_found;
        c = cur_adv(c);
        c = skip_ws(c);

        /* key match — return cursor pointing at the value */
        if (key_len == klen && memcmp(key_start, key, klen) == 0)
            return c;

        c = skip_value(c);
        c = skip_ws(c);
        if (cur_ch(c) == ',') { c = cur_adv(c); continue; }
        break;
    }
    return not_found;
}

/* ─────────────────────────────────────────────────────────────────────────
 * Extractors
 * ───────────────────────────────────────────────────────────────────────── */

/* Decode a JSON string, handling backslash escapes.
 * \uXXXX is passed through verbatim as 6 ASCII bytes. */
static char *extract_string(Cursor c) {
    c = skip_ws(c);
    if (cur_ch(c) != '"') return NULL;
    c = cur_adv(c);

    /* Pass 1: measure decoded length */
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

    /* Pass 2: decode */
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

/* Copy a number, boolean, or null verbatim. */
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

/* ─────────────────────────────────────────────────────────────────────────
 * Public API
 * ───────────────────────────────────────────────────────────────────────── */

void parasejson(const char *input, char **output, const char *element) {
    if (!input || !output || !element) return;
    *output = NULL;

    /* ── 1. Locate the first '{', capped at 1 MiB ── */
    const char *p   = input;
    const char *cap = input + 0x100000;
    while (p < cap && *p != '{') p++;
    if (p >= cap) {
        fprintf(stderr, "parasejson: no '{' found within 1 MiB\n");
        return;
    }

    /* ── 2. Build a bounded cursor over the whole remaining window ──
       We use `cap` rather than trying to find the exact closing '}':
       every inner skipper is bounded, so we can never over-read.     */
    Cursor c = { p, cap };

    /* ── 3. Walk the dot-separated path without strtok ──
       We tokenise manually so there is no shared mutable state and
       no dependency on strtok_r availability.                        */
    const char *seg     = element;
    const char *seg_end;

    while (*seg) {
        seg_end = seg;
        while (*seg_end && *seg_end != '.') seg_end++;

        size_t seg_len = (size_t)(seg_end - seg);
        if (seg_len == 0) {
            seg = (*seg_end == '.') ? seg_end + 1 : seg_end;
            continue;
        }

        if (cur_ch(c) != '{') {
            fprintf(stderr, "parasejson: expected object at key '%.*s'\n",
                    (int)seg_len, seg);
            return;
        }

        c = find_key(c, seg, seg_len);
        if (!c.p) {
            fprintf(stderr, "parasejson: key '%.*s' not found\n",
                    (int)seg_len, seg);
            return;
        }
        c = skip_ws(c);

        seg = (*seg_end == '.') ? seg_end + 1 : seg_end;
    }

    /* ── 4. Extract the final value ── */
    *output = (cur_ch(c) == '"') ? extract_string(c) : extract_primitive(c);
}