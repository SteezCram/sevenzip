# sevenzip

`sevenzip` is a wrapper around `7za.exe` for **Windows**, **Linux** and **Mac**. The goal is to provide a unique and an easy way to compress and extract multiple archives format.

## Motivation
I cannot find any NPM module that can compress in LZMA easily. They usally bind the command line without any modification to understand it better.

## Dependencies
The platform support for **Windows**, **Linux** and **Mac** is done by using 7za. It's a command line version of 7z published by the author.

## Installation
```
npm install @steezcram/sevenzip --save
```

## Usage
Simply include the module with this code:
```js
const sevenzip = require('@steezcram/sevenzip');
```

## Documentation
### Methods
- [compress](https://github.com/SteezCram/sevenzip/blob/main/docs/compress.md)
- [extract](https://github.com/SteezCram/sevenzip/blob/main/docs/extract.md)

### Objects
- [CompressionLevel](https://github.com/SteezCram/sevenzip/blob/main/docs/CompressionLevel.md)
