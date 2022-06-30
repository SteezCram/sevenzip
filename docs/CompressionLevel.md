# CompressionLevel

Enum object like to easily access all the compression level available. Available level are:
- `NONE` _(0)_
- `FASTEST` _(1)_
- `FAST` _(3)_
- `NORMAL` _(5)_
- `MAX` _(7)_
- `ULTRA` _(9)_

_Note: all the level different from this enum fallback to the normal level._

## Exemple
```js
const sevenzip = require('@steezcram/sevenzip');

const ultraLevel = sevenzip.CompressionLevel.ULTRA;
```