//warning this code is vibe coded
#ifndef JSONPARSER_H
#define JSONPARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * parasejson()
 *
 * Navigate a JSON object using a dot-separated path and extract the value
 * at that location as a null-terminated string.
 *
 *   parasejson(json, &out, "data.Media.title.romaji") -> "Bakemonogatari"
 *   parasejson(json, &out, "data.Media.episodes")     -> "15"
 *
 *   output  - heap-allocated result. Caller must free().
 *             NULL on failure or missing key.
 *   element - dot-separated path, e.g. "data.Media.title.romaji"
 */
void parasejson(const char *input, char **output, const char *element);














/*
 * paraselistjson()
 *
 * Navigate to an array via a dot-separated path and return every element
 * as a heap-allocated raw-JSON string.
 *
 * Intermediate arrays are traversed automatically — no index needed:
 *   paraselistjson(json, &items, &count,
 *                  "data.MediaListCollection.lists.entries");
 *   -> items[0] = {"media":{"title":{"romaji":"Tokyo Magnitude 8.0"},...},...}
 *   -> items[1] = {"media":{"title":{"romaji":"Azumanga Daiou ..."},...},...}
 *
 * Use parasejson() on each item to read fields:
 *   parasejson(items[i], &title, "media.title.romaji");
 *   if (!title) parasejson(items[i], &title, "media.title.english");
 *   parasejson(items[i], &episodes, "media.episodes");
 *   parasejson(items[i], &progress, "progress");
 *   parasejson(items[i], &score,    "score");
 *
 * Free when done:
 *   for (int i = 0; i < count; i++) free(items[i]);
 *   free(items);
 */
void paraselistjson(const char *input, char ***items, int *count, const char *element);

#ifdef __cplusplus
}
#endif

#endif /* JSONPARSER_H */