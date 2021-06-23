# compress(algorithm, parameters[, callback])

Compress a whole directory or some files. If the destination exists, it will be overwrite.

- `algorithm` `<String>`: select the algorithm to use, values possible are: `7z`, `zip`, `tar`, `wim`
- `parameters` `<Object>`: parameters object to send
    - `dll` `<String>`: DLL path to use. If not specified, use the provided DLL file. _(global)_
    - `dir` `<String>`: directory to compress. Cannot be with `files` as the same time. _(global)_
    - `files` `<Array>`: files to compress. Cannot be with `dir` as the same time. _(global)_
    - `destination` `<String>`: directory or new archive file path. You can use a directory if `dir` is use. _(global)_
    - `level` `<Number>`: compression level to use. Only accept: -1, 0, 1, 3, 5, 7, 9. You can use the [CompressionLevel]() enum. _(`7z` and `zip` only)_
    - `password` `<String>`: password to use to compress the archive. _(`7z` only)_
    - `is64` `<Boolean>`: use Deflate64 or Deflate. _(`zip` only_if_unset)_
- `callback` `<Function>`
    - `error` `<Error>`
- `progressCallback` `<Function>`
    - `progress` `<Object>`: progress information
        - `total` `<Number>`: total bytes to compress
        - `progress` `<Number>`: progress
        - `ratio` `<Number>`: compression ratio
        - `fileProcessed` `<String>`: file currently processed

## Exemple
```js
const sevenZip = require('sevenzip-node');

// By set the destination as a directory, the archive name will be: test.7z
sevenZip.compress("7z", {dir: 'C:\\Users\\public\\Desktop\\test', destination: 'C:\\Users\\tcroi\\Desktop\\'}, (error) => {
    if (error) throw error;
}, (progress) => {
    console.log(progress);
});

// Compress the selected files with Deflate64
sevenZip.compress("zip", {files: ['C:\\Users\\public\\Desktop\\test\\bigtxt.txt', 'C:\\Users\\public\\Desktop\\test\\bigmp4.mp4'], destination: 'C:\\Users\\tcroi\\Desktop\\test.zip', is64: true}, (error) => {
    if (error) throw error;
}, (progress) => {
    console.log(progress);
});
```

### Using await
```js
const sevenZip = require('sevenzip-node');

// By set the destination as a directory, the archive name will be: test.7z
var error = await sevenZip.compress("7z", {dir: 'C:\\Users\\public\\Desktop\\test', destination: 'C:\\Users\\tcroi\\Desktop\\'}, null, (progress) => {
    console.log(progress);
});
if (error) throw error;

// Compress the selected files with Deflate64
error = sevenZip.compress("zip", {files: ['C:\\Users\\public\\Desktop\\test\\bigtxt.txt', 'C:\\Users\\public\\Desktop\\test\\bigmp4.mp4'], destination: 'C:\\Users\\tcroi\\Desktop\\test.zip', is64: true}, null, (progress) => {
    console.log(progress);
});
if (error) throw error;
```