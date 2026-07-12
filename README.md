# profanityd

A lightweight profanity filtering daemon written in C++20 using Drogon.

It was written as a local replacement for the [neutrino API bad word filter](https://www.neutrinoapi.com/api/bad-word-filter/) used by my bots. It runs entirely offline and loads its dictionaries into memory on startup.

## API

```
POST /bad-word-filter
```

```json
{
    "content": "Some text to check",
    "censor-character": "#",
    "languages": ["en"]
}
```

`languages` is optional. If omitted all loaded dictionaries are searched.

The response format is compatible with the neutrino bad word filter endpoint. Authentication properties (`user-id` and `api-key`) are accepted for compatibility but ignored.

This daemon is intended to run on localhost behind trusted applications. It is not designed to be exposed directly to the internet.

## Dictionaries

Dictionary files are loaded from the `dictionaries` directory at startup. You must source your own bad word dictionaries. The expected format is one UTF-8 word per line.

## Reloading

To reload the dictionaries without restarting the daemon, send the daemon a `SIGHUP`.

## Comparison to Neutrino

| Technique                | Neutrino                        | profanityd |
|--------------------------|---------------------------------|-----------|
| ASCII punctuation        | ✅                              | ✅        |
| Slash separation         | ❌                              | ✅        |
| Emoji separation         | ❌                              | ✅        |
| Zalgo                    | ❌                              | ✅        |
| Mathematical alphabets   | ❌                              | ✅        |
| Accent flattening        | Detection appears inconsistent  | ✅        |
| Repeated letters         | ✅                              | ✅        |

