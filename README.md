# sevenzip-node

`sevenzip-node` is a wrapper around `7z.dll` or `7za.dll`. The goal is to provide a unique and an easy way to compress and extract multiple archives format.

## Motivation
I cannot find any NPM module that can compress in LZMA easily. They usally bind the command line without any modification to understand it better.

## Dependencies
I use [bit7z](https://github.com/rikyoz/bit7z) to wrap the DLL. It provides an easy way to do it. 
Currently the library is only available for Windows since bit7z is only available for it.
I will work on a current implementation for Linux through p7zip.

## Installation
```
npm install sevenzip-node
```

## Usage
Simply include the module with this code:
```
const sevenZip = require('sevenzip-node');
```

## Documentation
### Methods
- compress
- extract

### Objects
- CompressionLevel
