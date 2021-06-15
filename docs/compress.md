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

## Exemple
```js
const sevenZip = require('./index');

// By set the destination as a directory, the archive name will be: test.7z
sevenZip.compress("7z", {dir: 'C:\\Users\\public\\Desktop\\test', destination: 'C:\\Users\\tcroi\\Desktop\\'}, (error) => {
    console.log(error);
});

sevenZip.compress("zip", {files: ['C:\\Users\\public\\Desktop\\test\\bigtxt.txt', 'C:\\Users\\public\\Desktop\\test\\bigmp4.mp4'], destination: 'C:\\Users\\tcroi\\Desktop\\test.zip', is64: true}, (error) => {
    console.log(error);
});
```

### Using await
```js
const sevenZip = require('./index');

// By set the destination as a directory, the archive name will be: test.7z
var error = await sevenZip.compress("7z", {dir: 'C:\\Users\\public\\Desktop\\test', destination: 'C:\\Users\\tcroi\\Desktop\\'});

error = sevenZip.compress("zip", {files: ['C:\\Users\\public\\Desktop\\test\\bigtxt.txt', 'C:\\Users\\public\\Desktop\\test\\bigmp4.mp4'], destination: 'C:\\Users\\tcroi\\Desktop\\test.zip', is64: true});
```