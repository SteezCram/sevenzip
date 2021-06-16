# extract(algorithm, parameters[, callback])

Extract an archive to the specified destination.

- `algorithm` `<String>`: select the algorithm to use, values possible are: `7z`, `zip`, `tar`, `wim`
- `parameters` `<Object>`: parameters object to send
    - `dll` `<String>`: DLL path to use. If not specified, use the provided DLL file. _(global)_
    - `archive` `<String>`: archive to extract. _(global)_
    - `destination` `<String>`: directory to extract the files. _(global)_
    - `password` `<String>`: password to use to compress the archive. _(`7z` only)_
- `callback` `<Function>`
    - `error` `<Error>`

## Exemple
```js
const sevenZip = require('sevenzip-node');

// By set the destination as a directory, the archive name will be: test.7z
sevenZip.extract("7z", {archive: 'C:\\Users\\public\\Desktop\\test.7z', destination: 'C:\\Users\\tcroi\\Desktop\\test'}, (error) => {
    if (error) throw error;
});
```

### Using await
```js
const sevenZip = require('sevenzip-node');

// By set the destination as a directory, the archive name will be: test.7z
var error = await sevenZip.extract("7z", {archive: 'C:\\Users\\public\\Desktop\\test.7z', destination: 'C:\\Users\\tcroi\\Desktop\\test'});
if (error) throw error;
```