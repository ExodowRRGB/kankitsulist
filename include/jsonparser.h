//warning this code is vibe coded
#ifndef JSONPARSER_H
#define JSONPARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * jsonparser.h — zero-dependency, path-driven JSON field extractor
 *
 * parasejson() navigates a JSON structure using a dot-separated key path
 * and extracts the value at that location as a null-terminated string.
 *
 * Examples:
 *   parasejson(json, &out, "data.Media.id")              -> "5081"
 *   parasejson(json, &out, "data.Media.title.romaji")    -> "Bakemonogatari"
 *   parasejson(json, &out, "data.Media.status")          -> "FINISHED"
 *   parasejson(json, &out, "data.Media.episodes")        -> "15"
 *
 * parasejson()
 *
 *   input   - null-terminated string containing a JSON object somewhere
 *             inside it (leading garbage, HTTP headers, BOM bytes etc.
 *             before the first '{' are skipped automatically)
 *   output  - receives a heap-allocated, null-terminated result string.
 *             THE CALLER IS RESPONSIBLE FOR free(*output).
 *             Set to NULL on any failure or if the path is not found.
 *   element - dot-separated key path, e.g. "data.Media.title.romaji"
 */
void parasejson(const char *input, char **output, const char *element);

#ifdef __cplusplus
}
#endif

#endif /* JSONPARSER_H */
