# sevenzip-node

`sevenzip-node` is a wrapper around `7z.dll` or `7za.dll` for **Windows** and `7zz` for **Linux** and **Mac**. The goal is to provide a unique and an easy way to compress and extract multiple archives format.

## Motivation
I cannot find any NPM module that can compress in LZMA easily. They usally bind the command line without any modification to understand it better.

## Dependencies
The native module is build by using [bit7z](https://github.com/rikyoz/bit7z) to wrap the DLL. **The native module is only available for Windows since bit7z is only available for it.**.

The platform support for **Linux** and **Mac** is done by using 7zz. It's a command line version of 7z published by the author.

## Installation
```
npm install sevenzip-node
```

## Usage
Simply include the module with this code:
```js
const sevenZip = require('sevenzip-node');
```

## Documentation
### Methods
- [compress](https://github.com/SteezCram/sevenzip-node/blob/main/docs/compress.md)
- [extract](https://github.com/SteezCram/sevenzip-node/blob/main/docs/extract.md)

### Objects
- [CompressionLevel](https://github.com/SteezCram/sevenzip-node/blob/main/docs/CompressionLevel.md)
