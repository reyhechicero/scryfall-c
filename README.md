# scryfall-c
Search cards with the Scryfall REST API in C using libcurl

# WARNING
This is extremely unfinished. It is unable to search cards that have more than two "cards" on them such as `Fire // Ice` and `Shatterskull Smashing // Shatterskull, the Hammer Pass`.

# Compiling
```
gcc main.c -lcurl -ljansson -o scryfall
```
